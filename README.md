# ESP-Swarm-Synth
A swarm of ESP synths controlled by MIDI / PD Serial Data and synced by ESPnow protocol


ESP-Swarm-Synth is a distributed synthesizer system that leverages multiple ESP32 devices to create a synchronized swarm of sound generators using the Mozzi audio library. 
The system is controlled via MIDI or Pure Data (PD) serial data and uses the ESP-NOW protocol for synchronization across devices.


### 1. Sender (Arduino Code)
The sender ESP32 device receives MIDI or PD serial data and broadcasts control messages to all receiver devices using the ESP-NOW protocol.

### 2. Receiver (Arduino Code)
Each receiver ESP32 device generates sound using the Mozzi audio library, processes control messages from the sender, and outputs audio signals.

### 3. Pure Data (PD) Patch
The provided PD patch allows you to control the system visually and in real-time, sending serial data to the sender ESP32.

---


## Getting Started

### Prerequisites

- **Hardware**:
  - ESP32 boards (1 sender and at least 1 receiver).
  - Audio output hardware (e.g., speakers or amplifiers).
  - Pure Data software installed on your computer.

- **Software**:
  - Arduino IDE with ESP32 board support.
  - Mozzi library for audio synthesis.

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/<your-repo>/ESP-Swarm-Synth.git
   cd ESP-Swarm-Synth
   ```

2. Install the required Arduino libraries:
   - ESP-NOW
   - Mozzi

3. Flash the sender and receiver codes to your ESP32 devices:
   - `1_to_many_SENDER.ino` for the sender.
   - `1_to_many_RECEIVER.ino` for the receivers.

---

## Usage

### Step 1: Configure Addresses
- Update the MAC addresses in the sender code (`1_to_many_SENDER.ino`) to match the receivers' MAC addresses.

### Step 2: Flash Firmware
- Upload the sender and receiver sketches to the ESP32 devices.

### Step 3: Connect Audio Output
- Connect the DAC pins (`DAC_PIN1` and `DAC_PIN2`) of the receivers to your audio output hardware.

### Step 4: Use Pure Data (Optional)
- Open the provided PD patch (`Pure_Data_Patch.pd`) in Pure Data.
- Configure the correct COM port and baud rate for your sender ESP32.

### Step 5: Control the Swarm
- Send MIDI or PD serial data to the sender ESP32.
- The receivers will generate synchronized sounds based on the control data.

---

## Serial Command Guide

The sender code interprets serial commands to control the parameters of the receivers. Below are the supported commands:

| Command | Description               |
|---------|---------------------------|
| `a,b,c` | Frequencies for Receiver 1 |
| `d,e,f` | Volumes for Receiver 1     |
| `g`     | Waveform for Receiver 1    |
| `h`     | Noise volume Receiver 1    |
| `i,...` | Similar commands for other receivers |

---

## Pure Data Patch Overview

The provided PD patch includes:
- **Frequency Controls**: Sliders to adjust oscillator frequencies.
- **Volume Controls**: Sliders for individual oscillator and noise volumes.
- **Waveform Selector**: Radio buttons to select waveform types.
- **ADSR Controls**: Adjustable attack, decay, sustain, and release parameters.

---

## Contributing

Contributions are welcome! Please fork this repository, make your changes, and submit a pull request.

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

## Acknowledgments

- **ESP-NOW**: A wireless communication protocol by Espressif.
- **Mozzi**: A powerful audio synthesis library for Arduino.
- **Pure Data**: An open-source visual programming language for multimedia.



