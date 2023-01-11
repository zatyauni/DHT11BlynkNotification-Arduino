#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME "Orchid Bliss"
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <FirebaseESP32.h>
#include <HTTPClient.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";  // type your wifi name
char pass[] = "";  // type your wifi password


#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""


#define DHTPIN 4          // Mention the digital pin where you connected 
#define DHTTYPE DHT11     // DHT 11  
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
FirebaseData fbdo;

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println(t);
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V5, t);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("Humidity : ");
    Serial.println(h);


  if(t > 27){
   //Blynk.email("72@gmail.com", "Alert", "Temperature above 30°C !!");
    Blynk.logEvent("temperature_high","Temperature above 26°C !!");
  }
  if(t < 16){
   //Blynk.email(".com", "Alert", "Temperature below 26°C !!");
    Blynk.logEvent("temperature_low","Temperature below 18°C !!");
    }

  if(h > 80){
   //Blynk.email("", "Alert", "Humidity above 80% !!");
    Blynk.logEvent("humidity_high","Humidity above 80% !!");
  }
  if(h < 60){
   //Blynk.email("", "Alert", "Humidity below 60% !!");
    Blynk.logEvent("humidity_low","Humidity below 60% !!");
  }
}

void setup(){
   Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(2500L, sendSensor);

   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop(){
  Blynk.run();
  timer.run();

  //read sensor temp and hum
  float h = dht.readHumidity();
  float t = dht.readTemperature();  

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("*C  ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%  ");

  //set value in fireb (temp)
  Firebase.setFloat( fbdo,"Temperature", t);
  //set value in fireb (hum)
  Firebase.setFloat ( fbdo,"Humidity", h);

  delay(200);
}
