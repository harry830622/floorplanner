#ifndef NET_HPP
#define NET_HPP

#include <vector>
#include "terminal.hpp"

using namespace std;

class Net
{
  public:
    Net(){};

    vector<Terminal*> terminals_;

    int HPWL();
};

#endif
