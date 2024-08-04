#include <SoftwareSerial.h>
SoftwareSerial VSerial(2, 3); // RX, TX

const int flexPin = A0; 
#define MQ3pin A1
const int REED_PIN = 7;	// Pin connected to reed switch
const int SOS = 4;	// Pin connected to reed switch
float sensorValue;  //variable to store sensor value

void setup() 
{ 
  VSerial.begin(115200);
  pinMode(REED_PIN, INPUT_PULLUP); 
  pinMode(SOS, INPUT_PULLUP); 
  Serial.begin(9600);
  Serial.println("MQ3 warming up!");
	//delay(20000); // allow the MQ3 to warm up
} 

void loop() 
{ 
  sensorValue = analogRead(MQ3pin);
  int REED = digitalRead(REED_PIN);
  int SOSbtn = digitalRead(SOS);
  int flexValue;
  flexValue = analogRead(flexPin);
  //Serial.print("Flex Value");
 // Serial.println(flexValue);

 // Serial.print("MQ3 Value: ");
//Serial.println(sensorValue);


  if(SOSbtn == 0){
    Serial.println("SOS");
    VSerial.println("3");
    delay(1500);
  }
 // Serial.println("  ");
 // Serial.println("  ");
  if(flexValue<710 && sensorValue<400 && REED == 0) {
     Serial.println("Helemrt wared");
     VSerial.println("2");
     Serial.println("2");
     delay(1500);
  }
    else{
    Serial.println("Helmet not wared");
    VSerial.println("1");
    Serial.println("1");
    delay(1500);
    }
}



