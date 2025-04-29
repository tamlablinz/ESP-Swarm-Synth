# ESP-Swarm-Synth
A swarm of ESP synths controlled by MIDI / PD Serial Data and synced by ESPnow protocol


ESP-Swarm-Synth is a distributed synthesizer system that leverages multiple ESP32 devices to create a synchronized swarm of sound generators using the Mozzi audio library. 
The system is controlled via MIDI or Pure Data (PD) serial data and uses the ESP-NOW protocol for synchronization across devices.


### 1. Sender (Arduino Code)
The sender ESP32 device receives MIDI or PD serial data and broadcasts control messages to all receiver devices using the ESP-NOW protocol.

### 2. Receiver (Arduino Code)
Each receiver ESP32-S2 device generates sound using the Mozzi audio library, processes control messages from the sender, and outputs audio signals at the DAC pin.

### 3. Pure Data (PD) Patch
The provided PD patch allows you to control the system visually and in real-time, sending serial data to the sender ESP32.

---

## Getting Started

### Prerequisites

- **Hardware**:
  - ESP32-S2 boards (1 sender and at least 1 receiver)
  - Audio output hardware (Speakers + amplifiers)

- **Software**:
  - Arduino IDE with ESP32 board support.
  - Required Arduino libraries: ESP-NOW, Mozzi
  - Pure Data

---

## Usage

### Step 1:
- Use 'Get_MAC_Address.ino' at the ESP32 receiver devices & update the MAC addresses in the sender code to match the receivers' MAC addresses.

### Step 2: 
- Upload the sender and receiver sketches to the ESP32 devices.

### Step 3: 
- Connect the DAC pins (`IO17` and `IO18`) of the receivers to your audio output hardware.

### Step 4: 
- Open the provided PD patch in Pure Data.
- Enter the correct COM port for your sender ESP32.

### Step 5: 
- Use the sliders to send control data to the sender ESP32. 
  Available features: 3 oscillators, 4 waveforms (sine/saw/triangle/square), white noise; separate volume control by sliders or ADSR values for envelopes; 

## Pure Data Patch Overview

The provided PD patch includes:
- **Frequency Controls**: Sliders to adjust oscillator frequencies.
- **Volume Controls**: Sliders for individual oscillator and noise volumes.
- **Waveform Selector**: Radio buttons to select waveform types.
- **ADSR Controls**: Adjustable attack, decay, sustain, and release parameters.

---

