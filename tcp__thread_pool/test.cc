#include <iostream>
#include <map>

int main()
{
  std::map<std::string, std::string> dict;
  dict.insert(std::pair<std::string, std::string>("apple", "PingGuo"));
  dict.insert(std::pair<std::string, std::string>("banana", "XiangJiao"));
  dict.insert(std::pair<std::string, std::string>("peach", "TaoZi"));
  dict.insert(std::pair<std::string, std::string>("lemon", "NingMeng"));

  for(auto iter = dict.begin(); iter != dict.end(); ++i)
  {
    std::cout << iter->first << " : " << iter->second << std::endl;
  }
  return 0;
}
