#include "../include/mqtt/client.h"
#include "../include/mqtt/message.h"
#include "../include/mqtt/connect_options.h"
#include "../include/mqtt/async_client.h"

#ifndef ROBOT_COMMUNICATION_H
#define ROBOT_COMMUNICATION_H

namespace robot {

    class communication {

        const std::string ADDRESS{"tcp://test.mosquitto.org:1883"};
        const std::string CLIENT_ID{"ev3dev-emil"};

        mqtt::topic enemy_detected;
        mqtt::topic identify_position;

        mqtt::async_client client;

    public:
        const std::string TOPIC_ENEMY_DETECTED{"ev3dev/robot/enemy-detected"};
        const std::string TOPIC_IDENTIFY_POSITION{"ev3dev/robot/identify-position"};

        communication(const mqtt::async_client::message_handler &message_callback);

        void send_enemy_detected_message();

        void send_identify_position_message();

        ~communication();
    };
}
#endif //ROBOT_COMMUNICATION_H
