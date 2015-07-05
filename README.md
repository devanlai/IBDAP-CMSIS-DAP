# IBDAP-CMSIS-DAP
Armstart's CMSIS-DAP firmware implementation in gcc and makefile.

[http://www.armstart.com](http://www.armstart.com)

[IBDAP](https://hackaday.io/project/6578-ibdap-affordable-cmsis-dap-jtagswd-debug-probe) is a fully CMSIS-DAP compatible debug adapter. It provides vendor independent debug interface between your PC over USB and target ARM device over JTAG/SWD pins. You can do debugging functions like stepping, breakpoints, watch points and firmware download etc. It's fully supported by Keil, OpenOCD, GNU GDB, IAR and other commonly used debugging tools.

# Why IBDAP?

Debug adapters are expensive, some could cost thousands of dollars, some may not be compatible among different vendors. Luckily, ARM standardized the debugging interface which is called CMSIS-DAP and released the firmware implementation on some processors, however, you still need a Keil MDK Professional edition software in order to build the firmware, even the open source one provided by mbed and the price for Keil Professional is intimidating. All these barriers has become the first issue that every inventor is facing, and we need a solution!
IBDAP's objective is to become an affordable open source & open hardware CMSIS-DAP JTAG/SWD debug probe implementation using gcc & makefile. Anyone can modify and embed a debug probe on its own device easily with everything under its control.

Besides being functioning as a JTAG/SWD debug probe, IBDAP could also be used as a general development board. It has an ARM Cortex M0 processor  running at 48Mhz and has peripherals like UART, I2C, SPI, USB. It can be used in many applications like USB audio devices, USB mouse/keyboards, USB mass storage devices, USB-TTL adapter device and many many more. Moreover, the 10-bit high precision ADC peripheral also makes IBDAP an ideal device for any sensor projects.

![IBDAP pinmap](https://s3.amazonaws.com/armstart/Debug+Tools/IBDAP-LPC11U35/7.jpg)
 
