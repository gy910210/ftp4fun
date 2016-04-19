#include "ClientSocket.h"
#include "SocketException.h"
#include "Command.h"
#include <iostream>
#include <string>
#include <utility>
#include <stdlib.h>

bool ParseParam(const std::string param, std::pair<std::string, std::string> &pair) {
    std::size_t index = param.find("@");
    if (index == std::string::npos) {
        return false;
    }

    pair.first = param.substr(0, index);
    pair.second = param.substr(index + 1, param.size() - index - 1);

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Your parameter number is invalid." << std::endl;
        exit(0);
    }
    std::string param = argv[1];

    std::pair<std::string, std::string> param_pair;
    if (!ParseParam(param, param_pair)) {
        std::cerr << "Your parameter format is invalid." << std::endl;
        exit(0);
    }

    try {
        std::cout << param_pair.second << std::endl;
        ClientSocket client_socket(param_pair.second, 30000); // declare a ClientSocket
        std::string reply;
        try {
            client_socket << "echo";
            client_socket >> reply;
        } catch (SocketException &e) {
            // std::cout << "Exception was caught:" << e.description() << "\n";
        }
        std::cout  << reply << std::endl;

        try {
            client_socket << "checkuser?" + param_pair.first;
            client_socket >> reply;
        } catch (SocketException &e) {
            // std::cout << "Exception was caught:" << e.description() << "\n";
        }
        std::cout  << reply << std::endl;

        std::string cmd_str;
        Command cmd = Command();
        cmd.init();
        do {
            std::cout << "pftp >> ";
            std::cin >> cmd_str;
            cmd.set_cmd(cmd_str);
            if (!cmd.CheckCmd()) {
                std::cout << "Command \"" << cmd.cmd() << "\" is not found." << std::endl;
                continue;
            }
            cmd.run(client_socket, param_pair.first);
        } while (!cmd.CheckQuit());

    } catch (SocketException &e) {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }

    return 0;
}