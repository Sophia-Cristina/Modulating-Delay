/*
  ==============================================================================

    DelayClass.h
    Created: 30 Jul 2020 2:57:19pm
    Author:  ySPHAx

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

//==============================================================================
/*
*/
class DelayClass : public juce::Component
{
public:

    float MSNum = 100, FeedBNum = 0.75, FloorNum = 1, MultiNum = 0.5, SamplesinMS; // MS number from Slider, the same for the rest | SamplesinMS = How much samples there is in a millisecond
    float Omega = 1, ModMS = 100, ModFB = 0.75;
    // SampleRate | Delay Buffer Size from total permitted MS from the slider | The size of a Data block from a Buffer | Number of samples inside the delay time set by the slider (MSNum)
    int SampleRate, DelayBFSize, DelayBFDataSizeView = 0, BufferSize = 0, DelaySamples;
    //std::vector<int> DelayLine = std::vector<int>::vector(44100);
    juce::AudioBuffer<float> DelayBuffer;

    int WritePos = 0;

    // WAVETABLES:
    int TableSize = 256;
    std::vector<float> SineWave = std::vector<float>::vector(TableSize);
    int CntTable = 0;

    // OPTIONS:
    enum Options { Off, Signal, SineOp, SquareOp, TriOp, SawOp };

    DelayClass()
    {
        for (int n = 0; n < SineWave.size(); ++n) { SineWave[n] = sin((6.283185 * n) / SineWave.size()); }
    }

    ~DelayClass() override
    {
    }

    // #################################################
    // #################################################
    // #################################################

    float ModulateNum()
    {
        CntTable += WritePos;
        int SamplesInFreq = SampleRate / Omega;
        while (CntTable > SamplesInFreq) { CntTable -= SamplesInFreq; }
        int WavePos = round((CntTable / SamplesInFreq) * (TableSize - 1));
        return(((SineWave[WavePos]) + FloorNum) * MultiNum);
    }

    void FillDelayBuffer(int channel, const int BFDataSize, const int DelayBFDataSize, const float* BFDataReadPointer, const float* DelayBFDataReadPointer)
    {
        int NumSamples = BFDataSize;
        float* WritePointer = DelayBuffer.getWritePointer(channel);
        if (DelayBFDataSize > BFDataSize + WritePos)
        {
            if (NumSamples > 0)
            {
                //const auto increment = (endGain - startGain) / BFDataSize;
                float* d = WritePointer + WritePos;

                while (--NumSamples >= 0)
                {
                    *d++ = FeedBNum * (sin((Omega * 6.283185 * (WritePos + NumSamples)) / SampleRate) + FloorNum) * MultiNum * *BFDataReadPointer++;
                }
            }
            //DelayBuffer.copyFromWithRamp(channel, WritePos, BFDataReadPointer, BFDataSize, ModFB, ModFB);
        }
        else
        {
            const int BFRemaining = DelayBFDataSize - WritePos;
            NumSamples = BFRemaining;
            if (NumSamples > 0)
            {
                //const auto increment = (endGain - startGain) / BFDataSize;
                float* d = WritePointer + WritePos;

                while (--NumSamples >= 0)
                {
                    *d++ = FeedBNum * (sin((Omega * 6.283185 * (WritePos + NumSamples)) / SampleRate) + FloorNum) * MultiNum * *BFDataReadPointer++;
                }
            }
            //DelayBuffer.copyFromWithRamp(channel, WritePos, BFDataReadPointer, BFRemaining, ModFB, ModFB);
            NumSamples = BFDataSize - BFRemaining;
            if (NumSamples > 0)
            {
                //const auto increment = (endGain - startGain) / BFDataSize;
                float* d = WritePointer;

                while (--NumSamples >= 0)
                {
                    *d++ = FeedBNum * (sin((Omega * 6.283185 * NumSamples) / SampleRate) + FloorNum) * MultiNum * *BFDataReadPointer++;
                }
            }
            //DelayBuffer.copyFromWithRamp(channel, 0, BFDataReadPointer, BFDataSize - BFRemaining, ModFB, ModFB);
            //DelayBuffer.copyFromWithRamp(channel, 0, BFDataReadPointer, BFDataSize + BFRemaining, 0.8, 0.8); // If you hear cracks, this is a fix by a user in the comment section of The Audio Programmer tuto 40
        }
    }

    void GetFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, const int BFDataSize, const int DelayBFDataSize, const float* BFDataReadPointer, const float* DelayBFDataReadPointer)
    {
        // DelayBFDataSize is just the DelayBFSize, but to not mix both togheter, the data in DataSize means that is gathered from the processblock method.
        // static_cast<int> is equal to doing (int)1.0!
        const int ReadPos = static_cast<int> (DelayBFDataSize + WritePos - (ModMS * SamplesinMS)) % DelayBFDataSize;

        if (DelayBFDataSize > BFDataSize + ReadPos) { buffer.copyFrom(channel, 0, DelayBFDataReadPointer + ReadPos, BFDataSize); }
        else
        {
            const int BFRemaining = DelayBFDataSize - ReadPos;
            buffer.copyFrom(channel, 0, DelayBFDataReadPointer + ReadPos, BFRemaining); buffer.copyFrom(channel, BFRemaining, DelayBFDataReadPointer, BFDataSize - BFRemaining);
        }
    }

    void FeedbackDelay(int channel, const int BFDataSize, const int DelayBFDataSize, float* DryBuffer)
    {
        if (DelayBFDataSize > BFDataSize + WritePos)
        {
            DelayBuffer.addFromWithRamp(channel, WritePos, DryBuffer, BFDataSize, 0.9, 0.9);
        }
        else
        {
            const int BFRemaining = DelayBFDataSize - WritePos;
            DelayBuffer.addFromWithRamp(channel, BFRemaining, DryBuffer, BFRemaining, 0.9, 0.9);
            DelayBuffer.addFromWithRamp(channel, 0, DryBuffer, BFDataSize - BFRemaining, 0.9, 0.9);
        }
    }

    // #################################################
    // #################################################
    // #################################################

    // IF YOU SUDDENLY NEED GRAPHS, HERE IT IS!!!!
    // MAYBE FOR INFORMATION ON SCREEN!
    /*void paint (juce::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */
        /* g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background


         g.setColour (juce::Colours::grey);
         g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

         g.setColour (juce::Colours::white);
         g.setFont (14.0f);
         g.drawText ("DelayClass", getLocalBounds(),
                     juce::Justification::centred, true);   // draw some placeholder text
     }*/

     /*void resized() override
     {
         // This method is where you should set the bounds of any child
         // components that your component contains..

     }*/

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayClass)
};
