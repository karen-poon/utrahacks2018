#include "DHT.h"
#include <Servo.h> 

#define DHTTYPE DHT11   // DHT 11

#define DHTPIN 2
int servoPin = 9;
int LEDPin = 12;

int angle = 0;          // servo position in degrees
int servoSpeed = 15;    //speed of turning the servo
int previousValue;
int humy = 63;          //the humitidy cut off value

bool flag = 0;          //to check if humidity was previously higher or lower than humy

DHT dht(DHTPIN, DHTTYPE);
Servo servo;  

void setup() {
  Serial.begin(9600);
  dht.begin();
  servo.attach(servoPin);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH);   //LED is off when HIGH
  servo.write(15);

  float h = dht.readHumidity(); //current humidity value
  previousValue = h;           

  if (previousValue < humy) {
      for(angle = 25; angle > 0; angle--)
      {
          servo.write(angle);
          delay(servoSpeed);  
          digitalWrite(LEDPin, LOW);
      }
      flag = 1;
  }
  else if (previousValue >= humy) { 
      for(angle = 0; angle < 25; angle++)
      {                                
        servo.write(angle);
        delay(servoSpeed);
        digitalWrite(LEDPin, LOW);
      }
      flag = 0;
  }
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  digitalWrite(LEDPin, HIGH);
  
  float h = dht.readHumidity();
  
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");

  if (previousValue < humy) flag = 1;
  else flag = 0;
  
  //if humidity level was previously higher than or equal to cutoff, 
  //but is lower than cutoff now
  //open(servo up), water the plant
  if (h < humy && flag == 0) {
    for(angle = 25; angle > 0; angle--) //0 is up
    {                                  
      servo.write(angle);
      delay(servoSpeed);  //speed of turning the servo
      digitalWrite(LEDPin, LOW);
      Serial.println("Servo is moving");
    }
    previousValue = h;
  }

  //if humidity level was previously lower than cutoff, 
  //but is higher than or equal to cutoff now
  //close(servo down), stop watering
  else if (h >= humy && flag == 1) { 
    for(angle = 0; angle < 25; angle++)
    {                                
      servo.write(angle);
      delay(servoSpeed);
      digitalWrite(LEDPin, LOW);
      Serial.println("Servo is moving");
    }
    previousValue = h;
  }
}
