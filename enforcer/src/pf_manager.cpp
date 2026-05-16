#include "pf_manager.h"
#include <unistd.h>
#include <iostream>
#include <cstring>

PfManager::PfManager(const std::string& tableName) : dev_fd(-1), table_name(tableName) {
    openDevice();
}

PfManager::~PfManager() {
    closeDevice();
}

void PfManager::openDevice() {
    dev_fd = open("/dev/pf", O_RDWR);
    if (dev_fd < 0) {
        throw std::runtime_error("Failed to open /dev/pf. Enforcer daemon must run as root.");
    }
}

void PfManager::closeDevice() {
    if (dev_fd >= 0) {
        close(dev_fd);
        dev_fd = -1;
    }
}

bool PfManager::addIp(const std::string& ipAddress) {
    if (dev_fd < 0) return false;

    struct pfioc_table io_table;
    struct pfr_table table;
    struct pfr_addr addr;

    memset(&io_table, 0, sizeof(io_table));
    memset(&table, 0, sizeof(table));
    memset(&addr, 0, sizeof(addr));

    // Set up table name
    strncpy(table.pfrt_name, table_name.c_str(), PF_TABLE_NAME_SIZE - 1);
    
    // Set up IP address (assuming IPv4 for simplicity in prototype)
    addr.pfra_af = AF_INET;
    addr.pfra_net = 32;
    if (inet_pton(AF_INET, ipAddress.c_str(), &addr.pfra_ip4addr) != 1) {
        std::cerr << "Invalid IPv4 address format: " << ipAddress << std::endl;
        return false;
    }

    io_table.pfrio_table = table;
    io_table.pfrio_buffer = &addr;
    io_table.pfrio_esize = sizeof(addr);
    io_table.pfrio_size = 1;

    if (ioctl(dev_fd, DIOCRADDADDRS, &io_table) < 0) {
        perror("ioctl DIOCRADDADDRS failed");
        return false;
    }

    std::cout << "[Enforcer] Successfully added " << ipAddress << " to table " << table_name << std::endl;
    return true;
}

bool PfManager::removeIp(const std::string& ipAddress) {
    if (dev_fd < 0) return false;

    struct pfioc_table io_table;
    struct pfr_table table;
    struct pfr_addr addr;

    memset(&io_table, 0, sizeof(io_table));
    memset(&table, 0, sizeof(table));
    memset(&addr, 0, sizeof(addr));

    strncpy(table.pfrt_name, table_name.c_str(), PF_TABLE_NAME_SIZE - 1);
    
    addr.pfra_af = AF_INET;
    addr.pfra_net = 32;
    if (inet_pton(AF_INET, ipAddress.c_str(), &addr.pfra_ip4addr) != 1) {
        return false;
    }

    io_table.pfrio_table = table;
    io_table.pfrio_buffer = &addr;
    io_table.pfrio_esize = sizeof(addr);
    io_table.pfrio_size = 1;

    if (ioctl(dev_fd, DIOCRDELADDRS, &io_table) < 0) {
        perror("ioctl DIOCRDELADDRS failed");
        return false;
    }

    std::cout << "[Enforcer] Successfully removed " << ipAddress << " from table " << table_name << std::endl;
    return true;
}

std::vector<std::string> PfManager::getBlockedIps() {
    // Requires DIOCRGETADDRS, left empty for now
    std::vector<std::string> ips;
    return ips;
}
