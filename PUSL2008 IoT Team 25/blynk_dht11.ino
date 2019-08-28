#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "a3b01df079304dbcbc8476b476ccc138";
#define W5100_CS  10
#define SDCARD_CS 4
int greenpin =9;
int bluepin =7;
int redpin =6;
#define pin 8
#define DHTPIN 5          // What digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}
void setup()
{
  // Debug console
  Serial.begin(9600);
pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT); 
pinMode(8,OUTPUT);
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  float q = analogRead(A3);                           //Read air quality and put it into q variable 
  Blynk.virtualWrite(V1, q);                          //Send q value to blynk in V0 virtual pin        
  if (q>=150 && q<200) {   
    setColor(255, 255, 0);  // yellow                                        
    Blynk.notify("The Quailty of air is Very Unhealthy !!");     //Blynk will send notify if q>200
  }
  else if(q>=100 && q<150){
   setColor(255, 0, 0);  // red
   Blynk.notify("The Quailty of air is Unhealthy !!");     //Blynk will send notify if q>200
    }
  else if(q>=50 && q<100){
   
    setColor(0, 0, 255);  // blue
    Blynk.notify("The Quailty of air is Moderate !!");     //Blynk will send notify if q>200
    }
  else if(q>=0 && q<50){
    setColor(0, 255, 0);  // green
    Blynk.notify("The Quailty of air is Good !!");     //Blynk will send notify if q>200
    }
  float t = dht.readTemperature();                    //Read temperature and put it in t variable 
  if (t>=35) {   
    digitalWrite(8,HIGH);                                           
  }
  else if(t<=35){
    digitalWrite(8,LOW);
    }
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);  
}
