#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sstream>
#include <set>

std::set<std::string> user_set;

bool CheckUser(const std::string name) {
    return user_set.find(name) != user_set.end();
}

std::string BrowseDir(const std::string dir) {
    struct dirent *entry;
    DIR *dp;
    if ((dp = opendir(dir.c_str())) == NULL) {
        return "open dir failed!";
    }

    std::string res = "";
    while ((entry = readdir(dp)) != NULL) {
        res += entry->d_name;
        res += "\t";
    }
    closedir(dp);
    return res;
}

std::string UploadFile(const std::string file_path, const Byte *buf, int file_size) {
    FILE *fp;
    if ((fp = fopen(file_path.c_str(), "wb+")) == NULL) {
        return "Cannot open the file in server.";
    }
    if (fwrite(buf, sizeof(*buf), file_size, fp) == 0) {
        fclose(fp);
        return "Cannot write the file in server.";
    }
    if (fclose(fp) != 0) {
        return "Cannot close the file in server.";
    }
    return "Upload successfully.";
}

std::string DownloadFile(const std::string file_path, Byte *buf, int file_size) {
    FILE *fp;
    if ((fp = fopen(file_path.c_str(), "rb+")) == NULL) {
        return "Cannot open the file in server.";
    }
    if (fread(buf, sizeof(Byte), (std::size_t) file_size, fp) == 0) {
        fclose(fp);
        return "Cannot read the file in server";
    }
    if (fclose(fp) != 0) {
        return "Cannot close the file in server.";
    }
    return "Download successfully.";
}

std::string DeleteFile(const std::string file_path) {
    if (remove(file_path.c_str()) == 0) {
        return "Delete successfully.";
    } else {
        return "Failed delete.";
    }
}

std::string NewFile(const std::string file_path) {
    FILE *fp;
    if ((fp = fopen(file_path.c_str(), "w")) == NULL) {
        return "Failed new.";
    } else {
        fclose(fp);
        return "New successfully.";
    }
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

void * read_msg(void *pSock) {
    ServerSocket *sock = (ServerSocket*) pSock;

    try {
        while(true) {
            std::string data;
            (*sock) >> data; // receive data
            std::size_t index = data.find("?");
            std::string cmd = data.substr(0, index);
            std::string value = data.substr(index + 1, data.size() - index - 1);
            std::cout << cmd << " " << value << std::endl;

            std::string res;
            if (data == "echo") {
                (*sock) << "Connect successfully.";
            } else if (cmd == "checkuser") {
                if (CheckUser(value)) (*sock) << "Login successfully.";
                else (*sock) << "Login failed.";
            } else if (cmd == "browse") {
                res = BrowseDir("./" + value + "/");
                (*sock) << res;
            } else if (cmd == "upload") {
                std::size_t index_first = value.find_first_of(":");
                std::size_t index_last = value.find_last_of(":");
                std::string user = value.substr(0, index_first);
                std::string file_name = value.substr(index_first + 1, index_last - index_first - 1);
                std::string str_file_size = value.substr(index_last + 1, value.size() - index_last - 1);

                int file_size = atoi(str_file_size.c_str());
                Byte *buf = (Byte *) malloc(file_size * sizeof(Byte));
                if (buf == NULL) {
                    (*sock) << "Cannot create buffer in server.";
                } else {
                    sock->RecvFile(buf, (std::size_t) file_size);
                    res = UploadFile("./" + user + "/" + file_name, buf, file_size);
                    (*sock) << res;
                    free(buf);
                    buf = NULL;
                }
            } else if (cmd == "download") {
                std::size_t index = value.find(":");
                std::string user = value.substr(0, index);
                std::string file_name = value.substr(index + 1, value.size() - index - 1);
                std::string file_path = "./" + user + "/" + file_name;

                int file_size = GetFileSize(file_path.c_str());
                if (file_size == -1) {
                    (*sock) << "Invalid file in server.";
                } else {
                    Byte *buf = (Byte *) malloc(file_size * sizeof(Byte));
                    if (buf == NULL) {
                        (*sock) << "Cannot create buffer in server.";
                    } else {
                        std::stringstream ss;
                        std::string str_file_size;
                        ss << file_size;
                        ss >> str_file_size;
                        (*sock) << ":" + str_file_size;
                        res = DownloadFile(file_path, buf, file_size);
                        sock->SendFile(buf, (std::size_t) file_size);
                        (*sock) << res;
                        free(buf);
                        buf = NULL;
                    }
                }
            } else if (cmd == "delete") {
                std::size_t index = value.find(":");
                std::string user = value.substr(0, index);
                std::string file_name = value.substr(index + 1, value.size() - index - 1);
                std::string file_path = "./" + user + "/" + file_name;
                res = DeleteFile(file_path);
                (*sock) << res;
            } else if (cmd == "new") {
                std::size_t index = value.find(":");
                std::string user = value.substr(0, index);
                std::string file_name = value.substr(index + 1, value.size() - index - 1);
                std::string file_path = "./" + user + "/" + file_name;
                res = NewFile(file_path);
                (*sock) << res;
            }
        }
    } catch (SocketException &e) { /* std::cout << "Exception was caught:" << e.description() << "\n"; */ }

    delete(sock);
    sock = NULL;
    pthread_exit(NULL);
    return NULL;
}

int main() {
    std::cout << "running..." << std::endl;
    user_set.insert("duan");
    user_set.insert("yu");

    try {
        pthread_t tid;
        ServerSocket server(30000);
        while (true) {
            ServerSocket *new_sock = new ServerSocket;
            server.accept(*new_sock);
            pthread_create(&tid, NULL, read_msg, (void *) new_sock);
            pthread_detach(tid);
        }
    } catch (SocketException &e) {
        std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

    return 0;
}
