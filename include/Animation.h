#include "Channel.h"
#include "Tokenizer.h"
#include <vector>
#include <iostream>
using namespace std;

/*
TODO:
1) Load from file // DONE
   figure out why both Animation and Channel have load - channel doesnt need maybe
   figure out whether *file should be passed to both or token
   figure out how to do tangents with fixed values
2) Do evaluate (extrapolate) // DONE
   What to do with Animation::evaluate
   figure out why pose is passed in to Animation::evaluate - each channel is 1D for each DOF so need to calc for all
   is pose.size() == channels.size()? Yes
3) Do Player::update (calculate delta time and why poses are there) Pose skeleton
4) Do command line stuff
*/

class Animation
{
public:
    float start;
    float end;
    std::vector<Channel> channels;
    void evaluate(float time, vector<float> &pose);
    bool load(const char *filename);
};
