#include "pf_manager.h"
#include <iostream>
#include <cstdlib>

PfManager::PfManager(const std::string& tableName) : table_name(tableName) {
    // Optionally create the table on startup or assume pf.conf defines it
}

PfManager::~PfManager() {
}

bool PfManager::executePfctl(const std::string& command) {
    int result = std::system(command.c_str());
    return result == 0;
}

bool PfManager::addIp(const std::string& ipAddress) {
    std::string command = "pfctl -t " + table_name + " -T add " + ipAddress + " 2>/dev/null";
    if (executePfctl(command)) {
        std::cout << "[Enforcer] Successfully added " << ipAddress << " to table " << table_name << std::endl;
        return true;
    }
    std::cerr << "[Enforcer] Failed to add " << ipAddress << " to table " << table_name << std::endl;
    return false;
}

bool PfManager::removeIp(const std::string& ipAddress) {
    std::string command = "pfctl -t " + table_name + " -T delete " + ipAddress + " 2>/dev/null";
    if (executePfctl(command)) {
        std::cout << "[Enforcer] Successfully removed " << ipAddress << " from table " << table_name << std::endl;
        return true;
    }
    std::cerr << "[Enforcer] Failed to remove " << ipAddress << " from table " << table_name << std::endl;
    return false;
}

std::vector<std::string> PfManager::getBlockedIps() {
    // Requires executing `pfctl -t table_name -T show` and parsing output
    std::vector<std::string> ips;
    return ips;
}
