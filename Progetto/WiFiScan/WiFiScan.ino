/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>
#include <EEPROM.h>

String retiinfovett[10];
String ssidvett[10];
String ssidconnesso;
ESP8266WebServer server;
String scritta="FUNZIONA!";
String nomerete,pw;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
WiFiUDP UDP;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  Serial.println("Setup iniziato - v. 0.8");
  if(char(EEPROM.read(0x0F))=='1'){
    leggi();
  }
  
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //if(WiFi.status() != WL_CONNECTED){
  //  }
  WiFi.mode(WIFI_STA);
  delay(100);
  Serial.println("Setup concluso");
  
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
      //Serial.println("Controllo Connessione");
      ControllaConnessione();
    }else if(letto == "c"){ //DISCONNETTI SE CONNESSO
      Serial.println("Mi disconnetto");
      WiFi.disconnect();
    }else if(letto == "l"){ 
      leggi();
    }
  }
  if (WiFi.status() == WL_CONNECTED)
    {
      server.handleClient();
      int packetSize = UDP.parsePacket();
      if(packetSize)
        {
          UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
          Serial.println("Contents:");          
          Serial.println(packetBuffer); //first packet
          
          if(packetBuffer[0] == 'b'){ 
            Serial.println("INVIO RISPOSTA IN CORSO...");
            UDP.beginPacket("192.168.1.8", 11000);
            UDP.write("s");
            UDP.endPacket();
            delay(1000);
            Serial.println("Pacchetto UDP inviato");
          }  
        } 
        
    }       
    
}

void salva(String nome, String pw){
  for(int i=1;i<nome.length();i++)
  {
    EEPROM.write(0x0F+i, nome[i]); //Write one by one with starting address of 0x0F
  }
  EEPROM.commit();
  for(int j=nome.length()+1;j<pw.length();j++)
  {
    EEPROM.write(0x0F+j, pw[j]); //Write one by one with starting address of 0x0F
  }
  EEPROM.commit();
  EEPROM.write(0x0F, '1');
  EEPROM.write(0x0F+nome.length(), ';');
  EEPROM.write(0x0F+pw.length(), ';');
  EEPROM.commit();

  Serial.println("Credenziali salvate");
}

void leggi(){
  int i=1;
  bool sep=false;
    while(sep){
      nomerete = nomerete + char(EEPROM.read(0x0F+i));
      i++;
      if(char(EEPROM.read(0x0F+i))==';'){
        sep=false;
      }
    }
    Serial.print(nomerete);
    sep=false;
    while(sep){
      pw = pw + char(EEPROM.read(0x0F+i));
      i++;
      if(char(EEPROM.read(0x0F+i))==';'){
        sep=false;
      }
    }
    Serial.print(pw);
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
    salva(ssid,pw);
    ImpostaServer();
    
  }
  
  Serial.read();
  Serial.read();
  Serial.read();
}


void ImpostaServer(){
    server.on("/",handleIndex);
    server.on("/update",handleUpdate);
    server.begin(80);
    if(UDP.begin(82) == 1)
      {
        Serial.println("s");
      }
    else{
        Serial.println("n");
      }
}


void handleIndex(){
  server.send(200,"text/plain",scritta);
}

void handleUpdate(){
  scritta = server.arg("value");
  Serial.println(scritta);
  server.send(200,"text/plain","Aggiornato");
}


void ControllaConnessione(){
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("n");        
  }
  else{
    Serial.print("s");        
  }
}

//bool ESP8266WiFiSTAClass::isConnected() {
//    return (status() == WL_CONNECTED);
//}
