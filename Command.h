#ifndef LINUXSOCKET_COMMAND_H
#define LINUXSOCKET_COMMAND_H

#include <string>
#include <map>
#include "ClientSocket.h"

enum CmdTable {
    HELP = 0,
    QUIT, BROWSE, UPLOAD, DOWNLOAD, DELETE, NEW,
};

class Command {
private:
    std::map<std::string, int> cmd_map_;
    std::string cmd_;

    void Browse(ClientSocket &, const std::string);
    void Upload(ClientSocket &, const std::string);
    void Download(ClientSocket &, const std::string);
    void Delete(ClientSocket &, const std::string);
    void New(ClientSocket &, const std::string);

public:
    Command() {}
    Command(const std::string cmd) : cmd_(cmd) {}
    ~Command() {}

    void init();

    std::string cmd() const { return cmd_; }
    void set_cmd(const std::string cmd) { cmd_ = cmd; }

    bool CheckCmd() const { return cmd_map_.find(cmd_) != cmd_map_.end(); }
    bool CheckQuit() { return cmd_map_[cmd_] == QUIT; }

    void run(ClientSocket &, const std::string);
};

#endif //LINUXSOCKET_COMMAND_H
