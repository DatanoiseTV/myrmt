# _RMT Frequency Generator Example_

This example will shows how to configure and operate the remote control (RMT) peripheral to turn it into a programmable square frequency generator.


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

1. Compute the frequency generator parameters as well as the needed resouces. 
Does not create a frequency generator

params [-f | --freq <freq>] [-d | --duty <duty_cycle>]

2. Create a frequency generator bound to a GPIO pin but does not start it

create [-f | --freq <freq>] [-d | --duty <duty_cycle>] [ -g | --gpio ]

3. Starts frequency generator given by channel id (0-7)

start [-c | --channel]

4. Stops frequency generator given by channel id (0-7)

stop  [-c | --channel]

5. Delete Frequency generator given by channel id (0-7) and frees resources, including GPIO pin.

delete [-c | --channel]

# Issues

* WHen the number of items is exactly 64, 128, etc (including final EoTx zero mark), the wavefrom is generated only once
and then it stops
