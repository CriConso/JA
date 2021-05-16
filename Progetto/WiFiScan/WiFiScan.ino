/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"

String ssidvett[10];
void setup() {
  Serial.begin(115200);
  Serial.println("Setup starts - v. 0.2");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  for(int i=0;i<10;i++){
    ssidvett[i]=" ";
  }
  if(Serial.available()>0){
    if(Serial.read()=='a');{
      Scannerizzazione();
    }
  }
}

void Scannerizzazione(){
  Serial.println("Scansione d'area...");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scansione terminata.");
  if (n == 0) {
    Serial.println("Nessuna rete trovata");
  } 
  else{
    Serial.print(n);
    Serial.println(" reti trovate");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      ESP.wdtFeed();
      Serial.print(i + 1);
      Serial.print(": ");
      ssidvett[i] = WiFi.SSID(i);
      Serial.print(" (");
      ssidvett[i] += WiFi.RSSI(i);
      Serial.print(")");
      ssidvett[i] +=(WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " - Senza sicurezza" : " - Con sicurezza";
      Serial.println(ssidvett[i]);
      delay(500);
    }
    bool c=false;
    String is;
    long j = -1;
    Serial.read();
    Serial.read();
    Serial.println("Digitare il numero della rete desiderata: ");
    int cont=0;
    while(!c){
      //Serial.println("Tentativo numero " + cont);
      //cont++;   
      while(Serial.available()==0);
      is=Serial.read();
      j=is.toInt()-1;
      if((j>n)||((int)j<48||(int)j>57)){
       Serial.println("is: "+is);
          if((ssidvett[j]==" ")){
            Serial.print("La rete ");
            Serial.print((int)j); 
            Serial.println(" è inesistente");
          }
          else{
            Serial.println("Connessione alla rete " + ssidvett[j]+"...");
            //WiFi.softAP(ssid, password);
            c=true;
            Serial.print("AP IP address: ");
            Serial.println("Fatto");
          }
        }
      Serial.read();
      Serial.read();
      Serial.read();
    }
        

        //IPAddress myIP = WiFi.softAPIP();
        
        //Serial.println(myIP);
        delay(1000);
        
      
  }  
}

//bool ESP8266WiFiSTAClass::isConnected() {
//    return (status() == WL_CONNECTED);
//}
