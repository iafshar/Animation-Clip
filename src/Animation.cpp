#include "Animation.h"

bool Animation::load(const char *file) {
    Tokenizer token;

    token.Open(file);
    token.FindToken("range");
    start = token.GetFloat();
    end = token.GetFloat();

    token.FindToken("numchannels");
    int nChannels = token.GetInt();
    
    for (int i = 0; i < nChannels; i++)
    {
        token.FindToken("channel");
        Channel currChannel;
        token.SkipLine();
        token.FindToken("extrapolate");

        char in[256];
        
        token.GetToken(in); // exIn
        
        
        if (std::strcmp(in,"constant") == 0)
        {
            currChannel.exIn = 'f';
        }
        else if (std::strcmp(in,"linear") == 0)
        {
            currChannel.exIn = 'l';
        }
        else if (std::strcmp(in,"cycle") == 0)
        {
            currChannel.exIn = 'c';
        }
        else if (std::strcmp(in,"cycle_offset") == 0)
        {
            currChannel.exIn = 'o';
        }
        else if (std::strcmp(in,"bounce") == 0)
        {
            currChannel.exIn = 'b';
        }

        char out[256];
        token.GetToken(out); //exOut
        

        if (std::strcmp(out,"constant") == 0)
        {
            currChannel.exOut = 'f';
        }
        else if (std::strcmp(out,"linear") == 0)
        {
            currChannel.exOut = 'l';
        }
        else if (std::strcmp(out,"cycle") == 0)
        {
            currChannel.exOut = 'c';
        }
        else if (std::strcmp(out,"cycle_offset") == 0)
        {
            currChannel.exOut = 'o';
        }
        else if (std::strcmp(out,"bounce") == 0)
        {
            currChannel.exOut = 'b';
        }
        token.FindToken("keys");
        int nKeys = token.GetInt();
        token.SkipLine();
        
        for (int j = 0; j < nKeys; j++)
        {
            Keyframe currKey;
            currKey.time = token.GetFloat();
            currKey.value = token.GetFloat();

            char tIn[256];
            token.GetToken(tIn); // tangent in
            if (std::strcmp(tIn, "flat") == 0) 
            {
                currKey.ruleIn = 'f';
            }
            else if (std::strcmp(tIn, "linear") == 0)
            {
                currKey.ruleIn = 'l';
            }
            else if (std::strcmp(tIn, "smooth") == 0)
            {
                currKey.ruleIn = 's';
            }
            else //fixed
            {
                currKey.ruleIn = 'i';
                currKey.tanIn = atof(tIn);
            }
            
            char tOut[256];
            token.GetToken(tOut); //tangent out
            if (std::strcmp(tOut, "flat") == 0)
            {
                currKey.ruleOut = 'f';
            }
            else if (std::strcmp(tOut, "linear") == 0)
            {
                currKey.ruleOut = 'l';
            }
            else if (std::strcmp(tOut, "smooth") == 0)
            {
                currKey.ruleOut = 's';
            }
            else //fixed tangent value
            {
                currKey.ruleOut = 'i';
                currKey.tanOut = atof(tOut);
            }

            currChannel.keyframes.push_back(currKey);
            token.SkipLine(); // dont know if this messes things up or not
        }
        token.SkipLine();
        token.SkipLine();
        currChannel.precompute();
        channels.push_back(currChannel);
    }
    token.Close();
    return true;
} //maybe need to have Channel::load as well?

void Animation::evaluate(float time, vector<float> &pose) { //maybe use of pose is wrong
    vector<float> newPose;
    for (int i = 0; i < channels.size(); i++)
    {
        newPose.push_back(channels[i].evaluate(time));
        // if (channels[i].keyframes.size() == 1)
        // {
        //     cout << i << endl;
        // }
        
    }
    pose = newPose;
    
}