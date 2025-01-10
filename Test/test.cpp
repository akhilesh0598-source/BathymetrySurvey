#include <iostream>
#include <csignal>
#include <cstring>

void sigHandler(int signal)
{
    std::cout << "Signal captured " << signal << std::endl;
    exit(0);
}

int main()
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));

    action.sa_handler = sigHandler;
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    std::cout << "Press Control+C to exit the program!" << std::endl;
    while (true)
    {
        std::cout << "Program is running!!" << std::endl;
    }

    return 0;
}