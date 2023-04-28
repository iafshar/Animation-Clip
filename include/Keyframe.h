using namespace std;

class Keyframe
{
public:
    float time;
    float value;
    float tanIn;
    float tanOut;
    char ruleIn = 'z'; // flat: f, linear: l, smooth: s
    char ruleOut;
    float a;
    float b;
    float c;
    float d;
};

