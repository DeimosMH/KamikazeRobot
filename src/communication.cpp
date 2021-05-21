#include "communication.h"

robot::communication::communication(const mqtt::async_client::message_handler &message_callback) :
        enemy_detected{client, TOPIC_ENEMY_DETECTED, 1},
        identify_position{client, TOPIC_IDENTIFY_POSITION, 1},
        client{ADDRESS, CLIENT_ID} {
    mqtt::connect_options connect_options;
    connect_options.set_keep_alive_interval(20);
    connect_options.set_clean_session(false);
    client.connect(connect_options)->wait();

    identify_position.subscribe();
    enemy_detected.subscribe();

    client.set_message_callback(message_callback);
}

void robot::communication::send_enemy_detected_message() {
    enemy_detected.publish("10,20");
}

robot::communication::~communication() {
    client.disconnect();
}

void robot::communication::send_identify_position_message() {
    identify_position.publish("identify your position!");
}
