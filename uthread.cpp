/**
* @file  uthread.cpp
* @author chenxueyou
* @version 0.1
* @brief   :A asymmetric coroutine library for C++
* History
*      1. Date: 2014-12-12 
*          Author: chenxueyou
*          Modification: this file was created 
*/

#ifndef MY_UTHREAD_CPP
#define MY_UTHREAD_CPP


#include "uthread.h"
//#include <stdio.h>

void uthread_resume(schedule_t &schedule , int id)
{
    if(id < 0 || id >= schedule.threads.size()){
        return;
    }

    uthread_t *t = &(schedule.threads[id]);

    switch(t->state){
        case RUNNABLE:
            getcontext(&(t->ctx));
    
            t->ctx.uc_stack.ss_sp = t->stack;
            t->ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
            t->ctx.uc_stack.ss_flags = 0;
            t->ctx.uc_link = &(schedule.main);
            t->state = RUNNING;

            schedule.running_thread = id;

            makecontext(&(t->ctx),(void (*)(void))(uthread_body),1,&schedule);
            
            /* !! note : Here does not need to break */

        case SUSPEND:
            
            swapcontext(&(schedule.main),&(t->ctx));

            break;
        default: ;
    }
}

void uthread_yield(schedule_t &schedule)
{
    if(schedule.running_thread != -1 ){
        uthread_t *t = &(schedule.threads[schedule.running_thread]);
        t->state = SUSPEND;
        schedule.running_thread = -1;

        swapcontext(&(t->ctx),&(schedule.main));
    }
}

void uthread_body(schedule_t *ps)
{
    int id = ps->running_thread;

    if(id != -1){
        uthread_t *t = &(ps->threads[id]);

        t->func(t->arg);

        t->state = FREE;
        
        ps->running_thread = -1;
    }
}

int uthread_create(schedule_t &schedule,Fun func,void *arg)
{
    int id = 0;
    int threadnum = schedule.threads.size();
    
    for(id = 0; id < threadnum; ++id ){
        if(schedule.threads[id].state == FREE){
            break;
        }
    }
    
    if(id == threadnum){
        uthread_t thread;
        schedule.threads.push_back(thread);
    }

    uthread_t *t = &(schedule.threads[id]);

    t->state = RUNNABLE;
    t->func = func;
    t->arg = arg;

/*
//    这段代码以迁移到uthread_resume,初次使用在多个协程时会出现段错误
    getcontext(&(t->ctx));
    
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = DEFAULT_STACK_SZIE;
    t->ctx.uc_stack.ss_flags = 0;
    t->ctx.uc_link = &(schedule.main);

    makecontext(&(t->ctx),(void (*)(void))(uthread_body),1,&schedule);
*/    
    return id;
}

int schedule_finished(const schedule_t &schedule)
{
    if (schedule.running_thread != -1){
        return 0;
    }else{
        for(int i = 0; i < schedule.threads.size(); ++i){
            if(schedule.threads[i].state != FREE){
                return 0;
            }
        }
    }

    return 1;
}

#endif
