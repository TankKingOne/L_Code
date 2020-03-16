#include <iostream>
#include <cstdio>
#include <mutex>

/** 饿汉 **/
/*
class singleton{
  public:
    static int _data; // 所有实例化的对象共用同一个资源
    int *get_instance(){
      return &_data;
    }
};

int singleton::_data = 100; // 程序初始化阶段完成资源的加载以及初始化
*/


/** 懒汉 **/
std::mutex lock;
class singleton{
  public:
    volatile static int *_data; // 所有实例化的对象共用同一个资源；防止编译器过度优化
    volatile int *get_instance(){
      if(_data == NULL){ // 二次判断，减少锁冲突
        lock.lock();
        if(_data == NULL){ // 判断与申请资源存在线程安全问题
         _data = new int;
        }
      }
      lock.unlock();
      return _data;
    }
};

volatile int *singleton::_data = nullptr;

int main()
{
  /*
  singleton a1, a2;
  printf("a1 : %d - %p\na2 : %d - %p\n", a1._data, a1.get_instance(), a2._data, a2.get_instance());
  a1._data = 10;
  a2._data = 20;

  printf("a1 : %d - %p\na2 : %d - %p\n", a1._data, a1.get_instance(), a2._data, a2.get_instance());
  */

  singleton a1, a2;
  printf("a1 : %d - %p\na2 : %d - %p\n", *a1._data, a1.get_instance(), *a2._data, a2.get_instance());
  *a1._data = 10;
  *a2._data = 20;

  printf("a1 : %d - %p\na2 : %d - %p\n", *a1._data, a1.get_instance(), *a2._data, a2.get_instance());

  return 0;
}
