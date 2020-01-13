#ifndef guitar_include
#define guitar_include

#include <stdio.h>
#include <memory.h>
#include <inttypes.h>
#include <vector>
#include <cmath>

// constants
const float c_pi = 3.14159265359f;
const float c_twoPi = 2.0f * c_pi;
 
// typedefs
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef int16_t     int16;
typedef int32_t     int32;
 
//this struct is the minimal required header data for a wav file
struct SMinimalWaveFileHeader
{
    //the main chunk
    unsigned char m_chunkID[4];
    uint32        m_chunkSize;
    unsigned char m_format[4];
 
    //sub chunk 1 "fmt "
    unsigned char m_subChunk1ID[4];
    uint32        m_subChunk1Size;
    uint16        m_audioFormat;
    uint16        m_numChannels;
    uint32        m_sampleRate;
    uint32        m_byteRate;
    uint16        m_blockAlign;
    uint16        m_bitsPerSample;
 
    //sub chunk 2 "data"
    unsigned char m_subChunk2ID[4];
    uint32        m_subChunk2Size;
 
    //then comes the data!
};

class CKarplusStrongStringPluck
{
public:
    CKarplusStrongStringPluck(float frequency, float sampleRate, float feedback);
    float GenerateSample();
 
private:
    std::vector<float>  m_buffer;
    size_t              m_index;
    float               m_feedback;
};
 

/*
* This fucntion writes the wav file
* Input Parameters: file name, data vector, number of channels, sampleRate
* Return: true for writing file successfull; flase for not success
*/
template <typename T>
bool WriteWaveFile(const char* fileName, std::vector<T> data, int16 numChannels, int32 sampleRate)
{
    int32 dataSize = data.size() * sizeof(T); //get data size
    int32 bitsPerSample = sizeof(T) * 8; // the sizeof(T) returns the value in byte, *8 to get values in bits
 
    //open the file if we can
    FILE *File = nullptr;
    File = fopen(fileName, "w");//w+b
    if (!File) return false;
 
    SMinimalWaveFileHeader waveHeader; //declare a wavefile
 
    //fill out the main chunk
    memcpy(waveHeader.m_chunkID, "RIFF", 4);
    waveHeader.m_chunkSize = dataSize + 36;
    memcpy(waveHeader.m_format, "WAVE", 4);
 
    //fill out sub chunk 1 "fmt "
    memcpy(waveHeader.m_subChunk1ID, "fmt ", 4);
    waveHeader.m_subChunk1Size = 16;
    waveHeader.m_audioFormat = 1;
    waveHeader.m_numChannels = numChannels;
    waveHeader.m_sampleRate = sampleRate;
    waveHeader.m_byteRate = sampleRate * numChannels * bitsPerSample / 8;
    waveHeader.m_blockAlign = numChannels * bitsPerSample / 8;
    waveHeader.m_bitsPerSample = bitsPerSample;
 
    //fill out sub chunk 2 "data"
    memcpy(waveHeader.m_subChunk2ID, "data", 4);
    waveHeader.m_subChunk2Size = dataSize;
 
    //write the header
    fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, File);
 
    //write the wave data itself
    fwrite(&data[0], dataSize, 1, File);
 
    //close the file and return success
    fclose(File);
    return true;
}
 
/*
* This fucntion converts the float to int(rounding)
* Input Parameters: file name, data vector, number of channels, sampleRate
* Return: true for writing file successfull; flase for not success
*/
template <typename T>
void ConvertFloatSamples (const std::vector<float>& in, std::vector<T>& out)
{
    // make our out samples the right size
    out.resize(in.size());
 
    // convert in format to out format !
    for (size_t i = 0, c = in.size(); i < c; ++i)
    {
        float v = in[i];
        if (v < 0.0f)
            v *= -float(std::numeric_limits<T>::lowest());
        else
            v *= float(std::numeric_limits<T>::max());
        out[i] = T(v);
    }
}

float CalcFrequency(float octave, float note);
void GenerateSamples (std::vector<float>& samples, int sampleRate);
#endif
