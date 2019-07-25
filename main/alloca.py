#!/usr/bin/env python

RMT_CHANNEL_MAX = 8
FREQ_CHANNELS = [
	[7, 1, "Free"],
	[6, 1, "Free"],
	[5, 1, "Free"],
	[4, 1, "Free"],
	[3, 1, "Free"],
	[2, 1, "Free"],
	[1, 1, "Free"],
	[0, 1, "Free"],
]

def fgen_max_mem_blocks(index_to_channel):
	sum = 0
	for i in xrange(index_to_channel,-1,-1):
		if FREQ_CHANNELS[i][2] == "Free":
			sum += 1
	return sum

def fgen_channel_alloc(mem_blocks):

	for i in xrange(0, RMT_CHANNEL_MAX):
		N = fgen_max_mem_blocks(i)
		print("Exploring RMT channel {ch} with Maxblocks {N} when allocating {mem_blocks} blocks".format(ch=FREQ_CHANNELS[i][0], N=N, mem_blocks=mem_blocks))
		if FREQ_CHANNELS[i][2] == "Free" and mem_blocks <= N:
			FREQ_CHANNELS[i][2] = "Used"
			FREQ_CHANNELS[i][1] = mem_blocks
			print("RMT channel {ch} fits".format(ch=FREQ_CHANNELS[i][0]))
			if mem_blocks > 1:
				mem_blocks -= 1
				seq = range(i-1, i-mem_blocks-1, -1)
				print("Inutilizando canales superiores cuyos indices son {seq}".format(seq=seq))
				for j in seq:
					FREQ_CHANNELS[j][2] = "Unavailable"
					FREQ_CHANNELS[j][1] = 0
			return i, FREQ_CHANNELS[i]
	return "No More channels"

def fgen_channel_free(index_to_channel):
	if FREQ_CHANNELS[index_to_channel][2] == "Used":
		print("Freeng RMT channel {ch} whose index is {i}".format(ch=FREQ_CHANNELS[index_to_channel][0], i=index_to_channel))
		FREQ_CHANNELS[index_to_channel][2] = "Free"
		mem_blocks = FREQ_CHANNELS[index_to_channel][1]
		FREQ_CHANNELS[index_to_channel][1] = 1
		if mem_blocks > 1:
			mem_blocks -= 1
			seq = range(index_to_channel-1, index_to_channel-mem_blocks-1, -1)
			print("Restaurando canales superiores cuyos indices son {seq}".format(seq=seq))
			for j in seq:
					FREQ_CHANNELS[j][2] = "Free"
					FREQ_CHANNELS[j][1] = 1
	else:
		print("RMT channel {ch} whose index is {i} was already free".format(ch=FREQ_CHANNELS[index_to_channel][0], i=index_to_channel))

print(fgen_channel_alloc(mem_blocks=1))
print(fgen_channel_alloc(mem_blocks=2))
print(fgen_channel_alloc(mem_blocks=3))
print(fgen_channel_alloc(mem_blocks=3))
fgen_channel_free(index_to_channel=2)
fgen_channel_free(index_to_channel=0)
print(fgen_channel_alloc(mem_blocks=3))

print(FREQ_CHANNELS)
