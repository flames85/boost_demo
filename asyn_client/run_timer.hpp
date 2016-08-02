//
// Created by Shao.Admin on 16/7/28.
//
template <class T>
run_timer<T>::run_timer(shared_io s_io):s_io_(s_io),timer_(NULL)
{

}
template <class T>
void run_timer<T>::timeout(const error_code &err, deadline_timer *timer)
{
    if (err) {
        std::cout << "timer canceled: " << err.message() << std::endl;

    } else {
        // 回调函数
        callback_(s_obj_);
        // 继续运行定时器
        timer->expires_at(timer->expires_at() + boost::posix_time::seconds(timer_interval_));
        timer->async_wait(boost::bind(&run_timer<T>::timeout, this, _1, timer_));
    }
}
template <class T>
void run_timer<T>::timer_schedule(long timer_interval, void *(*callback)(boost::shared_ptr<T>), boost::shared_ptr<T> s_obj) {

    timer_interval_ = timer_interval;
    callback_ = callback;
    s_obj_ = s_obj;

    if(!timer_) {
        timer_  = new deadline_timer(*s_io_, boost::posix_time::seconds(timer_interval_));
        timer_->async_wait(boost::bind(&run_timer<T>::timeout, this, _1, timer_));
    }
}

template <class T>
void run_timer<T>::cancel_timer() {
    timer_->cancel();
    timer_->async_wait(boost::bind(&run_timer<T>::timeout, this, _1, timer_));
}