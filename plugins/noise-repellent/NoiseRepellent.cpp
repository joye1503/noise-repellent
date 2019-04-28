/*
noise-repellent -- Noise Reduction LV2

Copyright 2016 Luciano Dato <lucianodato@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/
*/

#include "NoiseRepellent.hpp"

static const float denormalGuard = 1e-15f; // http://www.earlevel.com/main/2019/04/19/floating-point-denormals/
static const uint32_t maxBufferSec = 6; // max buffer allocation in seconds - enough to consider all Sample rates

START_NAMESPACE_DISTRHO

//--------------------------------------------------------------------------------------------------------
//Init

NoiseRepellent::NoiseRepellent()
    : Plugin(paramCount, 0, 0) // 1 parameter
{
    // Default values
    fLatency = 1.0f;
    fLatencyInFrames = 0;
    fBuffer = nullptr;
    fBufferPos = 0;

    // allocates buffer
    sampleRateChanged(getSampleRate());

    // Update latency
    // setLatency(fLatencyInFrames);
}

NoiseRepellent::~NoiseRepellent()
{
    delete[] fBuffer;
}

void NoiseRepellent::initParameter(uint32_t index, Parameter &parameter)
{
    switch (index)
    {
        case paramLatency:
            parameter.hints = kParameterIsAutomable | kParameterIsInteger;
            parameter.name = "FFT size";
            parameter.symbol = "fftSize";
            parameter.unit = " ";
            parameter.ranges.def = 1;
            parameter.ranges.min = 0;
            parameter.ranges.max = 2;
            break;
    }
}

//--------------------------------------------------------------------------------------------------------
//Internal data

float NoiseRepellent::getParameterValue(uint32_t index) const 
{
    switch (index)
    {
        case paramLatency:
            return fLatency; 
        default:
            return 0.f;
    }
}

void NoiseRepellent::setParameterValue(uint32_t index, float value)
{
    if (getSampleRate() <= 0.f)
        return;

    switch (index)
    {
    case paramLatency:
        switch((uint32_t)value)
        {
            case 0:
                fLatency = 0.0232199546f; // 1024 at 44.1kHz
                break;
            case 1:
                fLatency = 0.0464399093f; // 2048 at 44.1kHz
                break;
            case 2:
                fLatency = 0.0928798186f; // 4096 at 44.1kHz
                break;
        }
        break;
    }

    // Setting internal values based on new parameters
    fLatencyInFrames = fLatency * getSampleRate();
    // Update latency
    setLatency(fLatencyInFrames);
}


// -----------------------------------------------------------------------
// DSP functions

// -----------------------------------------------------------------------
// Audio Processing

void NoiseRepellent::activate()
{
}

void NoiseRepellent::deactivate()
{
}

void NoiseRepellent::run(const float **inputs, float **outputs, uint32_t frames)
{
    const float *const in = inputs[0];
    /* */ float *const out = outputs[0];

    if (fLatencyInFrames == 0)
    {
        if (out != in)
            std::memcpy(out, in, sizeof(float) * frames);
        return;
    }

    // Put the new audio in the buffer.
    std::memcpy(fBuffer + fBufferPos, in, sizeof(float) * frames);
    fBufferPos += frames;

    // buffer is not filled enough yet
    if (fBufferPos < fLatencyInFrames + frames)
    {
        // silence output
        std::memset(out, 0, sizeof(float) * frames);
    }
    // buffer is ready to copy
    else
    {
        // copy latency buffer to output
        const uint32_t readPos = fBufferPos - fLatencyInFrames - frames;
        std::memcpy(out, fBuffer + readPos, sizeof(float) * frames);

        // move latency buffer back by some frames
        std::memmove(fBuffer, fBuffer + frames, sizeof(float) * fBufferPos);
        fBufferPos -= frames;
    }
}

 //--------------------------------------------------------------------------------------------------------
 //Callbacks (for sample rate changes management)

void NoiseRepellent::sampleRateChanged(double newSampleRate)
{
    if (fBuffer != nullptr)
        delete[] fBuffer;

    //making sure the buffer is big enough for fft resolution at big sample rates 
    const uint32_t maxFrames = newSampleRate * maxBufferSec;

    fBuffer = new float[maxFrames];
    std::memset(fBuffer, 0, sizeof(float) * maxFrames);

    fLatencyInFrames = fLatency * newSampleRate;
    fBufferPos = 0;
}

// -------------------------------------------------------------------------------------------------------

Plugin *createPlugin()
{
    return new NoiseRepellent();
}

END_NAMESPACE_DISTRHO