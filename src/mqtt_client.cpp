#include "mqtt_client.h"
#include <mosquittopp.h>
#include <sstream>
#include <string>
#include <string.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>

#define TOPIC_TO_SCALE "app_data"
#define TOPIC_FOR_SCALED_DATA "timeseries_data"
#define SCALAR_VALUE 1000

mqtt_client::mqtt_client(const char *id, std::string host, int port, int keepalive) : mosquittopp(id) {
    mosqpp::lib_init(); // Mandatory initialization for mosquitto library
    std::clog << "Connecting to MQTT broker" << std::endl;
    int status = connect(host.c_str(), port, keepalive); // non blocking connection to broker request
    //connect(host.c_str(), port, keepalive); // non blocking connection to broker request
    std::cout << "Connection Status " << status << std::endl;
    loop_start(); // Start thread managing connection / publish / subscribe
}

mqtt_client::~mqtt_client() {
    loop_stop(); // Kill the thread
    mosqpp::lib_cleanup(); // Mosquitto library cleanup
}

void mqtt_client::on_connect(int rc) {
    std::clog << "Successfully connected to MQTT broker" << std::endl;
    subscribe(NULL, TOPIC_TO_SCALE);
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos) {
    std::clog << "Successfully subscribed to: " << TOPIC_TO_SCALE << std::endl;
}

void mqtt_client::on_message(const struct mosquitto_message *message) {
    //std::clog << "Received message: " << (char*)message->payload << std::endl;
    try {
      nlohmann::json data = nlohmann::json::parse((char*)message->payload);
      double value = data["body"][0]["datapoints"][0][1];
      std::string tag_name = data["body"][0]["name"];
      //std::cout << "Extracted value " << *value << std::endl;
      value = value * SCALAR_VALUE;
      tag_name = tag_name + ".scaled_x_1000";
      data["body"][0]["name"] = tag_name;
      data["body"][0]["datapoints"][0][1] = value;
      //std::clog << "Publishing message: " << *tag_name << std::endl;
      //std::cout << "Scaled value to " << *value << std::endl;
      std::string serialized_data = data.dump();
      std::clog << "Publishing message: " << serialized_data << std::endl;
      publish(0, TOPIC_FOR_SCALED_DATA, serialized_data.length(), serialized_data.c_str(), 0, false);
    } catch (const std::exception& e) { // reference to the base of a polymorphic object
      std::cout << e.what(); // information from length_error printed
    }

}
