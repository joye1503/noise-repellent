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

#ifndef NOISE_REPELLENT_HPP_INCLUDED
#define NOISE_REPELLENT_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class NoiseRepellent : public Plugin
{
public:
    
    enum Parameters
    {
        paramLatency,
        paramCount
    };

    NoiseRepellent();
    ~NoiseRepellent() override;

protected:
    //--------------------------------------------------------------------------------------------------------
    // Information

    const char* getLabel() const override
    {
        return "Noise Repellent";
    }

    const char* getDescription() const override
    {
        return "An LV2 plugin for broadband noise reduction.";
    }

    const char* getMaker() const override
    {
        return "Luciano Dato";
    }

    const char* getHomePage() const override
    {
        return "https://github.com/lucianodato/noise-repellent";
    }

    const char* getLicense() const override
    {
        return "LGPLv3";
    }

    uint32_t getVersion() const override
    {
        return d_version(0, 2, 0);
    }

    int64_t getUniqueId() const override
    {
        return d_cconst('N', 's', 'R', 'p');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // DSP functions

    // -------------------------------------------------------------------
    // Process

    void activate() override;
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    //--------------------------------------------------------------------------------------------------------
    //Callbacks (optional)

    void sampleRateChanged(double newSampleRate) override;


private:
    // Parameters
    float fLatency;
    uint32_t fLatencyInFrames;

    // Buffer for previous audio, size depends on sample rate
    float* fBuffer;
    uint32_t fBufferPos;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseRepellent)
};
// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // NOISE_REPELLENT_HPP_INCLUDED