#include <sys/mman.h> // For everything shmem
#include <fcntl.h>    // For flag/mode definitions
#include <iostream>   // Text output
#include <unistd.h>   //
#include "json.hpp"
#include <signal.h>

#define TEST_BUF_SIZE 32768 // 32kb should be way more than enough, being safe on this
#define SHMEM_NAME "linux_shmem_test"

void catch_ctrl_c(int s)
{
    std::cout << std::endl
              << "Caught Ctrl-C, exiting..." << std::endl;
}

int main(int argc, char const *argv[])
{
    // Open shmem file
    int shmem_fd = shm_open(SHMEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
    if (shmem_fd == -1)
        throw std::runtime_error("SHM open fail!");

    if (ftruncate(shmem_fd, TEST_BUF_SIZE) == -1)
        throw std::runtime_error("File alloc fail!");

    char *json_text = (char *)mmap(0, TEST_BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);

    nlohmann::json json_obj = {
        {"lel", 2},
        {"lol", 11},
        {"lul", {{"lil", 22}, {"lal", 53}}}};
    std::cout << std::endl
              << "Json object:" << std::endl
              << json_obj.dump() << std::endl;
    
    for (auto& el : json_obj["lul"].items())
    {
        std::cout << el.key() << ":" << el.value() << ", ";
        json_obj["lul"][el.key()] = 1;
    }
    std::cout << std::endl;
    
    
    if (*json_text == '{')
    {
        nlohmann::json json_parsed = nlohmann::json::parse(json_text);
        std::cout << "Parsed object:" << std::endl
                  << json_parsed.dump() << std::endl;
    }

    std::cout << "SHMem insides:" << std::endl
              << json_text << std::endl;

    strcpy(json_text, json_obj.dump().c_str());
    std::cout << "SHMem written:" << std::endl
              << json_text << std::endl;

    struct sigaction ctrlcact;
    ctrlcact.sa_handler = catch_ctrl_c;
    sigemptyset(&ctrlcact.sa_mask);
    ctrlcact.sa_flags = 0;
    sigaction(SIGINT, &ctrlcact, NULL);
    std::cout << "Waiting for CTRL-C..." << std::endl;
    pause();

    std::cout << "Would you like to delete the shmem file?";
    std::string answer;
    std::cin >> answer;
    if ((answer != "y") && (answer != "yes"))
        return 0;

    std::cout << "Unlinking the shmem..." << std::endl;
    shm_unlink(SHMEM_NAME);

    return 0;
}
