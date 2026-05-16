#ifndef PF_MANAGER_H
#define PF_MANAGER_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// Note: macOS requires specific network headers for pf
#include <net/pfvar.h>

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
    int dev_fd;
    std::string table_name;

    // Helper to open /dev/pf
    void openDevice();
    // Helper to close /dev/pf
    void closeDevice();
};

#endif // PF_MANAGER_H
