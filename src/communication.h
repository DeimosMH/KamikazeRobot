#include "../include/mqtt/client.h"
#include "../include/mqtt/message.h"
#include "../include/mqtt/connect_options.h"
#include "../include/mqtt/async_client.h"

#ifndef ROBOT_COMMUNICATION_H
#define ROBOT_COMMUNICATION_H

namespace robot {

    class communication {

        const std::string ADDRESS{"tcp://broker.emqx.io:1883"};
        /**
         * client id is a unique identifier for the specific device,
         * should be changed when using multiple robots
         */
        const std::string CLIENT_ID{"ev3dev-emil"};

        mqtt::async_client client;

        mqtt::topic enemy_detected;
        mqtt::topic identify_position;
        mqtt::topic respond_position;


    public:

        /**
         * topic for announcing that an enemy has been detected,
         * current coordinates are to be sent
         * TODO: define coordinate protocol
         */
        //std::string TOPIC_ENEMY_DETECTED{"ev3dev/robot/enemy-detected"};

        /**
         * topic for announcing that all other robots should respond with their position
         */
        //std::string TOPIC_IDENTIFY_POSITION{"ev3dev/robot/identify-position"};

        /**
         * topic for responding position, payload should be coordinates
         */
        // const std::string TOPIC_RESPOND_POSITION{"ev3dev/robot/respond-position"};

        /**
         *
         * @param message_callback callback for mqtt messages, messages for all topics
         * are returned in the same callback
         */
        communication(const mqtt::async_client::message_handler &message_callback, int i);

        void send_enemy_detected_message();

        void send_identify_position_message();


        void send_respond_position_message(int x, int y);

        ~communication();

    };
}
#endif //ROBOT_COMMUNICATION_H
