#ifndef LINUXSOCKET_USER_H
#define LINUXSOCKET_USER_H

#include <string>

class User {
private:
    std::string user_name_;
public:
    User() {}
    User(std::string user_name) : user_name_(user_name) {}
    ~User() {}
    std::string user_name() const { return user_name_; }
    void set_user_name(const std::string user_name) {user_name_ = user_name; }

    bool CheckUser() const;
};

#endif //LINUXSOCKET_USER_H
