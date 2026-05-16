#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include <string>
#include <vector>
#include <pcap.h>

struct PacketMetadata {
    std::string source_ip;
    std::string dest_ip;
    uint16_t source_port;
    uint16_t dest_port;
    std::string protocol;
    int payload_length;
    int priority; // Used for MLFQ
};

class PacketParser {
public:
    static bool parsePacket(const struct pcap_pkthdr* pkthdr, const u_char* packet, int datalink, PacketMetadata& meta);
};

#endif // PACKET_PARSER_H
