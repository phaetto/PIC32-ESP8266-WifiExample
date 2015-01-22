# ESP8266 integrated with PIC32
This is the example project on how to use the https://github.com/phaetto/ESP8266 project.
Its a default project that has been customized with the required libraries.

## Setting up
You need a MCU with at least 35K bytes flash for this project without optimization.
The configuration runs the MCU at 20MHz.

###Hardware
You will need a FTDI USB to UART cable to test on hardware if you want to work on a terminal.
The default connection parameters for the terminal are (defined in user.h/user.c):
* Baud rate: 9600, data bits: 8, stop bit: 1, parity: none, flow: none
* UART2 pins are used

The default connection parameters for the WIFI are (defined again in user.h/user.c):
* Baud rate: 115200, data bits: 8, stop bit: 1, parity: none, flow: none
* UART4 pins are used
Have in mind that some versions are using different serial settings.

###Software
Clone https://github.com/phaetto/Commands
Then clone https://github.com/phaetto/ESP8266
And lastly clone this repository. Open in MPLAB X, build and deploy.
To connect to your router change the connections in user.c: [SSID] and [PASSWORD]

When running the example you should be able to see in the terminal your public ip coming from http://myip.dnsdynamic.org/

The example includes 2 services: one default (usually you do not need another one) and a second one that stays unconnected. It is only there as a proof of concept.

You can debug with terminal commands wifi-get/wifi-set.

## Customizing
The service definitions are included in CommandEngine.c.
The example requests that are used are defined in user.c.

The software is provided under The MIT License.
