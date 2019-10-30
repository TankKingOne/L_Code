#include "ring.hpp"
#include <time.h>
#include <unistd.h>

void *consumer(void *arg)
{
  int data;
  Ring *r = (Ring*)arg;
  for(;;){
    sleep(1);
    r->GetData(data);
    cout << "consumer: " << data << endl;
  }
}

void *producter(void *arg)
{
  Ring *r = (Ring*)arg;
  for(;;){
    //sleep(1);
    int data = rand()%100 + 1;
    r->PutData(data);
    cout << "producter: " << data << endl;
  }
}


int main()
{
  srand((unsigned long)time(NULL));
  
  Ring r;
  pthread_t c, p;
  pthread_create(&c, NULL, consumer, (void*)&r);
  pthread_create(&p, NULL, producter, (void*)&r);

  pthread_join(c, NULL);
  pthread_join(p, NULL);

  return 0;
}
