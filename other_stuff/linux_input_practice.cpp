#include <linux/input.h> // input_event
#include <fcntl.h> // open
#include <string.h> // memset
#include <unistd.h> // sleep, read
#include <iostream> // cout

int main(int argc, char const *argv[])
{
    struct input_event ev;
    int fd, rd;

    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    }
    else
    {
        fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    }

    if (fd == -1)
    {
        std::cout << "Couldn't open device!" << std::endl;
        return -1;
    }

    while (1)
    {
        memset((void *)&ev, 0, sizeof(ev));

        rd = read(fd, (void *)&ev, sizeof(ev));

        if (rd <= 0)
        {
            std::cout << "rd: " << rd << std::endl;
            sleep(1);
        }

        if (rd > 0)
        {
            std::cout << "type:" << ev.type << " code:" << ev.code << " value:" << ev.value << " rd:" << rd << std::endl;
        }
    }

    return 0;
}
