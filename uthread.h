/**
* @file  thread.h
* @author chenxueyou
* @version 0.1
* @brief   :A asymmetric coroutine library for C++
* History
*      1. Date: 2014-12-12 
*          Author: chenxueyou
*          Modification: this file was created 
*/

#ifndef MY_UTHREAD_H
#define MY_UTHREAD_H

#include <ucontext.h>
#include <vector>

#define DEFAULT_STACK_SZIE (1024*128)

enum ThreadState{FREE,RUNNABLE,RUNNING,SUSPEND};

struct schedule_t;

typedef void (*Fun)(void *arg);

typedef struct uthread_t
{
    ucontext_t ctx;
    Fun func;
    void *arg;
    enum ThreadState state;
    char stack[DEFAULT_STACK_SZIE];
}uthread_t;

typedef std::vector<uthread_t> Thread_vector;

typedef struct schedule_t
{
    ucontext_t main;
    int running_thread;
    Thread_vector threads;

    schedule_t():running_thread(-1){}
}schedule_t;

/*help the thread running in the schedule*/
static void uthread_body(schedule_t *ps);

/*Create a user's thread
*    @param[in]:
*        schedule_t &schedule 
*        Fun func: user's function
*        void *arg: the arg of user's function
*    @param[out]:
*    @return:
*        return the index of the created thread in schedule
*/
int  uthread_create(schedule_t &schedule,Fun func,void *arg);

/* Hang the currently running thread, switch to main thread */
void uthread_yield(schedule_t &schedule);

/* resume the thread which index equal id*/
void uthread_resume(schedule_t &schedule,int id);

/*test whether all the threads in schedule run over
* @param[in]:
*    const schedule_t & schedule 
* @param[out]:
* @return:
*    return 1 if all threads run over,otherwise return 0
*/
int  schedule_finished(const schedule_t &schedule);

#endif
