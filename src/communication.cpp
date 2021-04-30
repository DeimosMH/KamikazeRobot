#include <cstring>
#include "communication.h"

#define QOS 1

communication::communication(const std::string &address,
                             const std::string &client_id) :
        client(address, client_id) {

}

void communication::send() {

    mqtt::connect_options connect_options;
    connect_options.set_keep_alive_interval(20);
    connect_options.set_clean_session(true);
    client.connect(connect_options);

    std::cout << "Connected" << std::endl;

    auto message = mqtt::make_message(TOPIC, "PAYLOAD1");
    message->set_qos(QOS);
    client.publish(message);

    std::this_thread::sleep_for(std::chrono::seconds(20));


    client.disconnect();

}

void communication::recv() {

}

communication::~communication() {
}
