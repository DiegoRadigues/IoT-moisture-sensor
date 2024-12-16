const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const mqtt = require('mqtt');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

let gpsData = [];

// TTN MQTT Configuration 
const APP_ID = "groupe-i@ttn";  // Remplace par ton App ID
const API_KEY = "NNSXS.M6L3NOMEGI66IPZ4H5DZ2CBWJRW2OK4DO4KAQRI.7B4APQPW2RGIDIXRKFRPYXB5D2V7ARAOSEDKQHUTTTLX7PHZAJUA"; // Remplace par ton API Key
const BROKER = "eu1.cloud.thethings.network";
const PORT_MQTT = 1883;
const TOPIC = `v3/${APP_ID}/devices/+/up`;

// Connexion au broker MQTT
const mqttClient = mqtt.connect(`mqtt://${BROKER}`, {
  username: APP_ID,
  password: API_KEY
});

// Quand le client MQTT est connecté, s'abonner au topic
mqttClient.on('connect', () => {
  console.log('Connecté au broker MQTT');
  mqttClient.subscribe(TOPIC, (err) => {
    if (err) {
      console.log('Erreur lors de l\'abonnement:', err);
    } else {
      console.log(`Abonné au topic: ${TOPIC}`);
    }
  });
});

// Quand un message est reçu sur le topic MQTT
mqttClient.on('message', (topic, message) => {
  const payload = JSON.parse(message.toString());
  console.log("Message reçu depuis TTN :");
  console.log(payload);

  if (payload.uplink_message && payload.uplink_message.decoded_payload) {
    // Extraire les données utiles depuis decoded_payload
    const {
      fakeLaltitude: latitude,
      fakeLongitude: longitude,
      humidity
    } = payload.uplink_message.decoded_payload;

    const plantType = "Default"; // Par défaut ou autre valeur statique

    // Vérification des données valides
    if (latitude && longitude && humidity !== undefined) {
      console.log(`Données valides reçues : Latitude=${latitude}, Longitude=${longitude}, Humidity=${humidity}`);
      gpsData.push({ latitude, longitude, humidity, plantType });
      io.emit('gpsData', { latitude, longitude, humidity, plantType });
    } else {
      console.log("Données incomplètes reçues, ignorées.");
    }
  }
});


app.use(express.static('public'));
app.use(express.json());

app.post('/api/gpsdata', (req, res) => {
  const { latitude, longitude, humidity, plantType } = req.body;
  gpsData.push({ latitude, longitude, humidity, plantType });
  io.emit('gpsData', { 
    latitude: payload.uplink_message.decoded_payload.fakeLaltitude,
    longitude: payload.uplink_message.decoded_payload.fakeLongitude,
    humidity: payload.uplink_message.decoded_payload.humidity,
    plantType: "Default" 
  });
  
  res.sendStatus(200);
});

io.on('connection', (socket) => {
  console.log('Client connected');
  socket.emit('gpsData', gpsData);
});

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
