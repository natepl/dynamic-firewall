#include <iostream>
#include <pcap.h>
#include <cstdlib>
#include "packet_parser.h"
#include "mlfq.h"

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    MlfqScheduler* scheduler = reinterpret_cast<MlfqScheduler*>(userData);
    
    // We assume pcap user data also passes datalink info implicitly or we just assume DLT_PFLOG
    // For safety, let's hardcode DLT_PFLOG (117) since we are capturing on pflog0
    int datalink = 117; 
    
    PacketMetadata meta;
    if (PacketParser::parsePacket(pkthdr, packet, datalink, meta)) {
        scheduler->enqueue(meta);
    }
}

int main() {
    std::cout << "Starting AI Threat Analysis Engine (C++)..." << std::endl;
    
    // Initialize MLFQ with 3 priority levels
    MlfqScheduler scheduler(3);
    scheduler.start();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    const char* dev = "pflog0"; 
    
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    
    if (handle == nullptr) {
        std::cerr << "Couldn't open device " << dev << ": " << errbuf << std::endl;
        std::cerr << "Note: run with sudo." << std::endl;
        scheduler.stop();
        return 2;
    }
    
    std::cout << "Successfully listening on " << dev << "..." << std::endl;
    
    // Pass the scheduler instance as userData
    pcap_loop(handle, 0, packetHandler, reinterpret_cast<u_char*>(&scheduler));
    
    pcap_close(handle);
    scheduler.stop();
    return 0;
}
