#pragma once

#include "heads.h"

class Timer {
public:
    Timer(int interval = 1000, int count = -1);
    ~Timer();

    void start(int interval, std::function<void()> callback, int count = -1 );
    void start(int interval);
    void stop();
    void exit();

    bool isActive() const {
        return m_running;
    }

private:
    void timerThread();

private:
    std::atomic<bool> m_running;
    std::thread m_thread;
    int m_interval;
    int m_count;
    std::function<void()> m_callback;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_exit;
};

