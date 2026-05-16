#include <iostream>
#include <pcap.h>
#include <cstdlib>

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    std::cout << "[Analysis Engine] Captured packet of length: " << pkthdr->len << std::endl;
    // TODO: Extract IP header, ports, and metadata to feed into the MLFQ scheduler
}

int main() {
    std::cout << "Starting AI Threat Analysis Engine (C++)..." << std::endl;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    const char* dev = "pflog0"; // macOS pf logging interface
    
    // Open the device for capturing
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    
    if (handle == nullptr) {
        std::cerr << "Couldn't open device " << dev << ": " << errbuf << std::endl;
        std::cerr << "Make sure you are running with sudo privileges and pflog is active." << std::endl;
        return 2;
    }
    
    std::cout << "Successfully listening on " << dev << " for ambiguous traffic..." << std::endl;
    
    // Start the capture loop (loops indefinitely for now)
    pcap_loop(handle, 0, packetHandler, nullptr);
    
    pcap_close(handle);
    return 0;
}
