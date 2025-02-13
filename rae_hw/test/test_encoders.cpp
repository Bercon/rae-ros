#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>

#include "rae_hw/rae_motors.hpp"

int main(int argc, char* argv[]) {
    int encRatioL = 187;
    int encRatioR = 187;
    rae_hw::PID pid;
    if(argc == 2 && std::strcmp(argv[1], "-h") == 0) {
        std::cout << "Help:\n";
        std::cout << "Positional arguments only for now.\n";
        std::cout << "ros2 run rae_hw test_encoders encRatioL encRatioR.\n";
        std::cout << "With default arguments ros2 run rae_hw test_encoders 187 187" << std::endl;
        return 0;
    }
    if(argc > 1 && argc < 3) {
        std::cout << "Please input all arguments in following form: \n";
        std::cout << "ros2 run rae_hw test_encoders encRatioL encRatioR.\n";
        std::cout << "ros2 run rae_hw test_encoders 187 187" << std::endl;
    } else if(argc == 3) {
        encRatioL = atoi(argv[1]);
        encRatioR = atoi(argv[2]);
    }
    std::cout << "Starting test procedure.\n";
    std::cout << "Enc ratios - L: " << encRatioL << " R: " << encRatioR << " counts/rev." << std::endl;
    auto motorL = std::make_unique<rae_hw::RaeMotor>("left_wheel_name", "gpiochip0", "/sys/class/pwm/pwmchip0", 1, 41, 42, 43, encRatioL, 32, true, false, pid);
    auto motorR =
        std::make_unique<rae_hw::RaeMotor>("right_wheel_name", "gpiochip0", "/sys/class/pwm/pwmchip0", 2, 45, 46, 47, encRatioR, 32, false, false, pid);
    motorL->run();
    motorR->run();
    float prevPosL = 0.0;
    float prevPosR = 0.0;
    bool duration = true;
    while(duration) {
        auto leftPos = motorL->getPos();
        auto rightPos = motorR->getPos();
        if(leftPos != prevPosL || rightPos != prevPosR) {
            std::cout << "Left pos: " << leftPos << " rad.\n";
            std::cout << "Right pos: " << rightPos << " rad." << std::endl;
        }
        prevPosL = leftPos;
        prevPosR = rightPos;
    }

    return 0;
}
