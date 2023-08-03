//
//   Simple Mqtt script for Esp32
//   Version 1.0
//   Coded by Vadim 'syjoosy' Nikolaev
//   03.08.2023


//
// MQTT / WIFI
//

#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "WiFi_SSID";
const char* password = "WiFi_PASSWORD";
const char* mqtt_server = "MQTT_BROKER_IP";
const int   mqtt_port = 1883;
const char *mqtt_username = ""; // If no needed, leave it empty
const char *mqtt_password = ""; // If no needed, leave it empty
const char *mqtt_client_id = "Esp32_SimpleMqtt";

WiFiClient espClient;
PubSubClient client(espClient);

#define TEMPERATURE_TOPIC "/sensor/temperature"
#define LED_TOPIC "/sensor/led"

int temperature = 22; // Variable we will publish
int mqttDelay = 3; // Delay to publish topic for mqtt in seconds

//
// FUNCTIONS
//

void setup() {
  Serial.begin(9600);
  
  wifiSetup();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(receivedCallback);
}

void wifiSetup()
{
  WiFi.begin(ssid, password);
  Serial.println("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received from topic: ");
  Serial.println(topic);

  Serial.print(" with payload: ");
  
  for (int i = 0; i < length; i++)
    Serial.print((char)payload[i] + " ");
  
  // Place your subscribe logic here
}

void loop() 
{
  mqttLogic();
}

long lastMsg = 0;
char msg[20];
void mqttLogic()
{
  if (!client.connected())
    mqttconnect();

  client.loop();

  if (millis() - lastMsg > mqttDelay * 1000) 
  {
    lastMsg = millis();

    // Place your publish logic here
    if (!isnan(temperature) && temperature != 0)
    {
      snprintf(msg, 5, "%f", temperature);
      client.publish(TEMPERATURE_TOPIC, msg);
    }
  }
}

void mqttconnect() 
{
  while (!client.connected()) 
  {
    String client_id = mqtt_client_id;

    bool mqqtConnectionStatus;
    if (strlen(mqtt_username) > 0)
      mqqtConnectionStatus = client.connect(client_id.c_str(), mqtt_username, mqtt_password);
    else
      mqqtConnectionStatus = client.connect(client_id.c_str());
    

    if (mqqtConnectionStatus)
    {
      client.subscribe(LED_TOPIC);
      Serial.println("Connection to mqtt broker successful!");
    } 
    else 
    {
      Serial.println("Failed to connect to mqtt broker! Status code: " + client.state());
      delay(5000);
    }
  }
}


//     ██▒   █▓ ▄▄▄     ▓█████▄   ██▓ ███▄ ▄███▓    ██▓   ██████ ▓██   ██▓ ▄▄▄██▀ ▒█████   ▒█████    ██████ ▓██   ██▓██▓      ███▄    █   ██▓ ██ ▄█▀ ▒█████    ██▓    ▄▄▄      ▓█████ ██▒   █▓
//    ▓██░   █▒▒████▄   ▒██▀ ██▌▒▓██▒▓██▒▀█▀ ██▒    ░█  ▒██    ▒  ▒██  ██▒   ▒██ ▒██▒  ██▒▒██▒  ██▒▒██    ▒  ▒██  ██▒░█       ██ ▀█   █ ▒▓██▒ ██▄█▒ ▒██▒  ██▒ ▓██▒   ▒████▄    ▓█   ▀▓██░   █▒
//     ▓██  █▒░▒██  ▀█▄ ░██   █▌▒▒██▒▓██    ▓██░     ▓  ░ ▓██▄     ▒██ ██░   ░██ ▒██░  ██▒▒██░  ██▒░ ▓██▄     ▒██ ██░ ▓      ▓██  ▀█ ██▒▒▒██▒▓███▄░ ▒██░  ██▒ ▒██░   ▒██  ▀█▄  ▒███   ▓██  █▒░
//      ▒██ █░░░██▄▄▄▄██░▓█▄   ▌░░██░▒██    ▒██      ▒    ▒   ██▒  ░ ▐██▓░▓██▄██▓▒██   ██░▒██   ██░  ▒   ██▒  ░ ▐██▓░ ▒      ▓██▒  ▐▌██▒░░██░▓██ █▄ ▒██   ██░ ▒██░   ░██▄▄▄▄██ ▒▓█  ▄  ▒██ █░░
//       ▒▀█░  ▒▓█   ▓██░▒████▓ ░░██░▒██▒   ░██▒     ░  ▒██████▒▒  ░ ██▒▓░ ▓███▒ ░ ████▓▒░░ ████▓▒░▒██████▒▒  ░ ██▒▓░ ░      ▒██░   ▓██░░░██░▒██▒ █▄░ ████▓▒░▒░██████▒▓█   ▓██▒░▒████   ▒▀█░  
//       ░ ▐░  ░▒▒   ▓▒█ ▒▒▓  ▒  ░▓  ░ ▒░   ░  ░     ░  ▒ ▒▓▒ ▒ ░   ██▒▒▒  ▒▓▒▒░ ░ ▒░▒░▒░ ░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░   ██▒▒▒  ░      ░ ▒░   ▒ ▒  ░▓  ▒ ▒▒ ▓▒░ ▒░▒░▒░ ░░ ▒░▓  ░▒▒   ▓▒█░░░ ▒░    ░ ▐░  
//       ░ ░░  ░ ░   ▒▒  ░ ▒  ▒ ░ ▒ ░░  ░      ░        ░ ░▒  ░ ░ ▓██ ░▒░  ▒ ░▒░   ░ ▒ ▒░   ░ ▒ ▒░ ░ ░▒  ░ ░ ▓██ ░▒░         ░ ░░   ░ ▒░░ ▒ ░░ ░▒ ▒░  ░ ▒ ▒░ ░░ ░ ▒  ░ ░   ▒▒ ░ ░ ░     ░ ░░  
//         ░░    ░   ▒   ░ ░  ░ ░ ▒ ░░      ░           ░  ░  ░   ▒ ▒ ░░   ░ ░ ░ ░ ░ ░ ▒  ░ ░ ░ ▒  ░  ░  ░   ▒ ▒ ░░             ░   ░ ░ ░ ▒ ░░ ░░ ░ ░ ░ ░ ▒     ░ ░    ░   ▒      ░        ░  
//          ░        ░     ░      ░         ░                 ░   ░ ░      ░   ░     ░ ░      ░ ░        ░   ░ ░                      ░   ░  ░  ░       ░ ░  ░    ░        ░  ░   ░        ░  

