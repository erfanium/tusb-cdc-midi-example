# USB MIDI Device with Debugging via USB Serial

This project demonstrates how to build a USB MIDI device using the Raspberry Pi Pico, with an additional USB serial interface for debugging through `printf` and `stdio`. This allows the device to function as both a MIDI instrument and a serial port for debugging, perfect for embedded systems development ;)

## Features
- **MIDI Device**: Sends MIDI note sequences through the USB interface to any compatible MIDI host.
- **USB Debugging**: Sends serial output (e.g., using `printf`) through a USB CDC interface, allowing for easy debugging.
- **LED Blinking**: Visual feedback with a blinking LED based on the device's USB connection state.

## Getting Started

### Prerequisites
- Raspberry Pi Pico or compatible RP2040 device.
- TinyUSB library for USB support.
- [Pico SDK](https://github.com/raspberrypi/pico-sdk) installed and configured.
- A toolchain for compiling C code (e.g., GCC for ARM).

### Building the Project
1. Clone this repository:
   ```bash
   git clone https://github.com/erfanium/tusb-cdc-midi-example.git
   cd tusb-cdc-midi-example
   ```

2. Initialize and configure your Pico SDK environment:
   ```bash
   export PICO_SDK_PATH=/path/to/pico-sdk
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Flash the compiled `.uf2` file to your Raspberry Pi Pico:
   ```bash
   cp your_project.uf2 /media/youruser/RPI-RP2
   ```

### Running the Program
- **MIDI**: Once flashed, the device will send a predefined melody through the USB MIDI interface.
- **Serial Debugging**: Connect to the Pico's serial interface with a terminal emulator (e.g., `screen` or `minicom`):
  ```bash
  screen /dev/ttyACM0 115200
  ```
  You should see serial output like `Hello, Serial!` for debugging purposes.
