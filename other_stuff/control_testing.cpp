#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "gnuplot-iostream.h"

class control_testing
{
private:
    const float _kp;
    const float _ki;
    const float _kd;
    float _integ = 0;
    float _prev_diff = 0;

public:
    control_testing(const float p, const float i, const float d);
    ~control_testing();
    const float p_control(const float target, const float current);
    const float i_control(const float target, const float current);
    const float d_control(const float target, const float current);
    const float control(const float target, const float current);
};

control_testing::control_testing(const float p, const float i, const float d) : _kp(p), _ki(i), _kd(d)
{
}

control_testing::~control_testing()
{
}
const float control_testing::p_control(const float target, const float current)
{
    return (target - current) * _kp;
}
const float control_testing::i_control(const float target, const float current)
{
    const float diff = (target - current);
    _integ += diff * _ki;
    //std::cout << "ictrl " << _integ << std::endl;
    return _integ;
}
const float control_testing::d_control(const float target, const float current)
{
    const float diff = (target - current);
    const float ctrl = (diff - _prev_diff) * _kd;
    _prev_diff = diff;
    //std::cout << "dctrl " << ctrl << std::endl;
    return ctrl;
}
const float control_testing::control(const float target, const float current)
{
    float control = p_control(target, current);
    //std::cout << "pctrl " << control << std::endl;
    control += i_control(target, current);
    control += d_control(target, current);
    return control;
}

int main(int argc, char const *argv[])
{
    Gnuplot gp;

    control_testing ctrl(0.28, 0.000001, 0);

    float system_state[2] = {0, 0};
    float system_target = 100;
    float control = 0;
    unsigned int tic_count = 0;
    float max_min[2] = {0, 0};
    float max_d = 2;
    float prev_control = 0;
    unsigned int total_count = 0;
    std::vector<std::pair<unsigned int, float>> xy_points;

    for (unsigned char i = 0; i < 3;)
    {
        xy_points.push_back(std::make_pair(total_count, system_state[0]));
        control = ctrl.control(system_target, system_state[0]);
        /*
        if (control > 4)
            control = 4;
        else if (control < -4)
            control = -4;
        */
        if (control - prev_control > max_d)
            control = prev_control + max_d;
        else if (control - prev_control < -max_d)
            control = prev_control - max_d;
        prev_control = control;

        system_state[1] = system_state[0];
        //std::cout << "end ctrl " << control << std::endl;
        system_state[0] += control;
        ++tic_count;
        //std::cout << "state " << system_state[0] << std::endl;

        if (system_state[0] > max_min[0])
            max_min[0] = system_state[0];
        else if (system_state[0] < max_min[1])
            max_min[1] = system_state[0];

        if (std::abs(system_state[0] - system_target) < 0.1)
        {
            if (std::abs(system_state[0] - system_state[1]) < 0.1)
            {
                if (system_target == 100)
                    system_target = 0;
                else
                    system_target = 100;
                std::cout << "System target reached, new target " << system_target << std::endl
                          << "Tic count taken until target " << tic_count << std::endl
                          << "Maximum and minimum values: " << max_min[0] << ":" << max_min[1] << std::endl
                          << std::endl;
                tic_count = 0;
                ++i;
            }
        }
        ++total_count;
        // std::cout << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    gp << "plot" << gp.file1d(xy_points) << std::endl;

    return 0;
}
