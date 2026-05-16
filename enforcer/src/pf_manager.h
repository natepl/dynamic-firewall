#ifndef PF_MANAGER_H
#define PF_MANAGER_H

#include <string>
#include <vector>
#include <stdexcept>

class PfManager {
public:
    PfManager(const std::string& tableName);
    ~PfManager();

    // Add an IP to the dynamic blocklist table
    bool addIp(const std::string& ipAddress);

    // Remove an IP from the dynamic blocklist table
    bool removeIp(const std::string& ipAddress);

    // Get all current IPs in the table
    std::vector<std::string> getBlockedIps();

private:
    std::string table_name;

    // Helper to execute pfctl shell commands
    bool executePfctl(const std::string& command);
};

#endif // PF_MANAGER_H
