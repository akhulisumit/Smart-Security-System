#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 24);

 
#define SS_PIN 9
#define RST_PIN 8
#define Buzz 7
#define IR 6
MFRC522 mfrc522(SS_PIN, RST_PIN);  
Servo s1;
Servo s2;
int ppm;
int val = 0 ;
const int MQ5_PIN = A1;
const int co2 = A2;
const int thres_co2 = 450;
const int thres_co = 400;
const int rain = 48;

 // Create MFRC522 instance.
void LPG_check(){
  Serial.print("LPG  :"+ppm);
  }
  void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
   //Forward what Serial received to Software Serial Port
  }
 
}

void msgSend(String err){
 

  }
void setup()
{
   espSerial.begin(115200);
  pinMode(22, OUTPUT);
 
  digitalWrite(22, HIGH);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
 
s1.attach(3);
s2.attach(49);
s1.write(0);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(IR, INPUT);
pinMode(Buzz, OUTPUT);// buzzer
pinMode(2, OUTPUT); //redlight
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
pinMode(13, OUTPUT);
pinMode(rain, INPUT);
pinMode(51, OUTPUT); //motor

}

void loop()
{
  if(digitalRead(rain) == 0){
    digitalWrite(13, HIGH);
    digitalWrite(Buzz, HIGH);
    }
    else{
      digitalWrite(13, LOW);
    digitalWrite(Buzz, LOW);
      }
 
 int sensorValue = analogRead(MQ5_PIN); // Read the analog value from MQ-5 sensor
  float voltage = sensorValue * (5.0 / 1023.0); // Convert the analog value to voltage (assuming 5V reference)
  float ppm = 1000.0 * (0.23 * voltage - 0.16);

  Serial.println("CO2: "+String(analogRead(A2)));
   //LPG_check();
   
  if(digitalRead(IR) == LOW){
   
    digitalWrite(Buzz, HIGH);
    digitalWrite(2, HIGH);
    delay(5000);
    digitalWrite(2, LOW);
    }
  else{
      digitalWrite(Buzz, LOW);
    }

    int sensorValue2 = analogRead(A0);// print out the value you read:
  Serial.println("CO:   "+String(sensorValue2)); //carbon monoxide mq7
  delay(20); //carbon monoxide mq7

  if (sensorValue2 > 500){
    espSerial.println("Alert! <br> <b>Something is burning in Completely.</b> <br><i> <u> Sensor Found high amount of CO </u></i>");
    digitalWrite(22, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(47, 1);
    digitalWrite(Buzz, HIGH);
    digitalWrite(8, HIGH);
    s2.write(135);
    digitalWrite(51, HIGH);
    delay(5000);
  }
  else{
digitalWrite(47, LOW);
digitalWrite(Buzz, LOW);
s2.write(0);
  digitalWrite(22, HIGH);
digitalWrite(51, LOW);
   
  }


   if (sensorValue > 350){
    espSerial.println("Alert! <br> <b>Your Home is about to blast.</b> <br><i> <u> Sensor Found leakage of lpg </u></i>");
   
    digitalWrite(7, LOW);
    digitalWrite(Buzz, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(10, HIGH);
    s2.write(135);
    digitalWrite(22, LOW);
    delay(5000);
  }
  else{
digitalWrite(10, LOW);
digitalWrite(Buzz, LOW);
digitalWrite(22, HIGH);
s2.write(0);
   
  }

 
  if(analogRead(A2)>thres_co2){
      espSerial.println("Alert! <br> <b>Fire, Something is burning. Be Alert .</b> <br><i> <u> Sensor Found High Amount of CO2</u></i>");
   
    //co2
    digitalWrite(Buzz, HIGH);
     digitalWrite(7, HIGH);
     digitalWrite(12, 1);
     s2.write(135);
     digitalWrite(22, LOW);
  }
  else{
digitalWrite(12, LOW);
s2.write(0);
digitalWrite(22, HIGH);
    delay(300);
  }



 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  ("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "17 9D D2 4E" || content.substring(1) == "A7 31 D6 4E" || content.substring(1) == "67 A1 E3 4D" || content.substring(1)=="69 9F 20 A3" || content.substring(1)=="A9 D0 4E A3" || content.substring(1)=="79 BE E3 A3")  //change here the UID of the card/cards that you want to give access
  {
    s1.write(135);
    digitalWrite(4, HIGH);
    Serial.println("Authorized access");
        msgSend("Authorized access");

    Serial.println();
    delay(1000);
    s1.write(0);
   
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
 
 else   {
   digitalWrite(5, HIGH);
   

    Serial.println(" Access denied");
    delay(3000);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
} 
