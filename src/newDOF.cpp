#include "core.h"
#include "Tokenizer.h"
#include "Cube.h" 
#include "DOF.h"
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>

using namespace std;
//rotate may or may not use radians. Right now it's in radians

void DOF::setValue(float v) {
    if (v < min) {
        value = min;
    }
    else if (v > max) {
        value = max;
    }
    else {
        value = v;
    }
}
float DOF::getValue() {
    return value;
}
void DOF::setMin(float mi) {
    min = mi;
}
void DOF::setMax(float ma) {
    max = ma;
}
void DOF::update(float change) {
    value += change;
    if (value < min) {
        value = min;
    }
    else if (value > max) {
         value = max;
    }
}