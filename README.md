# Zephyr Sketch for Gy-485-44009 Sensor

This Arduino sketch demonstrates how to communicate with an Gy-485-44009 sensor via RS485. The example provided reads data from the sensor and calculates the lux value based on the received data frame.

## Dependencies

- nRF Connect for Desktop (tested with version 2.6.0)

## Hardware Requirements

- **nRF52832 Board**: Any nRF board compatible with the Zephyr, such as nRF52832.
- **RS-485 Module to TTL converter**: This module typically has RX (Receiver), TX (Transmitter), GND, and VCC pins.
- **Gy-485-44009 sensor**: High Precision RS485 Illuminance Light Intensity Brightness Sensor Module.

## Connections

# Between nRF52832 and 485 to TTL converter
    nRF52832                    485 to TTL converter
    VCC    --------------------->    VCC
    GND    --------------------->    GND
    RX     --------------------->    RXD
    TX     --------------------->    TXD

# Between Gy-485-44009 and 485 to TTL converter
    Gy-485-44009                   485 to TTL converter
        A+     --------------------->    A+
        B-     --------------------->    B-
# Between nRF52832 and Gy-485-44009
    Arduino Uno                  Gy-485-44009
    3.3 V  --------------------->    VCC
    GND    --------------------->    GND
    
## Installation

1. **Install nRF Connect for Desktop**: Download and install the nRF Connect for Desktop from [here]([https://www.arduino.cc/en/software](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-Desktop/Download)).

2. **Clone Repository**: Clone this repository to your local machine using the following command in your terminal or command prompt:

   ```bash
   git clone https://github.com/itsharshsrivastava/gy-485-44009-zephyr.git
