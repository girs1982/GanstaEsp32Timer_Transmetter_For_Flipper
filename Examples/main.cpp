//https://github.com/wladimir-computin/CC1101-ESP-Arduino
#include <CC1101_ESP_Arduino.h>
//https://github.com/wladimir-computin/espBitBanger
#include <EspBitBanger.h>
#include "GanstaTransmitEsp32.h"
//ESP32 PINs
#define MAX_ACTIONS 2000
const int SPI_SCK = 18; 			//board or mcu specific
const int SPI_MISO = 19; 			//board or mcu specific
const int SPI_MOSI = 23; 			//board or mcu specific
const int SPI_CS = 5; 				//select any pin
const int RADIO_INPUT_PIN = 12; 	//select any pin, this is the TX-PIN
const int RADIO_OUTPUT_PIN = 13; 	//select any pin, this is the RX-PIN
GanstaTransmitEsp32 transmitterGansta(RADIO_INPUT_PIN);
volatile bool busy = false; // Флаг, показывающий, идет ли сейчас передача данных
int SMALL=700;
int LONG=1400;  
int zu=0;
uint32_t shortz=250;
uint32_t longz=500;
int decodeMethod=0;
EspBitBanger bitBanger;
CC1101 cc1101(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS, RADIO_INPUT_PIN, RADIO_OUTPUT_PIN);
char string[16];
uint32_t f;
uint8_t buffer[] = {0b11101110, 0b10001110, 0b10001000, 0b10001110, 0b11101110, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b11101110, 0b10001110, 0b10001110, 0b10000000};
byte starline_code1[9] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,0x09};
void setup(){
	Serial.begin(115200);
    f = getCpuFrequencyMhz();
  sprintf(string, "CPU Freq: %i", f);
  Serial.println(string);
  //set to 40MHz
 // setCpuFrequencyMhz(80);
//	bitBanger.begin(2666, -1, RADIO_INPUT_PIN);
	cc1101.init();
	Serial.printf("CC1101: 0x%02x, version: 0x%02x\n", cc1101.getPartnum(), cc1101.getVersion());
	cc1101.setMHZ(433.92);
	cc1101.setTXPwr(TX_PLUS_10_DBM);
   /// cc1101.setChannel(1);
	cc1101.setDataRate(10000);
	cc1101.setModulation(ASK_OOK);	
	cc1101.setTx();
	for(int i = 0; i < 8; i++){
		delay(10);
	//	bitBanger.write(buffer, sizeof(buffer) / sizeof(buffer[0]));
	}
	cc1101.setIdle();
    // Настройка таймера
    // Инициализация таймера
    pinMode(RADIO_INPUT_PIN,OUTPUT);
  ///  setupTimer(); // Инициализируем таймер
   transmitterGansta.begin();
}
void loop(){
cc1101.setTx();
    // Пример отправки данных
byte starline_code2[96] = {0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF,0xBE, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x01, 0x01, 0x01,0xFF};
//starline_sendMan(starline_code1,7); 
starline_code1[3]=random(0,255);
transmitterGansta.sendPacketStarline(starline_code1,8,30);
delay(3000);
//if(!busy){repeatCount=30;}
//cc1101.setIdle();
}


