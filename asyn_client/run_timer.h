//
// Created by Shao.Admin on 16/7/28.
//

#ifndef RUN_TIMER_H
#define RUN_TIMER_H

#include "boost_util.h"

template <class T>
class run_timer
{
public:
    run_timer(shared_io s_io);

    // 设置定时器
    void timer_schedule(long timer_interval, void *(*callback)(boost::shared_ptr<T>), boost::shared_ptr<T> s_obj);
    void cancel_timer();

private:
    //
    void timeout(const error_code &err, deadline_timer *timer);

private:

    shared_io               s_io_;
    deadline_timer          *timer_;
    long                    timer_interval_;

    // 回调函数指针
    void*                   (*callback_)(boost::shared_ptr<T>);
    // 回调对象
    boost::shared_ptr<T>    s_obj_;
};

#include "run_timer.hpp"

#endif //RUN_TIMER_H
