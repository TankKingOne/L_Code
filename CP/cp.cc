#include <time.h>
#include "cp.hpp"

void *consumer(void *arg)
{
  sleep(1);
  BlockQueue *bq = (BlockQueue*)arg;
  int data;
  for(;;)
  {
    bq->PopData(data);
    cout << "consumer data: " << data << endl;
    //sleep(1);
  }
}
void *producter(void *arg)
{
  BlockQueue *bq = (BlockQueue*)arg;
  for(;;)
  {
    int data = rand() % 100 + 1;
    bq->PushData(data);
    cout << "producter data: " << data << endl;
    sleep(1);
  }
}

int main()
{
  srand((unsigned long)time(NULL));
  BlockQueue bq;
  
  pthread_t c, p;
  pthread_create(&c, NULL, consumer, (void*)&bq);
  pthread_create(&p, NULL, producter, (void*)&bq);

  pthread_join(c, NULL);
  pthread_join(p, NULL);

  return 0;
}
