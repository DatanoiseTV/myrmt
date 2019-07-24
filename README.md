# _RMT Frequency Generator Example_

This example will shows how to configure and operate the remote control (RMT) peripheral to turn it into a programmable square frequency generator.


### Configure the Project

```
make menuconfig
```

* Set serial port under Serial Flasher Options.

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.


## Issues

* WHen the number of items is exactly 64, 128, etc (including final EoTx zero mark), the wavefrom is generated only once
and then it stops

* There is a significant delay (200ms) when loop is enabled.

For any technical queries, please open an [issue] (https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you soon.
