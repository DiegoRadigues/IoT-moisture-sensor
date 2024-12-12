/* Heltec Automation LoRaWAN communication example
 *
 * Function:
 * 1. Upload node data to the server using the standard LoRaWAN protocol.
 *  
 * Description:
 * 1. Communicate using LoRaWAN protocol.
 * 
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 */

#include "LoRaWan_APP.h"
#include "Wire.h"
#include "GXHTC.h"

GXHTC gxhtc;

#define SOIL_MOISTURE_PIN 1 // GPIO connecté au capteur d'humidité du sol
#define AIR_VALUE 4095        // Valeur brute dans l'air
#define WATER_VALUE 1890    // Valeur brute dans l'eau

/* OTAA para */
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xB3, 0xE3 }; 
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xDE, 0x24, 0x9C, 0xE7, 0xAD, 0x32, 0xC5, 0xB2, 0x25, 0x0F, 0x40, 0x6E, 0x61, 0x76, 0x70, 0xB5 };

/* ABP para */
uint8_t nwkSKey[] = { 0x6C, 0x89, 0xCA, 0x23, 0xF7, 0x17, 0x67, 0x62, 0xF1, 0xA8, 0x09, 0xA4, 0x8B, 0x4E, 0x39, 0x26 };
uint8_t appSKey[] = { 0xDF, 0x06, 0x80, 0xFA, 0x56, 0x59, 0xE4, 0xD5, 0x84, 0xCD, 0xBC, 0x15, 0xCA, 0x6E, 0xA4, 0xC4 };
uint32_t devAddr =  ( uint32_t )0x260BBCCA;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/*LoraWan region, select in arduino IDE tools */
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported */
DeviceClass_t  loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP */
bool overTheAirActivation = true;

/*ADR enable */
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;

/* Number of trials to transmit the frame */
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port) {
  Wire.begin(21, 20);
  gxhtc.read_data();

  //Fake datas 
  float fakeSoilMoisturePercent = 42;

  // Lecture du capteur d'humidité du sol
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float soilMoisturePercent = map(soilMoistureValue, AIR_VALUE, WATER_VALUE, 0, 100);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  Serial.print(" %  Soil Moisture: ");
  Serial.print(soilMoistureValue);
  Serial.println(" %");

  delay(100);

  // Préparation du payload LoRaWAN
  unsigned char *puc;
  appDataSize = 0;

  // Température
  puc = (unsigned char *)(&soilMoisturePercent);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];

  puc = (unsigned char *)(&soilMoistureValue);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];

  puc = (unsigned char *)(&fakeSoilMoisturePercent);
  appData[appDataSize++] = puc[0];
  appData[appDataSize++] = puc[1];
  appData[appDataSize++] = puc[2];
  appData[appDataSize++] = puc[3];
  
  Wire.end();
}

// Setup du dispositif
void setup() {
  Serial.begin(115200);
  pinMode(SOIL_MOISTURE_PIN, INPUT); // Définir la broche comme entrée
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
}

// Boucle principale
void loop() {
  switch (deviceState) {
    case DEVICE_STATE_INIT:
      LoRaWAN.init(loraWanClass, loraWanRegion);
      LoRaWAN.setDefaultDR(3);
      deviceState = DEVICE_STATE_JOIN;
      break;

    case DEVICE_STATE_JOIN:
      LoRaWAN.join();
      break;

    case DEVICE_STATE_SEND:
      prepareTxFrame(appPort);
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;

    case DEVICE_STATE_CYCLE:
      txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;

    case DEVICE_STATE_SLEEP:
      LoRaWAN.sleep(loraWanClass);
      break;

    default:
      deviceState = DEVICE_STATE_INIT;
      break;
  }
}