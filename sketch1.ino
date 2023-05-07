#define FIREBASE_HOST "https://iot-sem-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyCY_yi0f9KqIuXJhHYrU67rmw-eik0ixl8"
#define WIFI_SSID "Harisa"
#define WIFI_PASSWORD "88888888"

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

FirebaseData firebaseData;

int status_prev = 1;
bool komanda = false;
int dioda = D5;
bool dioda1 = false;
int buzzer = D7;
int senzor = D1;

 void setup()  
 {   
   Serial.begin(9600);
   
   pinMode(dioda, OUTPUT);    
   pinMode(buzzer, OUTPUT);  
   pinMode(senzor, INPUT);   

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); //prints local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 }  

 void loop()   
 {  
  int statusPokreta = digitalRead(senzor);
  Serial.print("Vrijednost za detekciju pokreta: ");
  Serial.println(statusPokreta); 
  delay(2000);

  komanda = Firebase.getBool(firebaseData, "Pokret_/komanda");
  if(komanda == false)
  {
    noTone(buzzer); 
    delay(500);
  }

  if(statusPokreta == 1 )  
  {
    tone(buzzer,400);
    komanda=true;
    Firebase.setInt(firebaseData, "Pokret_/komanda", komanda);
    noTone(buzzer); 
    Serial.println("Pokret detektovan!");
    delay(50);
  }  
  else  
  { 
   noTone(buzzer); 
   Serial.println("Pokret nedetektovan!");
   delay(50);
  }

   if(status_prev != statusPokreta)
   {
       status_prev = statusPokreta;
       Firebase.setInt(firebaseData, "Pokret_/senzor", statusPokreta);
   }

   if(Firebase.getBool(firebaseData, "Svjetlo_/dioda"))
   {
     if(firebaseData.dataType() == "boolean"){
       dioda1 = firebaseData.boolData();
       digitalWrite(dioda, dioda1);
     }       
   }
   else
   {
      digitalWrite(dioda, LOW);
   }  
   
}  