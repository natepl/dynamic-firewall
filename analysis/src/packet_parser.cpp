#include "packet_parser.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <iostream>

bool PacketParser::parsePacket(const struct pcap_pkthdr* pkthdr, const u_char* packet, int datalink, PacketMetadata& meta) {
    int offset = 0;

    // Handle different datalink types to find the IP header
    if (datalink == DLT_EN10MB) { // Ethernet
        offset = 14; // Ethernet header is 14 bytes
    } else if (datalink == 117) { // DLT_PFLOG
        // macOS pflog header is typically 64 bytes
        offset = 64; 
    } else {
        // Unsupported datalink
        return false;
    }

    if (pkthdr->caplen < offset + sizeof(struct ip)) {
        return false; // Packet too small
    }

    const struct ip* ipHeader = (struct ip*)(packet + offset);
    
    // Only parse IPv4 for now
    if (ipHeader->ip_v != 4) return false;

    char srcIpStr[INET_ADDRSTRLEN];
    char dstIpStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipHeader->ip_src), srcIpStr, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ipHeader->ip_dst), dstIpStr, INET_ADDRSTRLEN);

    meta.source_ip = srcIpStr;
    meta.dest_ip = dstIpStr;

    int ipHeaderLen = ipHeader->ip_hl * 4;
    
    if (ipHeader->ip_p == IPPROTO_TCP) {
        meta.protocol = "TCP";
        if (pkthdr->caplen >= offset + ipHeaderLen + sizeof(struct tcphdr)) {
            const struct tcphdr* tcpHeader = (struct tcphdr*)(packet + offset + ipHeaderLen);
            meta.source_port = ntohs(tcpHeader->th_sport);
            meta.dest_port = ntohs(tcpHeader->th_dport);
        }
    } else if (ipHeader->ip_p == IPPROTO_UDP) {
        meta.protocol = "UDP";
        if (pkthdr->caplen >= offset + ipHeaderLen + sizeof(struct udphdr)) {
            const struct udphdr* udpHeader = (struct udphdr*)(packet + offset + ipHeaderLen);
            meta.source_port = ntohs(udpHeader->uh_sport);
            meta.dest_port = ntohs(udpHeader->uh_dport);
        }
    } else {
        meta.protocol = "OTHER";
        meta.source_port = 0;
        meta.dest_port = 0;
    }

    meta.payload_length = ntohs(ipHeader->ip_len) - ipHeaderLen;
    
    // Assign a baseline priority for the MLFQ (0 is highest)
    // E.g., SSH or unexpected high ports get higher priority (lower number)
    if (meta.dest_port == 22 || meta.dest_port > 10000) {
        meta.priority = 0; 
    } else {
        meta.priority = 1;
    }

    return true;
}
