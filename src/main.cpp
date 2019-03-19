#include <stdlib.h>
#include <map>
#include <vector>
#include <mosquittopp.h>
#include "mqtt_client.h"
#include <string>
#include <iostream>

#define MQTT_CLIENT_ID nullptr
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_KEEPALIVE 60

int main() {
  // prints hello world
  std::cout <<"Hello World\n";
  mosqpp::lib_init();
	mqtt_client* client = new mqtt_client(MQTT_CLIENT_ID, MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE);
	int rc;
	while (1) {
		rc = client->loop();
		if (rc) {
			client->reconnect();
		}
	}
	mosqpp::lib_cleanup();
	return 0;
}
