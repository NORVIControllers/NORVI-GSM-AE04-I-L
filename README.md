# NORVI GSM-AE04-I-L – Hardware Test Example

## Overview

This example demonstrates how to run a full hardware test on the NORVI GSM-AE04-I-L controller.

The purpose of this example is to show how to:

- Start the Wi-Fi access point and print the device MAC address
- Initialize the OLED display and the two ADS1115 analog-to-digital converters
- Read the DS3231 RTC and print the date and time
- Mount the microSD card and print the card type and size
- Test the LTE modem using AT commands
- Read the status of the 6 digital inputs
- Read the six 4–20 mA analog inputs in mA
- Read the front-panel buttons
- Switch the 2 transistor outputs ON and OFF
- Send test data over RS-485

This example can be used as a starting point for developers building applications with NORVI controllers.

## Product Used

Product: NORVI GSM-AE04-I-L (ESP32-WROOM-32, LTE, microSD, DS3231 RTC, 6 digital inputs, 6 × 4–20 mA analog inputs, 2 transistor outputs, RS-485, OLED display)
Platform: Arduino IDE / PlatformIO (ESP32)

https://norvi.io

## Purpose of This Example

The example demonstrates how to:

- Start a Wi-Fi access point named after the device MAC address
- Initialize the SSD1306 OLED display over I2C (address 0x3C, SDA = GPIO 16, SCL = GPIO 17)
- Initialize the two ADS1115 ADCs (I2C addresses 0x48 and 0x49) and read the 4–20 mA inputs
- Read the DS3231 RTC (I2C address 0x68) and print the date and time
- Mount the microSD card (SPI, CS = GPIO 15) and print the card type and size
- Communicate with the LTE modem (EC25) over UART (RXD = GPIO 33, TXD = GPIO 32, RESET = GPIO 21)
- Read the 6 digital inputs (GPIO HIGH = input OFF, GPIO LOW = input ON)
- Read the front-panel button analog value on GPIO 36
- Drive the transistor outputs (OUTPUT 1 = GPIO 12, OUTPUT 2 = GPIO 2)
- Transmit text over RS-485 (RX = GPIO 25, TX = GPIO 26, flow control = GPIO 22)

Naming used in the sketch and on the Serial Monitor:

- INPUT 1 to INPUT 6 are Digital Input 0 to Digital Input 5 in the datasheet
- OUTPUT 1 and OUTPUT 2 are Transistor Output 0 (T0) and Transistor Output 1 (T1) in the datasheet
- Vin 0 to Vin 5 are Analog Input 0 to Analog Input 5 (A0 to A5) in the datasheet; the values printed are currents in mA

The program runs the following checks once at start-up, in this order:

- OLED display
- ADS1115 analog converters (0x48 and 0x49)
- Wi-Fi access point
- RTC (5 readings, one per second)
- microSD card
- LTE modem (`AT`, `AT+CPIN?`, `AT+GSN`, 10 seconds per command)

The program then continuously:

- Passes anything typed in the Serial Monitor to the LTE modem and prints the modem reply
- Prints the state of INPUT 1 to INPUT 6 (0 or 1) on one line
- Prints the current of the six analog inputs in mA (Vin 0 to Vin 5)
- Prints the front-panel button analog value
- Switches OUTPUT 1 ON, then OUTPUT 2 ON 150 ms later, then both outputs OFF after another 150 ms
- Sends `RS485 01 SUCCESS` over RS-485 and prints any data received

All results are shown on the Serial Monitor.

Follow the steps below to run the example:

### 1. Hardware Connections

- Power the controller with 24 V DC.
- Connect the mini USB cable to your computer.
- Insert a microSD card (optional, for the SD test).
- Insert a SIM card and connect the LTE antenna (for the modem test).
- Connect digital input signals (18–32 V DC, sink or source) to INPUT 1 to INPUT 6.
- Connect 4–20 mA signals to the 6 analog inputs.
- Connect loads (or a multimeter) to the transistor outputs.
- For the RS-485 test, connect an RS-485 device or a loopback to the RS-485 terminals.
- Ensure signals are within the supported input range.

### 2. Configure the Program

No additional user configuration is required for this example before upload.

### 3. Upload the Program

- Open the example program in Arduino IDE / PlatformIO
- Select the board **ESP32 Dev Module** and the correct port
- Upload the program to the NORVI controller

### 4. Observe the Serial Monitor

Open the Serial Monitor at **115200 baud**.

When the program runs successfully:

- The OLED display and both ADS1115 converters initialize
- The Wi-Fi access point starts and the MAC address, AP IP address and SSID are printed
- The RTC date and time are printed five times
- The SD card is mounted and its type and size are printed
- The LTE modem replies `OK` to `AT`, then returns the SIM status and IMEI
- The digital input states are printed continuously
- The analog input currents are printed continuously
- The front-panel button value is printed continuously
- The outputs switch ON and OFF
- `RS485 01 SUCCESS` is sent continuously over RS-485 and is printed back if a loopback is connected

Example Output:

```
Hello
MAC Address: XX:XX:XX:XX:XX:XX
Setting up the Wi-Fi Access Point...
Access Point IP address: 192.168.4.1
Access Point SSID: XX:XX:XX:XX:XX:XX
Wi-Fi Access Point is active!
2026/11/11 Wednesday10:15:32
2026/11/11 Wednesday10:15:33
2026/11/11 Wednesday10:15:34
2026/11/11 Wednesday10:15:35
2026/11/11 Wednesday10:15:36
Card Mount: success
Card Type: SDHC
Card Size: 7580MB
Testing Modem
AT
OK
+CPIN: READY
OK
XXXXXXXXXXXXXXX
OK
Testing Modem Done
*********************************
111111
Vin 0 : 0.00mA
Vin 1 : 0.00mA
Vin 2 : 0.00mA
Vin 3 : 0.00mA
Vin 4 : 0.00mA
Vin 5 : 0.00mA

Push button  4095

RS485 01 SUCCESS
```

(Sample values only. The six digits show INPUT 1–6: 1 = OFF, 0 = ON. The RS-485 line appears only when a loopback or another device echoes the data. If no card is inserted, `NO SD card` is printed instead of the card details.)



### How to Validate the Example

To verify the example works correctly:

1. Confirm the OLED display turns on and none of the `SSD1306 allocation failed`, `Failed to initialize ADS 1 .` or `Failed to initialize ADS 2 .` messages appear
2. Confirm the Wi-Fi access point appears on your phone or computer (SSID = device MAC address, password `12345678`)
3. Confirm the RTC prints a valid date and time
4. Confirm the SD card mounts (`Card Mount: success`) and the card type and size are printed
5. Confirm the modem returns `OK` for `AT`, a SIM status for `AT+CPIN?` and the IMEI for `AT+GSN`
6. Apply a valid signal to a digital input and observe the printed value change from 1 to 0
7. Apply a 4–20 mA signal to an analog input and observe the printed current change
8. Press each front-panel button and confirm the button value changes
9. Observe OUTPUT 1 switch ON, then OUTPUT 2 switch ON, then both switch OFF (or check with a multimeter)
10. Connect an RS-485 device or loopback and confirm the data is received

### Device Preparation / Configuration

Before running the example ensure:

- The controller is powered correctly
- Required signals are connected
- SIM card and antenna are installed
- Any required configuration is completed

## Required Libraries

Install the following libraries before compiling.

- SPI
- Wire
- Adafruit_GFX
- Adafruit_SSD1306
- Adafruit_ADS1X15
- FS
- SD
- RTClib
- WiFi

Installation:

1. Open Arduino IDE
2. Go to Library Manager
3. Search and install the required libraries (SPI, Wire, FS, SD and WiFi are included with the ESP32 core)

## Limitations

- This example is provided for demonstration purposes
- The RTC is set to the compile time if it has lost power
- If the RTC is not found, `Couldn't find RTC` is printed and the program continues
- The RTC date and time are printed without zero padding and without a space between the day name and the hour
- The OLED is initialized but no custom content is drawn in this example (only the library's default splash screen is shown)
- If the OLED or either ADS1115 is not detected, the program stops at start-up
- The microSD card type is read before the card is mounted, so the printed card type may show `Unknown`
- The LTE modem test runs once at start-up, takes about 30 seconds and needs a valid SIM card and network coverage
- After start-up, the modem is only reachable by typing commands in the Serial Monitor; typed commands are forwarded once per loop (about 0.7 seconds plus the ADC read time)
- The analog input currents are calculated with a 10 Ω shunt, a gain of 20 and a calibration factor of 2.014

## Safety Notes

- Do not exceed the rated input voltage (digital inputs 32 V DC maximum, analog inputs 38 V DC maximum)
- Do not exceed the transistor output rating (100 mA, 36 V DC maximum)
- Ensure proper grounding
- Disconnect power before wiring
- Incorrect wiring may damage the controller

Test Date: 11/11/2026  
Verified By: Kaveesha

## Support

Documentation:
https://norvi.io/docs

User guide: https://norvi.io/docs/norvi-gsm-ae04-i-l-user-guide/

For additional support or inquiries, contact the NORVI support team (support@norvi.io).

Datasheet : https://norvi.io/docs/norvi-gsm-ae04-i-l-datasheet/

## License

This example is provided for development and educational purposes.
