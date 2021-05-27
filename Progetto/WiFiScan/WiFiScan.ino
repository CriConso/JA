/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>
#include <EEPROM.h>
#include <dht11.h>
#define DHT11PIN 2
dht11 DHT11;
float temperatura=0;
float umidita=0;

IPAddress ipRemoto;
String ipRemotoS;
String retiinfovett[10];
String ssidvett[10];
String ssidconnesso;
ESP8266WebServer server;
String scritta="FUNZIONA!";
String nomerete,pw;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
WiFiUDP UDP;
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  Serial.println("Setup iniziato - v. 0.8");
  if(char(EEPROM.read(0) == '1')){
    leggi();
  }
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //if(WiFi.status() != WL_CONNECTED){
  //  }
  for(int i=0;i<10;i++){
    ssidvett[i]=" ";
  }
  //WiFi.mode(WIFI_STA);
  delay(100);
  Serial.println("Setup concluso");
  
}

void loop() {
  
  
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
    }else if(letto == "e"){ 
      cleareeprom();
    }else if(letto == "d"){ 
      Misura();
    }else if(letto == "s"){ 
      Provascrivi();
    }else if(letto == "q"){ 
      Provaleggi();
    }else if(letto == "i"){ 
      Serial.print(WiFi.localIP());
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
          
          ipRemoto = UDP.remoteIP();
          ipRemotoS = ipRemoto.toString();
          Serial.println(ipRemotoS);
          
          if(packetBuffer[0] == 'b'){ 
            Serial.println("INVIO RISPOSTA IN CORSO...");
            UDP.beginPacket(ipRemoto, 11000);
            UDP.write("s");
            UDP.endPacket();
            delay(1000);
            Serial.println("Pacchetto UDP inviato");
          }
          else if(packetBuffer[0] == 'd'){ 
            Serial.println("INVIO RISPOSTA IN CORSO...");
            UDP.beginPacket(ipRemoto, 11001);
            Misura();
//            uint8_t buffer[4];
//            ::memcpy(buffer, &temperatura, 4);
            String ST = String(temperatura);
            String SU = String(umidita);
            UDP.write(ST[0]);
            UDP.write(ST[1]);
            UDP.write(ST[2]);
            UDP.write(ST[3]);
            UDP.write(ST[4]);
            UDP.write(";");
            UDP.write(SU[0]);
            UDP.write(SU[1]);
            UDP.write(SU[2]);
            UDP.write(SU[3]);
            UDP.write(SU[4]);
            UDP.endPacket();
            delay(1000);
            Serial.println("Pacchetto UDP inviato");
          }  
        } 
        
    }       
    
}

void Provascrivi(){
  bool apw = true;
  String string = "";
  int add=0;
  while(apw){
    if(Serial.available()>0){ //MIO METODO D’AVAILABLE
      string = Serial.readStringUntil(';');
      
      apw=false;
    }
  }
  Serial.println("Hai scritto: " + string);

  for(int i=0;i<string.length();i++)
  {

    Serial.print(string.charAt(i));
    Serial.println(add);
    EEPROM.write(add, string.charAt(i)); //Write one by one with starting address of 0x0F
    add++;
  }
  EEPROM.write(add,';');
  EEPROM.commit();
  Provaleggi();
}

void Provaleggi(){
  int i=0;
  int add=0;
  bool sep=false;
  Serial.println(char(EEPROM.read(add)));
    while(!sep){
      nomerete = nomerete + char(EEPROM.read(add+i));
      //Serial.println("Valore letto: " + EEPROM.read(add+i));
      i++;
      if(char(EEPROM.read(add+i))==';'){
        sep=true;
        i++;
      }
    }
    Serial.print(nomerete);
    while(!sep){
      nomerete = nomerete + char(EEPROM.read(add+i));
      //Serial.println("Valore letto: " + EEPROM.read(add+i));
      i++;
      if(char(EEPROM.read(add+i))==';'){
        sep=true;
        i++;
      }
    }
    Serial.print(nomerete);
}

void cleareeprom(){
  for (int i = 0 ; i < 512 ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.println("Cancellato la memoria");
}

void salva(String nome, String pw){
  
  Serial.print("Nome: " + nome + "lunga: " + nome.length());
  Serial.println("\npw: " + pw + "lunga: " + pw.length());
  Serial.print("Scrivo ");
  Serial.println(nome.length());
  int add=0;
  
//  Serial.print("Scrivo " + nome.length());
//  Serial.println(" a " + add);
  add++;
  
  Serial.print("Scrivo ");
  Serial.print((char)nome.length());
  Serial.print(" a ");
  Serial.println(add);
  EEPROM.write(add, (char)nome.length());
  add++;
  EEPROM.write(add, (char)nome.length());
  Serial.print("Scrivo ");
  Serial.print((char)pw.length());
  Serial.print(" a ");
  Serial.println(add);
  EEPROM.write(add, (char)pw.length()); 
  add++;
  EEPROM.write(add, ';');
  add++;
  for(int i=0;i<nome.length();i++)
  {
    Serial.print("Scrivo ");
    Serial.print(nome[i]);
    Serial.print(" a ");
    Serial.println(add);
    EEPROM.write(add, nome[i]); 
    add++;
  }
  EEPROM.write(add, ';');
  add++;
  for(int j=0;j<pw.length();j++)
  {
    Serial.print("Scrivo ");
    Serial.print(pw[j]);
    Serial.print(" a ");
    Serial.println(add);
    EEPROM.write(add, pw[j]); //Write one by one with starting address of 0x0F
    add++;
  }
  EEPROM.write(0, '1');
  
  EEPROM.write(add, ';');
  EEPROM.commit();

  Serial.println("Credenziali salvate");
}

void leggi(){
  int i=1;
  int add=0;
  bool sep=false;
  String num;
  //Serial.println(char(EEPROM.read(add)));
  
    while(!sep){
      num = num + int(EEPROM.read(add+i)) + ", ";
      Serial.print("Valore letto: ");
      Serial.println(int(EEPROM.read(add+i)));
      i++;
      if(char(EEPROM.read(add+i))==';'){
        sep=true;
        i++;
      }
    }
    Serial.print("I numeri sono: ");
    Serial.println(num);
    sep=false;
    while(!sep){
      nomerete = nomerete + char(EEPROM.read(add+i));
      Serial.print("Valore letto: ");
      Serial.println(char(EEPROM.read(add+i)));
      i++;
      if(char(EEPROM.read(add+i))==';'){
        sep=true;
        i++;
      }
    }
    Serial.print("Il nome è: ");
    Serial.println(nomerete);
    sep=false;
    
    while(!sep){
      pw = pw + char(EEPROM.read(add+i));
      Serial.print("Valore letto: ");
      Serial.println(char(EEPROM.read(add+i)));
      i++;
      if(char(EEPROM.read(add+i))==';'){
        sep=true;
      }
    }
    Serial.print("La password è: ");
    Serial.println(pw);

    ConncPw(nomerete, pw);
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
      if(is=="a"){
       return; 
      }      
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
    Serial.print(WiFi.localIP());
    delay(500);
    salva(ssid,pw);
    ImpostaServer();
    
  }
  
  Serial.read();
  Serial.read();
  Serial.read();
}

void ConncPw(String nom, String pass){
  Serial.read();
  Serial.read();
  Serial.read();
  Serial.println("La password inserita è: " + pass);
  Serial.println("Rete d'area: " + nom);
  WiFi.begin(nom, pass);
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
    
    ssidconnesso = nom;
    Serial.print("s");
    delay(500);
    Serial.print(WiFi.localIP());
    delay(500);
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
  if (WiFi.status() != WL_CONNECTED){
    Serial.print("n");        
  }
  else{
    Serial.print("s");        
  }
}

//bool ESP8266WiFiSTAClass::isConnected() {
//    return (status() == WL_CONNECTED);
//}



void Misura(){

   uint8_t chk = DHT11.read(DHT11PIN);
   temperatura = DHT11.temperature, DEC;
   umidita = DHT11.humidity, DEC;
   Serial.println(temperatura);
   Serial.println(umidita);
//  Serial.print(" *C, ");
//  Serial.print((float)umidita); 
//  Serial.println(" RH%");
}
