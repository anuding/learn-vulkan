#pragma once
#include <sstream>
#include <random>
#include <string>
#include "Singleton.h"
#include "Common.h"

class KeyGen : public Singleton<KeyGen>
{
public:
    keytype key;
    keytype genKey();
};
