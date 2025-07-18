#pragma once

#include "Epoll.h"
#include "IRequestHandler.h"

#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <atomic>
#include <random>

class RandomUtils {
public:
    static std::mt19937& rng() {
        static std::mt19937 instance(std::chrono::steady_clock::now().time_since_epoch().count());
        return instance;
    }

    static std::uniform_int_distribution<int>& sleepTimes() {
        static std::uniform_int_distribution<int> dist(10, 100);
        return dist;
    }
};

class WorkerThread {
public:
    WorkerThread(IRequestHandler *reqHandler) : handler(reqHandler), epollInstance(), client_cnt(0), running(false) {}
    
    void start() {
        running = true;
        thread_ = std::thread(&WorkerThread::threadLoop, this);
    }
    
    void join() {
        running = false;
        thread_.join();
    }

    /**
     * @brief get amount of clients being managed by this thread
     * 
     * @return int amount of client
     */
    int countClient() const { return client_cnt; }
    
    /**
     * @brief threadLoop to handle requests from client
     * 
     */
    void threadLoop();

    // control event with epoll
    void controlEvent(int ctrl_op, int fd, uint32_t events, void* data) { 
        epollInstance.controlEvent(ctrl_op, fd, events, data, client_cnt);
    }

    // handle each event distributed by listener thread
    void handleEpollEvent(EventData* data, uint32_t events);

    //
    void handleHttpData(const EventData &raw_request, EventData *raw_response);

private:
    IRequestHandler* handler;
    std::thread thread_;
    Epoll epollInstance;
    std::atomic<int> client_cnt;
    std::atomic<bool> running;
    epoll_event events[maxEvents];
};