#include <sstream>
#include "communication.h"

robot::communication::communication(const std::function<void(std::pair<int, int>)> &enemy_detected_callback,
                                    const std::function<void()> &identify_position_callback,
                                    const std::function<void(std::pair<int, int>)> &respond_position_callback) :
        enemy_detected_callback{enemy_detected_callback},
        identify_position_callback{identify_position_callback},
        respond_position_callback{respond_position_callback},
        client{ADDRESS, CLIENT_ID, mqtt::create_options(5)},
        enemy_detected{client, "ev3dev/robot/enemy-detected", 1},
        identify_position{client, "ev3dev/robot/identify-position", 1},
        respond_position{client, "ev3dev/robot/respond-position", 1} {

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

    client.set_message_callback([this](const mqtt::const_message_ptr &msg) {
        callback_handler(msg);
    });
}

std::vector<std::string> robot::communication::split(const std::string &string) {
    std::stringstream string_stream{string};
    std::vector<std::string> result;
    while (string_stream.good()) {
        std::string substr;
        getline(string_stream, substr, ',');
        result.push_back(substr);
    }
    return result;
}


void robot::communication::callback_handler(const mqtt::const_message_ptr &msg) {

    if (msg->get_topic() == "ev3dev/robot/enemy-detected") {

        std::string a = msg->get_payload();
        auto vec = split(a);
        auto target_position_x = std::stoi(vec[0]);
        auto target_position_y = std::stoi(vec[1]);

        std::cout << "x " << target_position_x << "y " << target_position_y << std::endl;

        std::pair<int, int> pair{target_position_x, target_position_y};

        enemy_detected_callback(pair);

    } else if (msg->get_topic() == "ev3dev/robot/identify-position") {

        identify_position_callback();

    } else if (msg->get_topic() == "ev3dev/robot/respond-position") {

        std::string a = msg->get_payload();
        auto vec = split(a);
        int x = std::stoi(vec[0]);
        int y = std::stoi(vec[1]);
        std::cout << "x " << x << "y " << y << std::endl;
        std::pair<int, int> location{x, y};
        respond_position_callback(location);
    }
}

void robot::communication::send_enemy_detected_message(int x, int y) {
    auto a = std::to_string(x) + "," + std::to_string(y);
    enemy_detected.publish(a);
}

void robot::communication::send_identify_position_message() {
    identify_position.publish("identify your position!");
}

void robot::communication::send_respond_position_message(int x, int y) {
    auto a = std::to_string(x) + "," + std::to_string(y);
    respond_position.publish(a);
}
