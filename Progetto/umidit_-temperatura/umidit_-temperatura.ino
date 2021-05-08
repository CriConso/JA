#include <DHT.h>
// dht11 è il tipo di sensore che uso
DHT dht(2, DHT11);
 
void setup() {
// imposto il tipo di display (colonne, righe)
dht.begin();
Serial.begin(9600);
Serial.print("Temperatura");
Serial.println("Umidità");

}
void loop() {
int t = dht.readTemperature();
int h = dht.readHumidity();
Serial.println(t + " - " + h);
delay(1000);
}
