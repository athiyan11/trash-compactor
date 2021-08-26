#include <WiFi.h>
#include <PubSubClient.h>

/**************
   Define Constants
 **************/
namespace {
const char * WIFISSID = "Sadhir"; // Put your WifiSSID here
const char *  PASSWORD = "rajan@66"; // Put your wifi password here
const char * TOKEN = "BBFF-AzS0enoGUyl98GW5YdaQiXCDsIEW8W"; // Put your Ubidots' TOKEN
const char * MQTT_CLIENT_NAME = "RandomName"; // MQTT client Name, assign an unique 8-12 alphanumeric character ASCII string;
const char * VARIABLE_LABEL = "distance"; // Assign the variable label
const char * VARIABLE_LABEL2 = "fire";
const char * DEVICE_LABEL = "ESP-32"; // The MAC address of the device will be assigned as device label
const char * MQTT_BROKER = "industrial.api.ubidots.com";
const int trigPin = 5; // Triger pin of the HC-SR04
const int echoPin = 18; // Echo pin of the HC-SR04   // ADC1_0 GPIO36
const int fire = 19;
}

/* Sensor's declarations */
long duration;
int distance;
/* Space to store the request */
char payload[300];
char topic[300];
char topic2[300];
char str_sensor[10];
char str_sensor2[10];

/**************
   Auxiliar Functions
 **************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

/**************
   Sensor Functions
 **************/
int readDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = (pulseIn(echoPin, HIGH));
  distance = float(duration / 29 / 2);
  return distance;
}

/**************
   Main Functions
 **************/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   pinMode(fire, INPUT);
  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  /* call the funtion readDistance() */
  distance = readDistance();
   int firesensorDigital = digitalRead(fire);
  /* Building the Ubidots request */
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\": %d}", VARIABLE_LABEL, distance); // Adds the variable label
  sprintf(payload, "{\"%s\": %d}", VARIABLE_LABEL, distance);
  Serial.println(topic);
  /* Print the sensor reading to the Serial Monitor */
  Serial.println("Publishing values to Ubidots Cloud");
  Serial.print("Distance = ");
  Serial.println(distance);
  client.publish(topic, payload);


 sprintf(topic2, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\": %d}", VARIABLE_LABEL2,firesensorDigital ); // Adds the variable label
  sprintf(payload, "{\"%s\": %d}", VARIABLE_LABEL2, firesensorDigital);
  Serial.println(topic2);
client.publish(topic2, payload);
  /* Publish the request to Ubidots */
 
  client.loop();
  delay(1000);
}
