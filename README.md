uthread
=======

一个简单的C++用户级线程（协程）库


* 一个调度器可以拥有多个协程
* 通过uthread_create创建一个协程
* 通过uthread_resume运行或者恢复运行一个协程
* 通过uthread_yield挂起一个协程，并切换到主进程中
* 通过schedule_finished 判断调度器中的协程是否全部运行完毕
* 每个协程最多拥有128Kb的栈，增大栈空间需要修改源码的宏`DEFAULT_STACK_SIZE `，并重新编译
