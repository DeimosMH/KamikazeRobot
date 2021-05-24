#include "communication.h"

robot::communication::communication(const mqtt::async_client::message_handler &message_callback,
                                    int i) :
        client{ADDRESS, CLIENT_ID, mqtt::create_options(5)},
        enemy_detected{client, "ev3dev/robot/enemy-detected", i},
        identify_position{client, "ev3dev/robot/identify-position", i},
        respond_position{client, "ev3dev/robot/respond-position", i} {
    mqtt::connect_options connect_options;
    connect_options.set_keep_alive_interval(20);
    connect_options.set_clean_session(false);
    connect_options.set_mqtt_version(5);
    client.connect(connect_options)->wait();

    /* Dont send back messages to ourselves */
    mqtt::subscribe_options subscribe_options{true};

    identify_position.subscribe(subscribe_options);
    enemy_detected.subscribe(subscribe_options);
    respond_position.subscribe(subscribe_options);

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

void robot::communication::send_respond_position_message(int x, int y) {
    auto a = std::to_string(x) + "," + std::to_string(y);
    respond_position.publish(a);
}
