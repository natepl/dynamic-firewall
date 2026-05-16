#include <iostream>
#include "pf_manager.h"

int main() {
    std::cout << "Starting Dynamic Firewall Enforcer Daemon (C++)..." << std::endl;

    try {
        // We will manage a pf table called "dynamic_blocklist"
        PfManager manager("dynamic_blocklist");

        // Example: Block a malicious IP
        std::string maliciousIp = "192.168.1.100";
        std::cout << "Attempting to block IP: " << maliciousIp << std::endl;
        
        if (manager.addIp(maliciousIp)) {
            std::cout << "Successfully added rule to pf." << std::endl;
        }

        // TODO: Initialize WebSocket client (websocketpp) here to listen to Supabase Realtime
        // When a new row is added to 'active_blocks', call manager.addIp(row.ip)
        // When a row is removed, call manager.removeIp(row.ip)

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::cerr << "Ensure you are running this executable with sudo privileges." << std::endl;
        return 1;
    }

    return 0;
}
