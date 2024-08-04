
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial VSerial(2, 3); // RX, TX
int num = 0; // Initialize integer variable to receive over serial
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16x2 display
String Location;
SoftwareSerial SIM900(9, 10); 
int motorPin1 = 7; // Input 1 for motor control
int motorPin2 = 8; // Input 2 for motor control
int enablePin = 6; // Enable pin for motor control
//---------------------------------------------------------------------------------

#define RX_PIN 4
#define TX_PIN 5
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
TinyGPS gps;

bool loopEnabled = true;
int Vib_sensorPin = A0; // AO pin of the module connected to 
void setup() {


  SIM900.begin(19200);
  delay(2000);

  Serial.begin(9600);
  delay(2000);

 // VSerial.begin(9600);
 // delay(2000);

 gpsSerial.begin(9600);
  delay(2000);

    lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  lcd.print("Please Wait"); 
  delay(1000);
  GPS();
 delay(9000);
 delay(2000);
 delay(2000);
}

void loop() {

  //Serial.println(Location);
//  if (VSerial.available() > 0) { // Check if data is available on serial port
 //   num = VSerial.parseInt(); // Read the integer variable from serial
    //Serial.println(num); // Print the received integer variable
 // }
    if (Serial.available() > 0) { // Check if data is available on serial port
    num = Serial.parseInt(); // Read the integer variable from serial
    //Serial.println(num); // Print the received integer variable
  }
  GPS();
  delay(500);
int Vib_sensor = analogRead(Vib_sensorPin); 
 Serial.println("Vib_sensor"); 
  Serial.println(Vib_sensor); 

  int State = num;
  int A = 3;
  int B = 2;
  int C = 1;
 
   if (!loopEnabled) {
        return; // Stop loop if loopEnabled is false
    }
    
    if (A == State) {
        delay(1000); 
      //GPS();
     // delay(2000); 
    Serial.println("SOS");
    Serial.println(Location);
    sendSMS(Location,"Emergency SOS");
    lcd.clear(); 
    lcd.setCursor(0, 0);             // Set cursor to first column of first row
    lcd.print("Emergency SOS"); 
    lcd.setCursor(0, 1);             // Set cursor to first column of second row
    lcd.print("Activated"); 
    }   

    if (Vib_sensor >500) {
        delay(500); 
        GPS();
        Serial.println("Accident");
        Serial.println(Location);
        sendSMS(Location,"Accident");
        motoroff();
        lcd.clear(); 
        lcd.setCursor(0, 0);             // Set cursor to first column of first row
        lcd.print("Accident"); 
        lcd.setCursor(0, 1);             // Set cursor to first column of second row
        lcd.print("Detected"); 
        loopEnabled = false; // Disable loop until next manual restart
        return; // Stop loop when B is low
        
    } else {
        if (B == State) {
          delay(500); 
            lcd.clear(); 
            Serial.println("Motor ON");
            motoron();
            lcd.setCursor(0, 0);             // Set cursor to first column of first row
            lcd.print("Helmet"); 
            lcd.setCursor(0, 1);             // Set cursor to first column of second row
           lcd.print("Detected");             // Print "World!" on second row             // Print "Hello" on first row
        }
    }


    if (C == State) {
      delay(1000); 
    lcd.clear(); 
    Serial.println("Motor OFF");
    motoroff();
    lcd.setCursor(0, 0);             // Set cursor to first column of first row
    lcd.print("Helmet"); 
    lcd.setCursor(0, 1);             // Set cursor to first column of second row
    lcd.print("Not Detected"); 
    
     }   
    
  
}

void GPS() {
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      float lat, lon;
      unsigned long fix_age, time, date;
      gps.f_get_position(&lat, &lon, &fix_age);
      gps.get_datetime(&date, &time, &fix_age);
      //Serial.print("Latitude: ");
      //Serial.println(lat, 6);
     // Serial.print("Longitude: ");
     // Serial.println(lon, 6);
     // Serial.println();
      String googleMapsLink = "http://maps.google.com/maps?q=loc:" + String(lat, 6) + "," + String(lon, 6);
     // Serial.println("Google Maps link: " + googleMapsLink);
      Location = googleMapsLink; 
      Serial.println(Location);
    }
  }
}


void sendSMS(String place, String message) {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r");
  delay(100);

  // replace X's with the recipient's mobile number
  SIM900.println("AT+CMGS=\"+917337209060\""); 
  delay(100);
  
  // replace with your own SMS message content
  SIM900.println(message);
  SIM900.println(place);
  Serial.println(message);
  Serial.println(place);
  delay(100);

  // end AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);

  Serial.println("SMS sent!");
}


void motoroff() {
  

  // Wait for 2 seconds
  delay(2000);
   // Set the motor to stop
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 0); // set motor speed to 0

  // Wait for 1 second
  delay(1000);

}



void motoron() {
  // Set the motor to rotate in one direction
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 127); // set motor speed to maximum (255)

  // Wait for 2 seconds
    // Wait for 1 second
  delay(1000);
}