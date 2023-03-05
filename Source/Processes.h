/*
  ==============================================================================

    Gain.h
    Created: 26 Feb 2023 10:30:28pm
    Author:  Chris

  ==============================================================================
*/

#include "StereoSample.h"
#pragma once

//this file contains a few small classes that perform some operations on sample data that I needed done


//=======================================================================

class Gain
{
public:

    //this can be called without the need to instantiate an object
    //just multiplies the input sample by a scalar gain between 0 and 1
    static void process(StereoSample& sample, float gain)
    {
        if (gain < 0 || gain > 1)
            jassertfalse;   //gain must be between 0 and 1 inclusive

        sample.leftData *= gain;
        sample.rightData *= gain;
    }
};


//=======================================================================
class Pan
{
public:

    //takes in a single Pan value and calculates a modifer for the left & right volume levels
    //I'm using a simple linear algorithm here. 
    //A pan of 1 sets leftVol to 0 and rightVol to 2. 
    //Vice verse for a pan of -1.
    //A pan of 0 sets both leftVol and rightVol to 1.
    void setPan(float pan)
    {
        if (pan < -1 || pan > 1)
            jassertfalse; //pan must be between -1 and 1 inclusive

        float normalized = (pan + 1) * 0.5f;

        leftVol  = (1.0 - normalized) * 2;
        rightVol = normalized * 2;

    }

    //applies the left & right volume changes to the supplied StereoSample
    void process(StereoSample& sample)
    {
        sample.leftData  *= leftVol;
        sample.rightData *= rightVol;
    }

private:
    float leftVol  = 1;
    float rightVol = 1;

};


//=======================================================================
class Normalize
{
    public:

        //The oscillators I've built don't take any information into consideration about what already exists in the audio buffer.
        //As a result, processing multiple oscillators at once can lead to final sample values well over 1, which is undesireable.
        //This function just divides the sample value by the total number of oscillators used to get it back to 
        //a value between -1 and 1 before it gets written to the audio buffer.
        static void normalizeOscillators(StereoSample& sample, int numOfOscillators)
        {
            sample.leftData /= numOfOscillators;
            sample.rightData /= numOfOscillators;
        }
};