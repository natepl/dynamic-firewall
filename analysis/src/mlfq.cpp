#include "mlfq.h"
#include <iostream>
#include <chrono>

MlfqScheduler::MlfqScheduler(int num_levels) : num_levels(num_levels), queues(num_levels), running(false) {
}

MlfqScheduler::~MlfqScheduler() {
    stop();
}

void MlfqScheduler::start() {
    running = true;
    worker = std::thread(&MlfqScheduler::workerLoop, this);
    std::cout << "[MLFQ] Scheduler started with " << num_levels << " priority levels." << std::endl;
}

void MlfqScheduler::stop() {
    if (running) {
        running = false;
        cv.notify_all();
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void MlfqScheduler::enqueue(const PacketMetadata& meta) {
    std::lock_guard<std::mutex> lock(mtx);
    int level = meta.priority;
    if (level < 0) level = 0;
    if (level >= num_levels) level = num_levels - 1;
    
    queues[level].push(meta);
    cv.notify_one();
}

void MlfqScheduler::workerLoop() {
    while (running) {
        PacketMetadata meta;
        bool found = false;
        
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ 
                if (!running) return true;
                for (const auto& q : queues) {
                    if (!q.empty()) return true;
                }
                return false;
            });

            if (!running) break;

            // Find the highest priority event (level 0 is highest)
            for (int i = 0; i < num_levels; ++i) {
                if (!queues[i].empty()) {
                    meta = queues[i].front();
                    queues[i].pop();
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            processEvent(meta);
        }
    }
}

void MlfqScheduler::processEvent(const PacketMetadata& meta) {
    std::cout << "[MLFQ] Processing event P" << meta.priority 
              << ": " << meta.source_ip << ":" << meta.source_port 
              << " -> " << meta.dest_ip << ":" << meta.dest_port 
              << " [" << meta.protocol << "]" << std::endl;
              
    // If it's a high priority suspicious packet, send to LLM
    if (meta.priority == 0) {
        sendToLlm(meta);
    }
}

void MlfqScheduler::sendToLlm(const PacketMetadata& meta) {
    std::cout << "[LLM Pipeline] Sending metadata to OpenAI for behavioral analysis..." << std::endl;
    // TODO: libcurl call to OpenAI API here
    // Example: "Determine if traffic from {meta.source_ip} to port {meta.dest_port} is malicious."
}
