#ifndef __RING_HPP__ 
#define __RING_HPP__ 

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM 32

using namespace std;

class Ring{
  private:
    int circle[NUM];
    int cap;
    int c_step;
    int p_step;
    sem_t sem_data;
    sem_t sem_blank;
  private:
    void P(sem_t &s)
    {
      sem_wait(&s);
    }
    void V(sem_t &s)
    {
      sem_post(&s);
    }
  public:
    Ring():cap(NUM)
    {
      c_step = p_step = 0;
      sem_init(&sem_data, 0, 0);
      sem_init(&sem_blank, 0, NUM);
    }
    // C
    void GetData(int &out)
    {
      P(sem_data);
      out = circle[c_step];
      c_step++; 
      V(sem_blank);

      c_step %= cap;
    }
    // P
    void PutData(const int &in)
    {
      P(sem_blank);
      circle[p_step] = in;
      p_step++;
      V(sem_data);

      p_step %= cap;
    }
    ~Ring()
    {
      sem_destroy(&sem_data);
      sem_destroy(&sem_blank);
    }
};

#endif 
