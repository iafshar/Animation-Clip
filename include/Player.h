#include "Animation.h"
#include "../src/Skeleton.cpp"
using namespace std;

class Player
{
public:
    Animation* clip;
    Skeleton* skel;
    float time = 0.0;
    std::vector<float> pose;
    void update(float dTime);
    void SetClip(Animation* c) {clip = c;}
    std::vector<float>& GetCurrentPose() {return pose;}
};
