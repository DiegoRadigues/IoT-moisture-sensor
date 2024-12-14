# Roquette Sensor Project

The **Roquette Sensor Project** is an IoT-based solution for monitoring soil moisture levels in agricultural fields. It integrates a GPS module, a soil moisture sensor, and LoRa communication to provide real-time updates and alerts to farmers. The system is designed to help optimize field management by providing actionable insights for irrigation, planting, and harvesting. 

See the [Project report (PDF)]( https://ecambxl-my.sharepoint.com/:b:/g/personal/20342_ecam_be/EdgzUPAuNTlOnrlUAXVl4SIB9g5uTVqBaViRzQ9YcXfF1A?e=G56xyh)


---

## Features

- **Real-Time Soil Monitoring:** Periodic updates on soil moisture levels.
- **Critical Alerts:** Notifications when soil is too dry or wet.
- **GPS Integration:** Provides location-based data for easier deployment.
- **LoRa Communication:** Long-range, low-power connectivity.
- **Interactive Web Interface:** Visualize data on a map with detailed insights for each sensor.

---

## Project Structure

### Arduino Code
The Arduino firmware (`roquette_sensor.ino`) runs on the ESP32 and is responsible for:
- Reading data from the soil moisture sensor.
- Parsing GPS data via the NEO-7M module.
- Sending data over LoRa to The Things Network (TTN).

### Node.js Server
The Node.js server (`server.js`) handles:
- Receiving data from TTN via MQTT.
- Parsing and validating incoming data.
- Serving a web interface to display soil and GPS data.
- Managing real-time updates using WebSocket.

---

## Installation and Setup

### Prerequisites
- **Hardware:**
  - ESP32 with LoRa module.
  - NEO-7M GPS module.
  - YL-69 soil moisture sensor.
- **Software:**
  - [Arduino IDE](https://www.arduino.cc/en/software) with the required libraries:
    - LoRa library
    - TinyGPS
  - [Node.js](https://nodejs.org) installed on your system.

### Arduino Setup
1. Open the Arduino sketch located at `arduino/roquette_sensor/roquette_sensor.ino`.
2. Configure the LoRa and GPS settings as per your hardware.
3. Upload the code to the ESP32 board.

### Backend Setup
1. Clone the repository:
```bash
   git clone https://github.com/DiegoRadigues/IoT-moisture-sensor.git
   cd IoT-moisture-sensor
 ```

2. Install the dependencies:
```bash
  npm install
```

3. Start the server:
```bash
  node server.js
```

4. Open your browser and navigate to:
```arduino
  http://localhost:3000
```

---

## How It Works

1. The ESP32 collects soil moisture and GPS data.
2. Data is transmitted over LoRa to TTN.
3. TTN forwards the data to the Node.js server via MQTT.
4. The server processes the data and updates the web interface in real-time.

---

## Web Interface

The web interface allows users to:

- View sensor locations on an interactive map (powered by Leaflet.js).
- Monitor real-time soil moisture data with historical trends.
- Set thresholds and customize alerts for specific crops.
  
![interface_zoom](https://github.com/user-attachments/assets/dae14534-d5b5-4d39-8262-f6f53a98898d)

---

## Bill of Materials

| Component              | Quantity | Price (€) |
|-------------------------|----------|-----------|
| ESP32 LoRa (Heltec v3) | 1        | 20.39     |
| NEO-7M GPS Module      | 1        | 2.99      |
| YL-69 Soil Sensor      | 1        | 0.96      |
| LoRa Gateway           | 1        | 239.69    |
| Battery Shield         | 1        | 0.96      |
| 18650 Battery          | 2        | 1.79      |

**Total Cost:** €266.78 (including gateway)  
**Excluding Gateway:** €27.09

---

## Future Improvements

- Integrating additional sensors (e.g., pH, temperature, or pollutants).
- Adding battery monitoring.
- Migrating from JSON storage to a SQL database for better scalability.
- Implementing predictive analytics using machine learning.

---

## Contribution

All contributers are welcome! Please fork the repository, make your changes, and submit a pull request.
