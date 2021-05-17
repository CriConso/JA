/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"

String retiinfovett[10];
String ssidvett[10];
String ssidconnesso;
void setup() {
  Serial.begin(115200);
  Serial.println("Setup starts - v. 0.7");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
//  if(WiFi.status() != WL_CONNECTED){
//    
//  }
  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  for(int i=0;i<10;i++){
    ssidvett[i]=" ";
  }
  if(Serial.available()>0){ //MIO METODO D’AVAILABLE
    String letto;
    letto = Serial.read();
    if(letto == "a"){ //SCANERIZZAZIONE DELLE RETI D’AREA
      Scannerizzazione();
    }else if(letto == "b"){ //CONTROLLO CONNESIONE
      Serial.println("Controllo Connessione");
      ControllaConnessione();
    }else if(letto == "c"){ //DISCONNETTI SE CONNESSO
      Serial.println("Mi disconnetto");
      WiFi.disconnect();
    }
  }
}

void Scannerizzazione(){
  Serial.println("Scansione d'area...");

  // WiFi.scanNetworks ritornerà il numero delle reti d’area trovate
  int n = WiFi.scanNetworks();
  Serial.println("f");
  if (n == 0) {
    Serial.println("Nessuna rete trovata");
  } 
  else{
    Serial.println(n);
    delay(100);
    //Serial.println(" reti trovate");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      ESP.wdtFeed();
      //Serial.print(i + 1);
      //Serial.print(": ");
      ssidvett[i] = WiFi.SSID(i);
      //retiinfovett[i] = WiFi.SSID(i);
      //Serial.print(" (");
      retiinfovett[i] += WiFi.RSSI(i);
      //Serial.print(")");
      retiinfovett[i] +=(WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " - Senza sicurezza" : " - Con sicurezza";
      Serial.println(ssidvett[i]);
      delay(500);
    }
    bool scegrete=false;
    String is;
    long j = -1;
    Serial.read();
    Serial.read();
    //Serial.println("Digitare il numero della rete d'area desiderata: ");
    int cont=0;
    while(!scegrete){  
      while(Serial.available()==0); //METODO D’AVAILABLE D’EMANUELE
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
            Serial.println("Connessione alla rete " + ssidvett[j] + "...");
            //WiFi.softAP(ssid, password);
            scegrete=true;
            Serial.print("Inserire la password:");
            Connetti(ssidvett[j]);
          }
        }
        
      Serial.read();
      Serial.read();
      Serial.read();
    }
        
        //IPAddress myIP = WiFi.softAPIP();
        delay(1000);
        
      
  }  
}
void Connetti(String ssid){
  Serial.read();
  Serial.read();
  Serial.read();
  
  bool apw = true;
  String pw = "";
  while(apw){
    if(Serial.available()>0){ //MIO METODO D’AVAILABLE
      pw = Serial.readStringUntil(';');
      
      apw=false;
    }
  }
  Serial.println("La password inserita è: " + pw);
  Serial.println("Rete d'area: " + ssid);
  WiFi.begin(ssid, pw);
  Serial.print("Connessione in corso");
  int k = 50;
  while (WiFi.status() != WL_CONNECTED&&k>0)
  {
    delay(500);
    k--;
    Serial.print(".");
  }
  Serial.println();
  delay(500);
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("n");
  }
  else{
    //Serial.print("Connected, IP address: ");
    
    ssidconnesso = ssid;
    Serial.print("s");
    delay(500);
    Serial.println(WiFi.localIP());
  }
  
  Serial.read();
  Serial.read();
  Serial.read();
}

void ControllaConnessione(){
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Il sensore non è connesso");        
  }
  else{
    Serial.print("Il sensore è connesso alla rete " + ssidconnesso);        
  }
}

//bool ESP8266WiFiSTAClass::isConnected() {
//    return (status() == WL_CONNECTED);
//}
