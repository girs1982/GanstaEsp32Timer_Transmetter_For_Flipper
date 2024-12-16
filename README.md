//How to//
//1 
  #include "GanstaTransmitEsp32.h"
  const int RADIO_INPUT_PIN = 12; 	//select any pin, this is the TX-PIN
  const int RADIO_OUTPUT_PIN = 13; 	//select any pin, this is the RX-PIN
  GanstaTransmitEsp32 transmitterGansta(RADIO_INPUT_PIN);
//2 
void setup(){
	Serial.begin(115200);
  transmitterGansta.begin();
}
//3
 byte starline_code1[9] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,0x09};
//4 transmitt
void loop(){
//cc1101.setTx();
// Пример отправки данных
starline_code1[3]=random(0,255);
transmitterGansta.sendPacketStarline(starline_code1,8,30);
delay(3000);
//cc1101.setIdle();
}


//# GanstaEsp32Timer_Transmetter_For_Flipper
//GanstaEsp32Timer_Transmetter_For_Flipper
