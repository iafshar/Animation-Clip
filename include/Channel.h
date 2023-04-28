#include "Keyframe.h"
#include "core.h"
#include <vector>
#include <iostream>
using namespace std;

class Channel
{
public:
    std::vector<Keyframe> keyframes;
    char exIn; //constant: f, linear: l, cyclic: c, cyclic offset: o, bounce: b
    char exOut;
    float evaluate(float time);
    //bool load(const char *filename); //lec 06 pg 32 says FILE*
    void precompute(); //calls computeTan and computeCubics for all keyframes
    void computeTans();
    void computeCubics();
};
