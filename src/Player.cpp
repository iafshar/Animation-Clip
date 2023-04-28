#include "Player.h"

void Player::update(float dTime) {
    if (time > clip->end)
    {
        time = clip->start;
    }
    else if (time < clip->start)
    {
        cout << "Time before Animation start" << endl;
        return;
    }
    
    clip->evaluate(time,GetCurrentPose()); //pose might not be updated properly (reference issue)
    vector<Joint> joints = skel->getJoints(skel->Root); // why does this not work? it just gets the current joint DOFs and sets them to that
    // if (time == 0.0)
    // {
        // for (int i = 0; i < joints.size(); i++)
        // {
        //     pose.push_back(joints[i].DOFs[0].getValue());
        //     pose.push_back(joints[i].DOFs[1].getValue());
        //     pose.push_back(joints[i].DOFs[2].getValue());
        // }
    // }
    
    
    
    int nJoints = skel->getNumJoints();
    Joint &tempJoint = skel->Root;

    // cout << tempJoint.children[1].children[0].DOFs[1].getValue() << endl;
    // cout << joints[3].DOFs[1].getValue() << endl;
    // cout << GetCurrentPose()[10] << endl;
    Channel test;
    Keyframe k1;
    k1.value = 10;
    k1.time = 2;
    k1.ruleOut = 'l';
    k1.ruleIn = 'l';
    Keyframe k2;
    k2.value = 28;
    k2.time = 4;
    k2.ruleIn = 'l';
    k2.ruleOut = 'l';
    Keyframe k3;
    k3.value = 30;
    k3.time = 12;
    k3.ruleIn = 'l';
    k3.ruleOut = 'l';
    test.keyframes.push_back(k1);
    test.keyframes.push_back(k2);
    test.keyframes.push_back(k3);
    test.precompute();
    test.exOut = 'f';
    test.exIn = 'f';
    // cout << test.keyframes[0].tanOut << endl;
    // cout << test.keyframes[1].tanIn << endl;
    //cout << test.evaluate(15) << endl;
    // cout << test.keyframes[0].a << endl;
    // cout << test.keyframes[0].b << endl;
    // cout << test.keyframes[0].c << endl;
    // cout << test.keyframes[0].d << endl;
    skel->Root.Offset = glm::vec3(GetCurrentPose()[0],GetCurrentPose()[1],GetCurrentPose()[2]);
    // cout << nJoints << endl;
    //cout << GetCurrentPose()[43] << endl; 
    //cout << joints[13].name << endl; //test extrap
    for (int i = 0; i < nJoints; i++)
    {
        // skel->increaseX(i,tempJoint);
        // skel->increaseY(i,tempJoint);
        // skel->increaseZ(i,tempJoint);

        // skel->setX(i,joints[i].DOFs[0].getValue(),tempJoint); 
        // skel->setY(i,joints[i].DOFs[1].getValue(),tempJoint); // sets to non transformed mesh
        // skel->setZ(i,joints[i].DOFs[2].getValue(),tempJoint);
        // cout << skel->getX(i,tempJoint) << endl;
        
        if (GetCurrentPose().size()-3 < 3*i+3)
        {
            break;
        }
        int j = i;
        skel->setX(j,GetCurrentPose()[3*i+3],tempJoint); 
        j = i;
        skel->setY(j,GetCurrentPose()[3*i+1+3],tempJoint);
        j = i;
        skel->setZ(j,GetCurrentPose()[3*i+2+3],tempJoint);

        
        // cout << GetCurrentPose()[3*i] << endl;
        // cout << GetCurrentPose()[3*i+1] << endl;
        // cout << GetCurrentPose()[3*i+2] << endl;
    //         pose.push_back(joints[i].DOFs[0].getValue());
    //         pose.push_back(joints[i].DOFs[1].getValue());
    //         pose.push_back(joints[i].DOFs[2].getValue());
        //reference issue
    }
    
    // cout << joints[0].DOFs[0].getValue() << endl;
    // cout << skel->Root.DOFs[0].getValue() << endl;
    time += dTime;
}