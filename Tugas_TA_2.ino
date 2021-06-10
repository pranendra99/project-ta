/*
  This example shows how to measure the power consumption
  of devices in AC electrical system
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "ZMPT101B.h"
#include "ACS712.h"

// Set these to run example.
#define FIREBASE_HOST "kontrol-relay-8906b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "iaH486tbsu1K3qKQPlwtrlTy1O5aiQVZODS2RHup"
#define WIFI_SSID "SD kraton 3"
#define WIFI_PASSWORD "sdk3okebgt"

    /* 0- General */
#define S0 D0                             /* Assign Multiplexer pin S0 connect to pin D0 of NodeMCU */
#define S1 D1                             /* Assign Multiplexer pin S1 connect to pin D1 of NodeMCU */
#define S2 D2                             /* Assign Multiplexer pin S2 connect to pin D2 of NodeMCU */
#define S3 D3                             /* Assign Multiplexer pin S3 connect to pin D3 of NodeMCU */
#define SIG A0                            /* Assign SIG pin as Analog output for all 16 channels of Multiplexer to pin A0 of NodeMCU */
    

//int decimal = 2;                          //Decimal places of the sensor value outputs 
int sensor0;                            /* Assign the name "sensor0" as analog output value from Channel C0 */
int sensor1;                            /* Assign the name "sensor1" as analog output value from Channel C1 */
int sensor2;                            /* Assign the name "sensor2" as analog output value from Channel C2 */
int sensor3;                            /* Assign the name "sensor3" as analog output value from Channel C3 */
int sensor4;                            /* Assign the name "sensor4" as analog output value from Channel C4 */
int sensor5;                            /* Assign the name "sensor5" as analog output value from Channel C5 */


ZMPT101B voltageSensor(A0);

ACS712 currentSensor(ACS712_05B, A0);

//ACS712  ACS(A0, 5.0, 1023, 185);

void setup(){
  Serial.begin(9600);
  
  pinMode(S0,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin SO */        
  pinMode(S1,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin S1 */  
  pinMode(S2,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin S2 */ 
  pinMode(S3,OUTPUT);                       /* Define digital signal pin as output to the Multiplexer pin S3 */  
  pinMode(SIG, INPUT);                      /* Define analog signal pin as input or receiver from the Multiplexer pin SIG */  
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  
        // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  delay(100);
  voltageSensor.calibrate();
  currentSensor.calibrate();
//  ACS.autoMidPoint();
//  Serial.print("MidPoint: ");
//  Serial.print(ACS.getMidPoint());
//  Serial.print(". Noise mV: ");
//  Serial.println(ACS.getNoisemV());
  
  Serial.println("Done!");
  delay(1000);
 
}

void loop(){
  int val1 = Firebase.getInt("relay1");
  delay(100);
  int val2 = Firebase.getInt("relay2");
  delay(100);
  int val3 = Firebase.getInt("relay3");
  delay(100);
  int val4 = Firebase.getInt("relay4");
  delay(100);

  digitalWrite(D4, val1);
  delay(50);
  digitalWrite(D5, val2);
  delay(50);
  digitalWrite(D6, val3);
  delay(50);
  digitalWrite(D7, val4);
  delay(100);


  // Channel 0 (C0 pin - binary output 0,0,0,0)
  digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  sensor0 = analogRead(SIG);
  delay(100);

  // Channel 1 (C1 pin - binary output 1,0,0,0)
  digitalWrite(S0,HIGH); digitalWrite(S1,LOW); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  sensor1 = analogRead(SIG);
  delay(100);
  
  // Channel 2 (C2 pin - binary output 0,1,0,0)
  digitalWrite(S0,LOW); digitalWrite(S1,HIGH); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  sensor2 = analogRead(SIG);
  delay(100); 
   
  // Channel 3 (C3 pin - binary output 1,1,0,0)
  digitalWrite(S0,HIGH); digitalWrite(S1,HIGH); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
  sensor3 = analogRead(SIG);
  delay(100);

  // Channel 4 (C4 pin - binary output 0,0,1,0)
  digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,HIGH); digitalWrite(S3,LOW);
  sensor4 = analogRead(SIG);
  delay(100);

  // Channel 5 (C5 pin - binary output 1,0,1,0)
  digitalWrite(S0,HIGH); digitalWrite(S1,LOW); digitalWrite(S2,HIGH); digitalWrite(S3,LOW);
  sensor5 = analogRead(SIG);
  delay(100);
 

  // To measure voltage/current we need to know the frequency of voltage/current
  // By default 50Hz is used, but you can specify desired frequency
  // as first argument to getVoltageAC and getCurrentAC() method, if necessary
  
  int V = voltageSensor.getVoltageAC();
  delay(100);
  int I = currentSensor.getCurrentAC();
  //float I = ACS.mA_AC();
  delay(100);

//  Serial.print("Arus: ");
//  Serial.println(I);
  // To calculate the power we need voltage multiplied by current
  int W = V * I;
  delay(50);
  
  Firebase.setInt("arus", I);
  delay(100);
  Firebase.setInt("tegangan", V); 
  delay(100);
  Firebase.setInt("daya", W);
  delay(100);
  Firebase.setInt("ldr1", sensor2);
  delay(100);
  Firebase.setInt("ldr2", sensor3);
  delay(100);
  Firebase.setInt("ldr3", sensor4);
  delay(100);
  Firebase.setInt("ldr4", sensor5);
  delay(100);
}
