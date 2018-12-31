# Salieri Drivers V0.1.0
----
These are a set of basic low level hardware drivers for the MSP432P401R development board, intended to be used for a 3D printer or a basic CNC machine.

### Prerequisites
* [Code Composer Studio](http://www.ti.com/tool/CCSTUDIO)
* [MSP432P401R Development Board](http://www.ti.com/tool/MSP-EXP432P401R)
* [Simplelink MSP432 SDK](http://www.ti.com/tool/SIMPLELINK-MSP432-SDK) 

### Getting Started
The drivers are designed to be used with code composer studio from Texas Instruments and the MSP432P401R development board. The following hardware drivers are supported on **V0.1.0**:
* PWM Driver
* ADC Driver
* GPIO Driver
* UART Driver
* Timer32 Driver

The *tests* subdirectory contains the **Salieri Board** test and **Salieri Drivers** static library project. To use them import the project into Code Composer Studio, compile and debug the projects. The **Salieri Board Test** is meant to be used with the [Salieri Board V0.2.0](https://easyeda.com/editor#id=20549971d6604598b88ad85f331f87c4|f895aaef3df34546a72c6eb9567a702a|1f3f72b8fa97468085e567bb571b500f|a4bc771082314921833b0450e7e573f9), to test the basic input/output used on the board. **USE THE BOARD/DRIVERS AT YOUR OWN RISK**. There is also a ccs sample project thet setups all the necessary dependencies.

### Versioning
Salieri V0.1.0, not ready for release still in progress

### Authors
Alejandro Martinez



