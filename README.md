# 🔋 Smart Battery Management System (Smart-BMS)

### IoT-Enabled 3-Cell Lithium-Ion Battery Monitoring, Protection & Passive Cell Balancing

[![Platform](https://img.shields.io/badge/Platform-ESP32%20%7C%20Arduino-blue)]()
[![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-orange)]()
[![Simulation](https://img.shields.io/badge/Simulation-Proteus-green)]()
[![IoT](https://img.shields.io/badge/IoT-ThingSpeak%20%7C%20Dashboard-purple)]()

---

## 📌 Overview

The **Smart Battery Management System (Smart-BMS)** is an embedded and IoT-based system developed to monitor and manage a **3-cell Lithium-Ion battery pack**.

The system monitors **individual cell voltage, pack voltage, charging/discharging current, temperature, State of Charge (SOC), and cell balancing status**.

The project combines:

**Battery Management + Embedded Systems + IoT + Proteus Simulation**

---

## 🎯 Objectives

- Monitor individual battery cell voltages
- Measure charging and discharging current
- Monitor battery temperature
- Estimate battery State of Charge (SOC)
- Implement passive cell balancing
- Display battery parameters locally
- Send battery data to an IoT dashboard
- Validate the system using Proteus simulation

---

## ⚙️ Key Features

| Feature                         |Implementation
| 🔋 Cell Voltage Monitoring     | Voltage Divider 
| ⚡ Current Monitoring          | ACS712 
| 🌡️ Temperature Monitoring      | LM35 
| 📊 SOC Estimation              | Coulomb Counting 
| 🔄 Passive Balancing           | MOSFET + Resistor 
| 🖥️ Local Display               | LCD 
| 📡 IoT Monitoring              | ESP32 + Dashboard 
| 🧪 Simulation                  | Proteus 

---

## 🔄 Working Principle

The system continuously collects battery parameters through voltage, current, and temperature sensors.

The microcontroller processes the sensor data and determines the battery condition, including SOC and cell balancing status.

For passive balancing, the controller activates the corresponding balancing circuit when a cell voltage exceeds the defined balancing condition.

---

## 🔧 Hardware Components

- 3 × Lithium-Ion Cells
- Arduino / ESP32
- ACS712 Current Sensor
- LM35 Temperature Sensor
- Voltage Divider Circuit
- MOSFETs
- Balancing Resistors
- LCD Display
- Relay / Switching Components
- Connecting Components

---

## 💻 Software & Tools

- **Arduino IDE**
- **C/C++**
- **ESP32**
- **Proteus**
- **ThingSpeak / IoT Dashboard**
- **MATLAB**
- **GitHub**

---

## 📊 Parameters Monitored

- Cell 1 Voltage
- Cell 2 Voltage
- Cell 3 Voltage
- Pack Voltage
- Battery Current
- Temperature
- State of Charge (SOC)
- Cell Balancing Status

---

## 🧪 Proteus Simulation

The **Proteus** folder contains the simulation files used to develop and test the BMS.

The simulation is used to verify:

- Cell voltage monitoring
- Current measurement
- Temperature monitoring
- Passive balancing
- Microcontroller operation
- LCD output

---

## 📡 IoT Dashboard

The dashboard provides real-time visualization of important battery parameters.

### Dashboard Parameters

- Cell voltages
- Pack voltage
- Current
- Temperature
- SOC
- Balancing status

The dashboard can also be extended for historical data, graphs, alerts, and cloud-based monitoring.

