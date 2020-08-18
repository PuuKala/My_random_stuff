#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <exception>
#include "json.hpp"

#define JSON_MAX_SIZE 4096

using namespace std;

int main(int argc, char const *argv[])
{
    int fd = open("test.json", O_RDWR | O_SYNC);
    char *json_txt = (char *)mmap(0, JSON_MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct flock lock = {
        .l_type = F_WRLCK,
        .l_whence = SEEK_SET,
        .l_start = 0,
        .l_len = 0};
    lock.l_pid = getpid();

    cout << "Waiting for lock..." << endl;
    if (fcntl(fd, F_SETLKW, &lock) < 0)
        throw runtime_error("Couldn't lock!");
    cout << "Lock obtained! Continuing..." << endl;

    nlohmann::json json = nlohmann::json::parse(json_txt);
    for (int i = 0; i < json.size(); i++)
    {
        cout << json[i]["something"] << endl;
    }

    nlohmann::json *json2 = &json;
    cout << json.dump() << endl;
    cout << json2->dump() << endl;
    json[0]["else"] = true;
    cout << json.dump() << endl;
    string dummy;
    cout << "Press anything to open lock and continue.";
    cin >> dummy;
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) < 0)
        throw runtime_error("Couldn't open lock!");

    cout << "Press anything to open lock and continue.";
    cin >> dummy;
    return 0;
}
