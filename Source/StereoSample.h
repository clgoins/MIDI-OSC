/*
  ==============================================================================

    StereoSample.h
    Created: 26 Feb 2023 10:21:22pm
    Author:  Chris

  ==============================================================================
*/

#pragma once

//quick struct that holds 2 floating point values to represent two samples; one for the left channel, and one for the right channel
//used to pass around between processes that might modify sample data before they get written to the output buffer

struct StereoSample
{
    float leftData = 0;
    float rightData = 0;
};