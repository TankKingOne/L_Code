#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATH_NAME "/tmp"
#define PRJ_ID 0x6666
#define SIZE 4096

int main()
{
  key_t k = ftok(PATH_NAME, PRJ_ID);
  if(k < 0)
  {
    printf("ftok error");
    return 1;
  }
  int shm_id = shmget(k, SIZE, IPC_CREAT | IPC_EXCL | 0644);
  if(shm_id < 0)
  {
    printf("shmget error!\n");
    return 2;
  }

  sleep(3);
  char *p = (char*)shmat(shm_id, NULL, 0);

  int i = 0;
  while(i < SIZE)
  {
    printf("%s\n", p);
    i++;
    sleep(1);
  }

  shmdt(p);
  shmctl(shm_id, IPC_RMID, NULL);
  return 0;
}
