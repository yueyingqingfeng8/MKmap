#include "timer.h"
#include <QDebug>

Timer::Timer(int interval, int count)
    : m_running(false), m_interval(interval), m_count(count),m_callback(NULL),m_exit(false)
{
    //创建线程后分离开,不需要接受线程结束信息
    m_thread = std::thread([this](){
        timerThread();
    });

    m_thread.detach();
}

Timer::~Timer()
{
    m_exit = true;
    m_condition.notify_all();
}

void Timer::start(int interval, std::function<void()> callback, int count)
{
    stop();
    if (!m_running)
    {
        m_interval = interval;
        m_count = count;
        m_callback = callback;
        m_running = true;
        m_condition.notify_all();
    }
}

void Timer::start(int interval)
{
    stop();
    if (!m_running)
    {
        m_interval = interval;
        m_running = true;
        m_condition.notify_all();
    }
}

void Timer::stop()
{
    if (m_running)
    {
        m_running = false;
        m_condition.notify_all();
    }
}

void Timer::timerThread()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    //运行时定时堵塞,不运行时不定时堵塞
    while (!m_exit)
    {
        if (m_running)
        {
            auto status = m_condition.wait_for(lock, std::chrono::milliseconds(m_interval));

            //超时,执行函数和计数变化
            if (status == std::cv_status::timeout)
            {
                m_running = false;

                if (m_callback != NULL)
                    m_callback();

                if (m_count > 0)
                    m_count--;

                if (m_count != 0)
                    m_running = true;
            }
            //被中断,计数归0
            else if (status == std::cv_status::no_timeout)
            {
                m_running = false;
                m_count = 0;
            }
        }
        else
        {
            //使用m_condition.wait_for(lock, std::chrono::hours::max());无法堵塞,所以循环等待1小时
            m_condition.wait_for(lock, std::chrono::hours(1));
        }
    }
}
