#include <SPI.h>
#include <LoRa.h>
//unsigned char Hiall[] = "Nivel Agua";
 char Hiall[] = "Nivel Agua";
String Byte;              // outgoing message
long lastSendTime = 0;        // last send time

void setup(){
  Serial.begin(115200);  
  while (!Serial);
  Serial.println("LoRa gateway");
  if (!LoRa.begin(915600000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
 LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() 
{
  if(runEvery(3000)){
   LoRa.receive();
   
  }
  
  
}

void onReceive(int packetSize){
  if (packetSize == 0) return;        
  String incoming = "";
  while (LoRa.available()){
    incoming += (char)LoRa.read();
  }
  
  Serial.println("Mensaje recibido: ");
  Serial.println(incoming);
  //si recibe mensaje entonces envía
  HiAll(Hiall);
  

}

void HiAll(String Byte){
  LoRa.beginPacket();                   // start packet
  LoRa.print(Byte);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  Serial.println("Enviando: " + Byte);
  }
boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
