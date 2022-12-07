#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
#define SS_PIN D4  // sda
#define RST_PIN D3
 
int lock = D0;    
 
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
char auth[] ="xxx";    //Blynk Authentication Token -- sent via Email from Blynk
char ssid[] = "xxx";   //Enter WiFi Name
char pass[] = "xxx";   //Enter Wifi Password
 
SimpleTimer timer;
int fflag = 0; 
int eflag = 0; 
int jflag = 0; 
WidgetTerminal terminal(V2);
 
void setup() {
    Serial.begin(9600);        // Initialize serial communications with the PC
    Blynk.begin(auth, ssid, pass);
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();        // Init MFRC522 card
    pinMode(lock,OUTPUT);
    digitalWrite(lock, HIGH);
    //Serial.println("Scan a MIFARE Classic PICC to demonstrate Value Blocks.");
    timer.setInterval(1000, iot_rfid);
}
 
void loop() {


  Blynk.run();
  timer.run(); // Initiates SimpleTimer
       
}
 
void iot_rfid()
{
 
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;
        }
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }
 
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
                return;
        }
        // Now a card is selected. The UID and SAK is in mfrc522.uid.
        
        // Dump UID
        Serial.print("Card UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
               Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
               Serial.print(mfrc522.uid.uidByte[i], DEC);
        } 
        Serial.println();
 
        // Dump PICC type
        byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    //    Serial.print("PICC type: ");
//Serial.println(mfrc522.PICC_GetTypeName(piccType));
        if (              piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
                &&        piccType != MFRC522::PICC_TYPE_MIFARE_1K
                &&        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
                //Serial.println("This sample only works with MIFARE Classic cards.");
                return;
        }
        
        // Enter RFID Tag ID here 
        if( ((mfrc522.uid.uidByte[0] == 66) && (mfrc522.uid.uidByte[1] == 22) && (mfrc522.uid.uidByte[2] == 165) && (mfrc522.uid.uidByte[3] == 86)) && (fflag == 1) )
        {
         Serial.println("user1");              
         Blynk.virtualWrite(V2,"user1");         
         digitalWrite(lock, LOW);                 
         delay(3000);                           
         digitalWrite(lock, HIGH);                 
        }
  
        else
        if(( (mfrc522.uid.uidByte[0] == 226) && (mfrc522.uid.uidByte[1] == 157) && (mfrc522.uid.uidByte[2] == 201) && (mfrc522.uid.uidByte[3] == 85))&& (eflag == 1) )
        {
         Serial.println("user2");            
         Blynk.virtualWrite(V2,"user2");             
         digitalWrite(lock, LOW);               
         delay(3000);                            
         digitalWrite(lock, HIGH);                 
        }
        else
        if( ((mfrc522.uid.uidByte[0] == 227) && (mfrc522.uid.uidByte[1] == 94) && (mfrc522.uid.uidByte[2] == 227) && (mfrc522.uid.uidByte[3] == 17))&& (jflag == 1) )
        {
         Serial.println("user3");                    
         Blynk.virtualWrite(V2,"user3");             
         digitalWrite(lock, LOW);                  
         delay(3000);                              
         digitalWrite(lock, HIGH);              
        }
 
        else 
         Serial.println("User not recognized");    
  
}
 
// in Blynk app writes values to the Virtual Pin 3
BLYNK_WRITE(V3)
{
   fflag = param.asInt(); // assigning incoming value from pin V3 to a variable
  // Blynk.virtualWrite(V2, fflag );
 
}
 
 
// in Blynk app writes values to the Virtual Pin 4
BLYNK_WRITE(V4)
{
   eflag = param.asInt(); // assigning incoming value from pin V4 to a variable
 
}
 
BLYNK_WRITE(V5)
{
   jflag = param.asInt(); // assigning incoming value from pin V5 to a variable
 
}
