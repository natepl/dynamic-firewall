#ifndef MLFQ_H
#define MLFQ_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "packet_parser.h"

class MlfqScheduler {
public:
    MlfqScheduler(int num_levels);
    ~MlfqScheduler();

    // Add a parsed packet metadata to the scheduler
    void enqueue(const PacketMetadata& meta);

    // Start the worker thread
    void start();

    // Stop the worker thread
    void stop();

private:
    int num_levels;
    std::vector<std::queue<PacketMetadata>> queues;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> running;
    std::thread worker;

    void workerLoop();
    void processEvent(const PacketMetadata& meta);
    
    // Stub to simulate sending to OpenAI API
    void sendToLlm(const PacketMetadata& meta);
};

#endif // MLFQ_H
