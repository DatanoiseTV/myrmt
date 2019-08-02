# Frequency Generator

4-channel square wave frequency generator based on ESP32's versatile RMT hardware module.

![Console](doc/screenshot2.png?raw=true)

Main characteristics:
* Up to 4 independent channel outputs on GPIO pin #5,, #18 #19, #21
* Frequency range from 0.01 Hz to 500 Khz
* Duty cycle between 0.01 and 0.99, 0.50 by default (square wave)
* command line interface using a serial console

# Configure the Project

```
make menuconfig
```

* Set serial port under Serial Flasher Options.

# Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Commands

The most important command is `help` which shows everythoing else, as shown below:

![Console](doc/screenshot1.png?raw=true)

# Design

The frequncy generator is based on these formulae:

```
Fclk = ( FREQ_APB / Prescaler ) [Hz] RMT internal clock
Fout = Fclk / N
```

Where `FREQ_APB` is 80 MHz, `Prescaler` is a value between 1 .. 255 and `N` is an arbitrary integer number. As we have two degrees of freedom, `Prescaler` and `N` are heuristically chosen to minimize roundoff errors.

As seen in the figure below, N eventually becomes the number of `Fclk` clock ticks and it is divided into low level tick count `NL` and high level tick count `NH`. The duty cycle becomes `NH / (NH + NL)`.

```
 <------------------------- NRep ---------------->
 <-------- N ------->
 <-- NH --><-- NL -->
 +--------+         +-- ...     +--------+        +-- ...
 |        |         |           |        |        |
-+        +---------+        ---+        +--------+
```

The number `N`, which can be quite large, is decomposed into [32 bit RMT items](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/rmt.html#transmit-data) forming a list that is loaded in RMT's internal RAM. When the RMT hardware reaches the end-of-list item, it loops again. According to ESP32 Technical manual, looping introduces a small `1/Fclk` delay (jitter):

>***Note***: When enabling the continuous transmission mode by setting
>RMT_REG_TX_CONTI_MODE, the transmitter will transmit the data on the channel
>continuously, that is, from the first byte to the last one, then from the first to the last again, and
>so on. In this mode, there will be an idle level lasting one clk_div cycle between N and N+1
>transmissions.

The frequency generator software tries to repeat the items `NRep` times before looping so that it minimizes jitter. Depending on the available RMT RAM this is not always possible. The available RMT internal RAM is divided into 8 64-item blocks and can be flexibily assigned to RMT channels (with some restrictions). Very low frequency generators such as 0.01 Hz can take up the whole RMT RAM. The `params` utility shows us some examples:


```bash
ESP32> params -f 500000
------------------------------------------------------------------
                 FREQUENCY GENERATOR PARAMETERS                   
Final Frequency:	500000.0000 Hz
Final Duty Cycle:	50.00%
Prescaler:		80
N:			2 (1 high + 1 low)
Nitems:			1, repeated x62
Blocks:			1 (64 items each)
Jitter:			1.000 us each 62 times
------------------------------------------------------------------
```

```bash
ESP32> params -f 5
------------------------------------------------------------------
                 FREQUENCY GENERATOR PARAMETERS                   
Final Frequency:	5.0000 Hz
Final Duty Cycle:	50.00%
Prescaler:		250
N:			64000 (32000 high + 32000 low)
Nitems:			1, repeated x62
Blocks:			1 (64 items each)
Jitter:			3.125 us each 62 times
------------------------------------------------------------------
```

```bash
ESP32> params -f 1
------------------------------------------------------------------
                 FREQUENCY GENERATOR PARAMETERS                   
Final Frequency:	1.0000 Hz
Final Duty Cycle:	50.00%
Prescaler:		250
N:			320000 (160000 high + 160000 low)
Nitems:			5, repeated x12
Blocks:			1 (64 items each)
Jitter:			3.125 us each 12 times
------------------------------------------------------------------
```

```bash
ESP32> params -f 0.04
------------------------------------------------------------------
                 FREQUENCY GENERATOR PARAMETERS                   
Final Frequency:	0.0400 Hz
Final Duty Cycle:	50.00%
Prescaler:		250
N:			8000000 (4000000 high + 4000000 low)
Nitems:			123, repeated x1
Blocks:			2 (64 items each)
Jitter:			3.125 us each 1 times
------------------------------------------------------------------
```

```bash
ESP32> params -f 0.03
------------------------------------------------------------------
                 FREQUENCY GENERATOR PARAMETERS                   
Final Frequency:	0.0300 Hz
Final Duty Cycle:	50.00%
Prescaler:		203
N:			13136290 (6568145 high + 6568145 low)
Nitems:			201, repeated x1
Blocks:			4 (64 items each)
Jitter:			2.537 us each 1 times
------------------------------------------------------------------
```

```bash
ESP32> params -f 0.01
------------------------------------------------------------------
                 FREQUENCY GENERATOR PARAMETERS                   
Final Frequency:	0.0100 Hz
Final Duty Cycle:	50.00%
Prescaler:		250
N:			32000000 (16000000 high + 16000000 low)
Nitems:			489, repeated x1
Blocks:			8 (64 items each)
Jitter:			3.125 us each 1 times
------------------------------------------------------------------
```
