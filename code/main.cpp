#include "guitar.h"

//the entry point of our application
int main(int argc, char **argv)
{
    // sound format parameters
    const int c_sampleRate = 44100;
    const int c_numSeconds = 9;
    const int c_numChannels = 2;
    const int c_numSamples = c_sampleRate * c_numChannels * c_numSeconds;
 
    // make space for our samples
    std::vector<float> samples;
    samples.resize(c_numSamples);
 
    // generate samples
    GenerateSamples(samples, c_sampleRate);
 
    // convert from float to the final format
    std::vector<int32> samplesInt;
    ConvertFloatSamples(samples, samplesInt);
 
    // write our samples to a wave file
    WriteWaveFile("out.wav", samplesInt, c_numChannels, c_sampleRate);
}
