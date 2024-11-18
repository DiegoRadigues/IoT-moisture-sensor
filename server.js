const express = require('express');
const http = require('http');
const socketIo = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

let gpsData = []; 

app.use(express.static('public'));


app.use(express.json());
app.post('/api/gpsdata', (req, res) => {
  const { latitude, longitude, humidity, plantType } = req.body;
  gpsData.push({ latitude, longitude, humidity, plantType });
  io.emit('gpsData', { latitude, longitude, humidity, plantType }); 
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
