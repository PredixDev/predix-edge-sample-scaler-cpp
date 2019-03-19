#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <ctime>
#include <cstdio>
#include <string>
#include <mosquittopp.h>

class mqtt_client : public mosqpp::mosquittopp
{
	public:
		mqtt_client(const char *id, std::string host, int port, int keepalive);
		~mqtt_client();
		void on_connect(int rc);

	private:
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
		void on_message(const struct mosquitto_message *message);
};

#endif
