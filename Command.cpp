#include "Command.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void Command::init() {
    cmd_map_["help"] = 0;
    cmd_map_["quit"] = 1;
    cmd_map_["browse"] = 2;
    cmd_map_["upload"] = 3;
    cmd_map_["download"] = 4;
    cmd_map_["delete"] = 5;
    cmd_map_["new"] = 6;
}

void Command::Browse(ClientSocket &socket, const std::string user) {
    std::string reply;
    try {
        socket << "browse?" + user;
        socket >> reply;
    } catch (SocketException &e) {
        // std::cout << "Exception was caught:" << e.description() << "\n";
    }
    std::cout  << reply << std::endl;
}

int GetFileSize(const char *path) {
    int filesize = -1;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0) {
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    return filesize;
}

void Command::Upload(ClientSocket &socket, const std::string user) {
    std::cout << "Input the file name in the client: ";
    std::string file_name;
    std::cin >> file_name;
    std::string file_path = "./" + file_name;
    int file_size = GetFileSize(file_path.c_str());

    if (file_size == -1) {
        std::cout << "Invalid file in client." << std::endl;
        return;
    }
    std::string reply;
    try {
        std::stringstream ss;
        std::string str_file_size;
        ss << file_size;
        ss >> str_file_size;
        FILE *fp;
        if ((fp = fopen(file_path.c_str(), "rb+")) == NULL) {
            std::cout << "Cannot open the file in client." << std::endl;
            return;
        }

        Byte *buf = (Byte *) malloc(file_size * sizeof(Byte));
        if (buf == NULL) {
            std::cout << "Cannot create buffer in client." << std::endl;
            return;
        }

        if (fread(buf, sizeof(Byte), file_size, fp) == 0) {
            std::cout << "Cannot read the file in client." << std::endl;
            return;
        }

        if (fclose(fp) != 0) {
            std::cout << "Cannot close the file in client." << std::endl;
            return;
        }

        socket << "upload?" + user + ":" + file_name + ":" + str_file_size;
        socket.SendFile(buf, (std::size_t) file_size);
        socket >> reply;

        free(buf);
        buf = NULL;
    } catch (SocketException &e) {
        // std::cout << "Exception was caught:" << e.description() << "\n";
    }
    std::cout  << reply << std::endl;
}

void Command::Download(ClientSocket &socket, const std::string user) {
    std::cout << "Input the file name in the server: ";
    std::string file_name;
    std::cin >> file_name;

    std::string reply;
    try {
        socket << "download?" + user + ":" + file_name;
        socket >> reply;

        if (reply[0] != ':') {
            std::cout << reply << std::endl;
            return;
        }
        std::string str_file_size = reply.substr(1, reply.size() - 1);

        int file_size = atoi(str_file_size.c_str());

        std::string file_path = "./" + file_name;
        FILE *fp;
        if ((fp = fopen(file_path.c_str(), "wb+")) == NULL) {
            std::cout << "Cannot open file in client." << std::endl;
            return;
        }
        Byte *buf = (Byte *) malloc(file_size * sizeof(Byte));
        if (buf == NULL) {
            std::cout << "Cannot create buffer in client." << std::endl;
            return;
        }
        socket.RecvFile(buf, (std::size_t) file_size);

        if (fwrite(buf, sizeof(Byte), file_size, fp) == 0) {
            free(buf);
            buf = NULL;
            fclose(fp);
            std::cout << "Cannot write the file in client." << std::endl;
            return;
        }
        if (fclose(fp) != 0) {
            free(buf);
            buf = NULL;
            std::cout << "Cannot close file in client." << std::endl;
            return;
        }
        socket >> reply;

        free(buf);
        buf = NULL;
    } catch (SocketException &e) {
        // std::cout << "Exception was caught:" << e.description() << "\n";
    }
    std::cout  << reply << std::endl;
}

void Command::Delete(ClientSocket &socket, const std::string user) {
    std::cout << "Input the file name in the server: ";
    std::string file_name;
    std::cin >> file_name;

    std::string reply;
    try {
        socket << "delete?" + user + ":" + file_name;
        socket >> reply;
    } catch (SocketException &e) {
        // std::cout << "Exception was caught:" << e.description() << "\n";
    }
    std::cout  << reply << std::endl;
}

void Command::New(ClientSocket &socket, const std::string user) {
    std::cout << "Input the file name in the server: ";
    std::string file_name;
    std::cin >> file_name;

    std::string reply;
    try {
        socket << "new?" + user + ":" + file_name;
        socket >> reply;
    } catch (SocketException &e) {
        // std::cout << "Exception was caught:" << e.description() << "\n";
    }
    std::cout  << reply << std::endl;
}

void Command::run(ClientSocket &socket, const std::string user) {
    switch (cmd_map_[cmd_]) {
        case HELP:
            std::cout << "Input \"browse\" to browse the server.\n";
            std::cout << "Input \"upload\" to upload your file to server.\n";
            std::cout << "Input \"download\" to download server file.\n";
            std::cout << "Input \"delete\" to delete server file.\n";
            std::cout << "Input \"new\" to new server file.\n";
            break;
        case QUIT:
            break;
        case BROWSE:
            Browse(socket, user);
            break;
        case UPLOAD:
            Upload(socket, user);
            break;
        case DOWNLOAD:
            Download(socket, user);
            break;
        case DELETE:
            Delete(socket, user);
            break;
        case NEW:
            New(socket, user);
            break;
    }
}