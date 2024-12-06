#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>  // Librería para ThingSpeak

const char* ssid = "iCUCEI";

#define DHTPIN 4  
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

unsigned long myChannelNumber = 2776525;  // Tu ID de canal en ThingSpeak
const char* myWriteAPIKey = "9IK3PDEJNPDQD6LA";  // API Key de escritura de tu canal

WiFiClient  client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi!");
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  // Lee la temperatura y humedad
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifica si los valores son válidos
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error al leer del sensor DHT11");
    return;
  }

  // Enviar datos a ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);    

  // Enviar los datos cada 20 segundos
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if(x == 200) {
    Serial.println("Datos enviados correctamente");
  } else {
    Serial.println("Error al enviar los datos");
  }

  delay(20000);  // Espera 20 segundos antes de enviar los datos nuevamente
}