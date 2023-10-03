#include "mouseoneway.h"

MouseOneWay::MouseOneWay(mkFun::CommandInfo *commandInfo) :
    m_commandInfo(commandInfo),m_replaceRightKey(false),
    m_moveDistance(mouseOneWay::MoveDistance),m_judgeDistance(mouseOneWay::JudgeDistance),
    m_intercept_time(mouseOneWay::InterceptTime)
{
    m_timer = new Timer();

    m_effectFun = std::bind(&MouseOneWay::mouseOneWayFun, this, std::placeholders::_1, std::placeholders::_2);

    readDistanceSet();
}

MouseOneWay::~MouseOneWay()
{
    delete m_timer;
}

void MouseOneWay::initOnewayFunInfo()
{
    m_pos.setPoint(Point(-1, -1));

    m_timer->stop();
}

int MouseOneWay::mouseOneWayFun(WPARAM mouseType, Point mousePos)
{
    //按键按下启动一个时间段
    if(mouseType == WM_RBUTTONDOWN)
    {
        if(!m_timer->isActive())
        {
            m_pos.setPoint(mousePos);
            m_timer->start(m_intercept_time);
        }

        return 1;
    }

    //按键松开表示时间段结束,判断是否需要执行特殊功能
    if(mouseType == WM_RBUTTONUP)
    {
        //右键被代替,执行特殊功能
        if(m_replaceRightKey)
        {
            m_replaceRightKey = false;
        }
        else
        {
            //右键未被替换时,计时器开启中就关掉,右键升起表示一个循环结束
            if(m_timer->isActive())
            {
                initOnewayFunInfo();
            }

            Hook::addMouseInfoEvent(MOUSEEVENTF_RIGHTDOWN, mousePos);
            Hook::addMouseInfoEvent(MOUSEEVENTF_RIGHTUP, mousePos);
        }

        return 1;
    }

    if(mouseType == WM_MOUSEMOVE)
    {
        if(m_pos.x == -1) return 0;

        //计时时间内判断移动方向
        if(m_timer->isActive())
        {
            //判断方向后插入指令列表
            insertCommandList(judgeDirection(mousePos));
        }
    }

    return 0;
}

std::string MouseOneWay::judgeDirection(Point& mousePos)
{
    if(pow(mousePos.y - m_pos.y, 2) + pow(mousePos.x - m_pos.x, 2) < m_moveDistance * m_moveDistance)
    {
        return "";
    }

    std::string  direction;
    if(mousePos.y < m_pos.y - m_judgeDistance)
    {
        //上面
        if(mousePos.x < m_pos.x - m_judgeDistance)       //左上
        {
            direction = "UL";
        }
        else if(mousePos.x > m_pos.x + m_judgeDistance)  //右上
        {
            direction = "UR";
        }
        else                                                    //上
        {
            direction = "U";
        }
    }
    else if(mousePos.y > m_pos.y + m_judgeDistance)
    {
        //下面
        if(mousePos.x < m_pos.x - m_judgeDistance)       //左下
        {
            direction = "DL";
        }
        else if(mousePos.x > m_pos.x + m_judgeDistance)  //右下
        {
            direction = "DR";
        }
        else                                                    //下
        {
            direction = "D";
        }
    }
    else if(mousePos.x < m_pos.x - m_judgeDistance)
    {
        direction = "L";                    //左
    }
    else
    {
        direction = "R";                    //右
    }

    m_replaceRightKey = true;
    initOnewayFunInfo();

    return direction;
}

void MouseOneWay::insertCommandList(std::string&& command)
{
    //检测到方向不为空,则加入指令列表,由于json的关键字为字符串,所以这里指令为字符串
    if(!command.empty())
    {
        //上锁
        std::unique_lock<std::mutex> lock(m_commandInfo->mutex);
        //增加命令信息
        m_commandInfo->commandList.push_back(std::tuple<int, mkFun::CommandData>(mkFun::MouseFunType::MOUSE_ONEWAY, command));
        // 释放互斥锁
        lock.unlock();
        //通知线程执行命令
        m_commandInfo->condition.notify_one();
    }
}

void MouseOneWay::readDistanceSet()
{
    //第一个是移动距离,第二个是判断距离
    auto list = OrderMap::searchSetting(MOW_DISTANCE);
    if (list.size() == 2)
    {
        m_moveDistance = list.front();
        m_judgeDistance = list.back();
    }
}

void MouseOneWay::setDistance(int move, int judge)
{
    if (move == -1 || judge == -1) return;

    m_moveDistance = move;
    m_judgeDistance = judge;
}

HOOK::MOUSE_EFFECT_FUN MouseOneWay::getEffectFun() const
{
    return m_effectFun;
}

