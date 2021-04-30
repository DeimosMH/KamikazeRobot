#include "../include/mqtt/client.h"
#include "../include/mqtt/message.h"
#include "../include/mqtt/connect_options.h"
#include "../include/mqtt/async_client.h"

#ifndef ROBOT_COMMUNICATION_H
#define ROBOT_COMMUNICATION_H

class communication {

    const std::string TOPIC{"ev3dev/robot"};

    mqtt::client client;


public:
    communication(const std::string &address,
                  const std::string &client_id);

    ~communication();

    void send();

    void recv();

};

#endif //ROBOT_COMMUNICATION_H
