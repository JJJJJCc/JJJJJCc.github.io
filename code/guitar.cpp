#include "guitar.h"
 
/*
 * Construtor
 */
CKarplusStrongStringPluck::CKarplusStrongStringPluck (float frequency, float sampleRate, float feedback){
    m_buffer.resize(uint32(float(sampleRate) / frequency));
    for (size_t i = 0, c = m_buffer.size(); i < c; ++i) {
        m_buffer[i] = ((float)rand()) / ((float)RAND_MAX) * 2.0f - 1.0f;  // noise
        //m_buffer[i] = float(i) / float(c); // saw wave
    }
    m_index = 0;
    m_feedback = feedback;
}

float CKarplusStrongStringPluck::GenerateSample(){
    // get our sample to return
           float ret = m_buffer[m_index];
    
           // low pass filter (average) some samples
           float value = (m_buffer[m_index] + m_buffer[(m_index + 1) % m_buffer.size()]) * 0.5f * m_feedback;
           m_buffer[m_index] = value;
    
           // move to the next sample
           m_index = (m_index + 1) % m_buffer.size();
    
           // return the sample from the buffer
           return ret;
}

//calculate the frequency of the specified note.
//fractional notes allowed!
float CalcFrequency(float octave, float note)
/*
    Calculate the frequency of any note!
    frequency = 440×(2^(n/12))
 
    N=0 is A4
    N=1 is A#4
    etc...
 
    notes go like so...
    0  = A
    1  = A#
    2  = B
    3  = C
    4  = C#
    5  = D
    6  = D#
    7  = E
    8  = F
    9  = F#
    10 = G
    11 = G#
*/
{
    return (float)(440 * pow(2.0, ((double)((octave - 4) * 12 + note)) / 12.0));
}

void GenerateSamples (std::vector<float>& samples, int sampleRate)
{
    std::vector<CKarplusStrongStringPluck> notes;
 
    enum ESongMode {
        e_twinkleTwinkle,
        e_strum
    };
 
    int timeBegin = 0;
    ESongMode mode = e_twinkleTwinkle;
    for (int index = 0, numSamples = samples.size(); index < numSamples; ++index)
    {
        switch (mode) {
            case e_twinkleTwinkle: {
                const int c_noteTime = sampleRate / 2;
                int time = index - timeBegin;
                // if we should start a new note
                if (time % c_noteTime == 0) {
                    int note = time / c_noteTime;
                    switch (note) {
                        case 0:
                        case 1: {
                            notes.push_back(CKarplusStrongStringPluck(CalcFrequency(3, 0), float(sampleRate), 0.996f));
                            break;
                        }
                        case 2:
                        case 3: {
                            notes.push_back(CKarplusStrongStringPluck(CalcFrequency(3, 7), float(sampleRate), 0.996f));
                            break;
                        }
                        case 4:
                        case 5: {
                            notes.push_back(CKarplusStrongStringPluck(CalcFrequency(3, 9), float(sampleRate), 0.996f));
                            break;
                        }
                        case 6: {
                            notes.push_back(CKarplusStrongStringPluck(CalcFrequency(3, 7), float(sampleRate), 0.996f));
                            break;
                        }
                        case 7: {
                            mode = e_strum;
                            timeBegin = index+1;
                            break;
                        }
                    }
                }
                break;
            }
            case e_strum: {
                const int c_noteTime = sampleRate / 32;
                int time = index - timeBegin - sampleRate;
                // if we should start a new note
                if (time % c_noteTime == 0) {
                    int note = time / c_noteTime;
                    switch (note) {
                        case 0: notes.push_back(CKarplusStrongStringPluck(55.0f, float(sampleRate), 0.996f)); break;
                        case 1: notes.push_back(CKarplusStrongStringPluck(55.0f + 110.0f, float(sampleRate), 0.996f)); break;
                        case 2: notes.push_back(CKarplusStrongStringPluck(55.0f + 220.0f, float(sampleRate), 0.996f)); break;
                        case 3: notes.push_back(CKarplusStrongStringPluck(55.0f + 330.0f, float(sampleRate), 0.996f)); break;
                        case 4: mode = e_strum; timeBegin = index + 1; break;
                    }
                }
                break;
            }
        }
 
        // generate and mix our samples from our notes
        samples[index] = 0;
        for (CKarplusStrongStringPluck& note : notes)
            samples[index] += note.GenerateSample();
 
        // to keep from clipping
        samples[index] *= 0.5f;
    }
}
 
