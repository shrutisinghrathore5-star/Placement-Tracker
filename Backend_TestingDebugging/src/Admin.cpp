#include "Admin.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"
#include <iostream>

void Admin::displayInfo() const {
    std::cout << "Administrator: " << name << " (username: " << username << ")\n";
}

std::string Admin::serialize() const {
    std::vector<std::string> fields = {username, password, name};
    return Utils::join(fields, '|');
}

Admin Admin::deserialize(const std::string& line) {
    auto f = Utils::split(line, '|');
    if (f.size() < 3) throw FileIOException("Corrupt admin record: " + line);
    Admin a;
    a.username = f[0];
    a.password = f[1];
    a.name = f[2];
    return a;
}
