
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <random>
#include "Random.h"

using namespace DrageEngine;

void Random::SetSeed()
{
    srand((unsigned)time(NULL));
}

void Random::SetSeed(unsigned seed)
{
    srand(seed);
}

float Random::Float(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

int Random::Int(int min, int max)
{
    return rand() % (max - min) + min;
}

bool Random::Bool()
{
    return rand() % 2 == 0;
}

Quaternion Random::Rotation()
{
    return Quaternion::FromEuler(Float(0, 360), Float(0, 360), Float(0, 360));
}

std::string Random::UUID()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;
    
    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    
    return ss.str();
}
