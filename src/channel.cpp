#include "Channel.h"

void Channel::computeTans() {
    for (int i = 0; i < keyframes.size(); i++)
    {
        if (keyframes[i].ruleIn == 'f')
        {
            keyframes[i].tanIn = 0;
        }
        else if (keyframes[i].ruleIn == 'l' && i != 0)
        {
            keyframes[i].tanIn = (keyframes[i].value - keyframes[i-1].value)/(keyframes[i].time - keyframes[i-1].time);
        }
        else if (keyframes[i].ruleIn == 's' && i != 0 && i != (keyframes.size()-1))
        {
            keyframes[i].tanIn = (keyframes[i+1].value - keyframes[i-1].value)/(keyframes[i+1].time - keyframes[i-1].time);
        }

        if (keyframes[i].ruleOut == 'f')
        {
            keyframes[i].tanOut = 0;
        }
        else if (keyframes[i].ruleOut == 'l' && i != keyframes.size()-1)
        {
            keyframes[i].tanOut = (keyframes[i+1].value - keyframes[i].value)/(keyframes[i+1].time - keyframes[i].time);
        }
        else if (keyframes[i].ruleOut == 's' && i != 0 && i != (keyframes.size()-1))
        {
            keyframes[i].tanOut = (keyframes[i+1].value - keyframes[i-1].value)/(keyframes[i+1].time - keyframes[i-1].time);
        }     
        
    }
    
    // if (keyframes[0].ruleIn == 'l')
    // {
    //     /* TODO */
    // }
    // else if (keyframes[0].ruleIn == 's')
    // {
    //     /* TODO */
    // }

    if (keyframes[0].ruleOut == 's')
    {
        keyframes[0].tanOut = (keyframes[1].value - keyframes[0].value)/(keyframes[1].time - keyframes[0].time);
    }
    
    if (keyframes[keyframes.size()-1].ruleIn == 's')
    {
        keyframes[keyframes.size()-1].tanIn = (keyframes[keyframes.size()-1].value - keyframes[keyframes.size()-2].value) / (keyframes[keyframes.size()-1].time - keyframes[keyframes.size()-2].time);
    }

    // if (keyframes[keyframes.size()-1].ruleOut == 'l')
    // {
    //     /* TODO */
    // }
    // else if (keyframes[keyframes.size()-1].ruleOut == 's')
    // {
    //     /* TODO */
    // }


    
}

void Channel::computeCubics() {
    //f(t0)  = a(t0)^3  + b(t0)^2 + c(t0) + d
    //f(t1)  = a(t1)^3  + b(t1)^2 + c(t1) + d
    //f'(t0) = a(3t0)^2 + b(2t0)  + c     + 0d 
    //f'(t1) = a(3t1)^2 + b(2t1)  + c     + 0d

    for (int i = 0; i < keyframes.size()-1; i++)
    {   
        float scale = keyframes[i+1].time - keyframes[i].time;
        glm::vec4 values = glm::vec4(keyframes[i].value, keyframes[i+1].value, scale * keyframes[i].tanOut, scale * keyframes[i+1].tanIn);
        //glm::vec4 values = glm::vec4(keyframes[i].value, keyframes[i+1].value, keyframes[i].tanOut, keyframes[i+1].tanIn);
        // glm::mat4 herm = glm::mat4(keyframes[i].time, keyframes[i+1].time, 3*keyframes[i].time, 3*keyframes[i+1].time,
        //                         keyframes[i].time, keyframes[i+1].time, 2*keyframes[i].time, 2*keyframes[i+1].time,
        //                         keyframes[i].time, keyframes[i+1].time,                   1,                     1,
        //                                         1,                   1,                   0,                     0);
        // herm = glm::mat4(0,1,0,3,
        //                             0,1,0,2,
        //                             0,1,1,1,
        //                             1,1,0,0);
        glm::mat4 herminv = glm::mat4(2,-3,0,1,
                                      -2,3,0,0,
                                      1,-2,1,0,
                                      1,-1,0,0);
        // glm::vec4 cubics = glm::inverse(herm) * values;
        glm::vec4 cubics = herminv * values;
        keyframes[i].a = cubics.x;
        keyframes[i].b = cubics.y;
        keyframes[i].c = cubics.z;
        keyframes[i].d = cubics.w;
    }
    
}

void Channel::precompute() {
    computeTans();
    computeCubics();
}

float Channel::evaluate(float time){
    Keyframe upper; 
    Keyframe lower;
    float value;
    for (int i = 0; i < keyframes.size(); i++)
    {
        //cout << keyframes[i].value << endl;
        if (time < keyframes[i].time)
        {
            upper = keyframes[i];
            if (i != 0)
            {
                lower = keyframes[i-1]; //span else ex bef
            }
            break;
        } 
        else if (time == keyframes[i].time)
        {
            return keyframes[i].value; //falls exactly on some key
        }
        
    }
    if (upper.ruleIn == 'z')
    {
        /* extrapolate after */
        if (exOut == 'f')
        {
            /* Flat */
            return keyframes[keyframes.size()-1].value;
        }
        else if (exOut == 'l')
        {
            /* Do Linear */
            float c = keyframes[keyframes.size()-1].value - (keyframes[keyframes.size()-1].tanIn * keyframes[keyframes.size()-1].time);
            return ((keyframes[keyframes.size()-1].tanIn * time) + c); // check
        }
        else if (exOut == 'c')
        {
            /* Do Cyclic */
            float newTime = time - (keyframes[keyframes.size()-1].time - keyframes[0].time);
            return evaluate(newTime);
        }
        else if (exOut == 'o')
        {
            /* Do Cyclic Offset */
            float newTime = time - (keyframes[keyframes.size()-1].time - keyframes[0].time);
            return evaluate(newTime) + (keyframes[keyframes.size()-1].value - keyframes[0].value);
        }
        else
        {
            /* Do Bounce */
            float newTime = keyframes[keyframes.size()-1].time - (time - keyframes[keyframes.size()-1].time);
            return evaluate(newTime);
        }
        
        
    }
    else if (upper.time == keyframes[0].time)
    {
        /* extrapolate before */
        if (exIn == 'f')
        {
            /* Flat */
            return keyframes[0].value;
        }
        else if (exIn == 'l')
        {
            /* Do Linear */
            float c = keyframes[0].value - (keyframes[0].tanOut * keyframes[0].time);
            return ((keyframes[0].tanOut * time) + c); // y = mx + c
        }
        else if (exIn == 'c')
        {
            /* Do Cyclic */
            float newTime = time + (keyframes[keyframes.size()-1].time - keyframes[0].time);
            return evaluate(newTime);
        }
        else if (exIn == 'o')
        {
            /* Do Cyclic Offset */
            float newTime = time + (keyframes[keyframes.size()-1].time - keyframes[0].time);
            return evaluate(newTime) - (keyframes[keyframes.size()-1].value - keyframes[0].value);
        }
        else
        {
            /* Do Bounce */
            float newTime = keyframes[0].time + (keyframes[0].time - time);
            return evaluate(newTime);
        }
    }

    if (lower.ruleIn != 'z') // if time is between two keys
    {
        value = (time - lower.time) / (upper.time - lower.time);
        float t3 = value * value * value;
        float t2 = value * value;
        glm::vec4 fir = glm::vec4(t3,t2,value,1);
        glm::vec4 sec = glm::vec4(lower.a, lower.b,lower.c,lower.d);
        //cout << lower.a << endl;
        float cubic = glm::dot(fir,sec);
        //float cubic = lower.d + value * (lower.c + value * (lower.b + value * (lower.a)));
        //cout << cubic << endl;
        return cubic;
    }
    //return 0.0;
    
}