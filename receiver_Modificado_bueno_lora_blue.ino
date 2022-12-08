//receiver_ESP32_lora
//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//libreria para bluetooth
#include "BluetoothSerial.h"


//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915600000

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

/// Bluetooth
BluetoothSerial SerialBT;
////

char Hiall[] = "Dame nivel";
String Byte; 
String LoRaData;
String incoming = "";

void setup() { 
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) 
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEPTOR");
  display.display();

  //LoRa.onReceive(onReceive);
  LoRa.receive();
  
  //initialize Serial Monitor
  Serial.begin(115200);

  Serial.println("LoRa Receptor, Test");

   //inicializa bluetooth
  SerialBT.begin("Concentrador de lectura"); //Bluetooth device name
  Serial.println("Dispositivo listo, puedes vincular!");

  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Sensor inicializado OK!");
  display.setCursor(0,10);
  display.println("Sensor Inicialización OK!");
  display.display();  

  

}

void loop() {
   
  if (SerialBT.available())
  {
    char mychar=SerialBT.read();
    Serial.print("Mensaje de cell:");
    Serial.println (mychar);
      
    if(runEvery(3000)){
            HiAll(Hiall);
     }
  }
    int packetSize = LoRa.parsePacket();
    if (packetSize == 0) return;        
    while (LoRa.available()){
    incoming = LoRa.readString();
    int rssi = LoRa.packetRssi();
    
    // bluetooth
    SerialBT.print("SENSOR EN CASA ");
    SerialBT.print("Tamaño de paquete: ");
    SerialBT.print(packetSize);
    SerialBT.print("  ");
    SerialBT.print("Mensaje recibido: ");
    SerialBT.print(incoming);
    SerialBT.print("  ");
    SerialBT.print("RSSI: ");
    SerialBT.println(rssi);

    
   // Display information
   display.clearDisplay();
   display.setCursor(0,0);
   display.print("SENSOR EN CASA");
   display.setCursor(0,20);
   display.print("Mensaje recibido:");
   display.setCursor(0,30);
   display.print(incoming);
   display.setCursor(0,40);
   display.print("RSSI:");
   display.setCursor(30,40);
   display.print(rssi);
   display.display();   
  
  //delay por si acaso
   delay(10);
     
  }
  Serial.println("Nivel recibido: ");
  Serial.println(incoming);
   
   
   
   
   
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
