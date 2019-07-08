#!/usr/bin/env python

'''
Digital function generator

Calculate parameters for the ESP32 RMT module
and implement a digital wavefor generator
with programable frquency and duty cycle


From:
		Tclk = Prescaler / FREQ_APB			[1]
		1/Fout = Tclk*(Nhigh + Nlow)	[2]
		Dcyc = Nhigh / (Nhigh + Nlow)	[3]

	we obtain

		Nlow = Nhigh * (1 - Dcyc) / Dcyc	[4]
		Nhigh + Nlow = 1 / (Fout * Tclk)	[5]

	and finally the values of Nlow and Nhigh

		Nhigh = Dcyc / (Fout * Tclk)
		Nlow  = (1 - Dcyc) / (Fout * Tclk)

		with the rrestriction that Nhigh, Nlow >= 1

'''

from __future__ import division
import math


# FREQ_APB Clock is 80 MHz
FREQ_APB = 80000000.0

def reparte(Fout):

	whole = round(FREQ_APB/Fout)
	

	Prescaler = 255
	err, N    = math.modf(whole/Prescaler)
	
	while Prescaler > 1:
		new_err, new_N = math.modf(whole/Prescaler)
		if new_err == 0 and new_N > 1:
			err = new_err
			N   = new_N
			break
		elif new_err < err:
			err = new_err
			N   = new_N
		Prescaler -= 1

	if (Prescaler == 2 and err != 0.0):
		Prescaler = 1
		N = whole
	return Prescaler, int(N)
	


# Fout - Output Frequency to generate
# Dcy  - Duty Cycle (0.0 < Dc < 1.0)


def makeItemsList(Nhigh, Nlow):
	items = []

	# quick way out
	if (Nhigh < 32768 and Nlow < 32768):
		items.append((Nhigh,1,Nlow,0))
		items.append((0,0,0,0))
		return items

	# Long high period
	while Nhigh > 32767*2:
		Nhigh   -= 32767*2
		items.append((32767,1,32767,1))

	# Ending high part
	if 32767 < Nhigh <= 32767*2:
		Nhigh -= 32767
		items.append((32767,1,Nhigh,1))
	else:
		padding = min(Nlow, 32767)
		items.append((Nhigh,1,padding,0))
		Nlow -= padding

	# Long low period
	while Nlow > 32767*2:
		Nlow   -= 32767*2
		items.append((32767,0,32767,0))

	# Ending low part
	if 32767 < Nlow <= 32767*2:
		Nlow -= 32767
		items.append((32767,0,Nlow,0))
	elif Nlow > 0:
		items.append((Nlow,0,0,0))

	items.append((0,0,0,0))  # End of transmission item
	return items

def checkWaveLength(items):
	highs =      [i[0] for i in items if i[1] == 1]
	highs.extend([i[2] for i in items if i[3] == 1])
	lows  =      [i[0] for i in items if i[1] == 0]
	lows.extend( [i[2] for i in items if i[3] == 0])
	return sum(highs), sum(lows)

def countItemsList(Nhigh, Nlow):
	count = 0

	# quick way out
	if (Nhigh < 32768 and Nlow < 32768):
		count += 2  # item + End of transmission
		return count

	# Long high period
	while Nhigh > 32767*2:
		Nhigh   -= 32767*2
		count += 1

	# Ending high part
	if 32767 < Nhigh <= 32767*2:
		Nhigh -= 32767
		count += 1
	else:
		padding = min(Nlow, 32767)
		Nlow -= padding
		count += 1
		
	# Long low period
	while Nlow > 32767*2:
		Nlow   -= 32767*2
		count += 1

	# Ending low part
	if 32767 < Nlow <= 32767*2:
		Nlow -= 32767
		count += 1
	elif Nlow > 0:
		count += 1

	count += 1 # End of transmission item
	return count
	

def calculate(Fout, Dcyc):
	'''Calculate RMT parameters given Fo and Dc parameters'''

	Prescaler, Ntot = reparte(Fout)
	Tclk  = Prescaler / FREQ_APB 
	Nhigh = Ntot * Dcyc  # still floating point
	Nlow  = Ntot - Nhigh # still floating point

	if (Nhigh < 1):
		print("Can't continue because Nhigh = {Nhigh:f} < 1".format(Nhigh=Nhigh))
		return
	if (Nlow < 1) :
		print("Can't continue because Nlow = {Nlow:f} < 1".format(Nlow=Nlow))
		return

	# Haciendo esto primamos conservar el duty cycle a costa de modificar la frecuencia
	Nhigh = int(round(Nhigh))
	Nlow  = int(round(Nlow))
	Ntot  = Nhigh + Nlow

	# Y asi es como queda la cosa
	Fout2 = FREQ_APB / (Prescaler*Ntot)
	Dcyc2 = Nhigh    / (Nhigh + Nlow)
	ErrFreq = (Fout2 - Fout)/Fout
	ErrDcyc  = (Dcyc2 - Dcyc)/Dcyc

	print("*************************************************************")
	print("Ref Clock = {FREQ_APB:.0f} Hz, Prescaler = {Pr:d}, RMT Clock = {RMT:.2f} Hz".format(FREQ_APB = FREQ_APB, Pr = Prescaler, RMT = 1/Tclk))
	print("Fout = {Fout:.3f} Hz => {Fout2:.3f} Hz ({Err1:0.2f}%), Duty Cycle = {Dcyc:.2f}% => {Dcyc2:.2f} ({Err2:0.2f}%)".format(
		Fout = Fout, Fout2 = Fout2, Dcyc = Dcyc*100, Dcyc2=Dcyc2*100, Err1=ErrFreq*100, Err2=ErrDcyc*100))
	print("Ntot = {Ntot:d}, Nhigh = {Nhigh:d}, Nlow = {Nlow:d}".format(Ntot = Ntot, Nhigh = Nhigh, Nlow = Nlow))


	Nitems = countItemsList(Nhigh, Nlow)
	Nchan = 1 + (Nitems // 64) if Nitems > 0 else 0
	print("Nitems = {Nitems:d}, NChannels = {Nchan:d}".format(Nitems=Nitems,Nchan=Nchan))
	if Nchan > 8:
		print("Sorry, can't generate such waveform even with 8 channels")
		return


	Nrep = Nchan * 63 // (Nitems -1) 
	print("This sequence can be repeated {Nrep:d} times + final EoTx (0,0,0,0)".format(Nrep=Nrep))
	jitter = 1/(Ntot*Nrep)
	print("Jitter due to repetition {jitter:0.4f}%".format(jitter=jitter*100))


	items = makeItemsList(Nhigh,Nlow)
	print(makeItemsList(Nhigh,Nlow))

	Th, Tl = checkWaveLength(items)
	if(Nhigh != Th):
		print(" =====> error in High level sums {0}".format(Nhigh - Th))
	if(Nlow != Tl):
		print(" =====> error in Low  level sums {0}".format(Nlow - Tl))
	

if __name__ == "__main__":
	calculate(0.009,   0.50)
	calculate(0.008,   0.75)
	calculate(0.009,   0.50)
	calculate(0.01,    0.75)
	calculate(0.01,    0.5)
	calculate(0.02,    0.5)
	calculate(0.02,    0.1)
	calculate(0.025,   0.5)
	calculate(0.05,    0.75)
	calculate(0.05,    0.5)
	calculate(0.1,     0.5)
	calculate(0.1,     0.63)
	calculate(0.1,     0.005)
	calculate(0.1,     1-0.005)
	calculate(509,     0.5)
	calculate(5,       0.5)
	calculate(5,       1-0.005)
	calculate(5,       0.5)
	calculate(6473,    0.5)
	calculate(43247,   0.7)
	calculate(50000,   0.7)
	calculate(70729,   0.5)
	calculate(100000,  0.5)
	calculate(500000,  0.5)