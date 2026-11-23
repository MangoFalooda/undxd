
#pragma once

#include "JuceHeader.h"
#include "../DXLookNFeel.h"

class VuMeter : public juce::Component {
public:
    static constexpr int numSegments = 47;
    float v = 0.0f;  

    explicit VuMeter(float maxDecibels = 0.0f) : maxDb(maxDecibels), minDb(-50.0f) {
        setInterceptsMouseClicks(false, false);
    }

    void setLevel(float inGain) {
        v = inGain;
        repaint();
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();
        if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
            return;

        g.setColour(DXLookNFeel::background);
        g.fillRoundedRectangle(bounds, 2.0f);

        float normLevel = 0.0f;

        if (v > 0.00001f) {
            float db = (v < 0.0f) ? v : juce::Decibels::gainToDecibels(v, minDb);
            normLevel = juce::jmap(db, minDb, maxDb, 0.0f, 1.0f);
        } else if (v < 0.0f) 
        {
            normLevel = juce::jmap(v, minDb, maxDb, 0.0f, 1.0f);
        }

        normLevel = juce::jlimit(0.0f, 1.0f, normLevel);

        auto innerArea = bounds.reduced(1.5f, 2.0f);
        const float totalWidth = innerArea.getWidth();
        const float gap = 1.0f;
        const float segmentWidth =
            (totalWidth - (gap * (numSegments - 1))) / static_cast<float>(numSegments);
        const int activeSegments = static_cast<int>(std::round(normLevel * numSegments));

        for (int i = 0; i < numSegments; ++i) {
            auto segX = innerArea.getX() + i * (segmentWidth + gap);
            juce::Rectangle<float> segRect(segX, innerArea.getY(), segmentWidth,
                                           innerArea.getHeight());

            if (i < activeSegments) {
                g.setColour(DXLookNFeel::fillColour);
                g.fillRect(segRect);
            } else {
                g.setColour(DXLookNFeel::fillColour.withAlpha(0.3f));
                g.fillRect(segRect);
            }
        }
    }

private:
    float minDb = -50.0f;
    float maxDb = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VuMeter)
};



class VuMeterOutput : public juce::Component {
public:
    static constexpr int totalBlocks = 47;
    static constexpr int numRedBlocks = 6;
    static constexpr int numYellowBlocks = 6;
    static constexpr int numGreenBlocks = totalBlocks - numRedBlocks - numYellowBlocks;  // 35

    float v = 0.0f;

    explicit VuMeterOutput(float maxDecibels = 6.0f) : maxDb(maxDecibels), minDb(-50.0f) {
        setInterceptsMouseClicks(false, false);
    }

     static double getDecayFactor(double sampleRate) {
                constexpr double dBdrop = -40.0;    
                constexpr double fallingTime = 0.3;  
                return std::exp(dBdrop / 10.0 * std::log(10.0) / (fallingTime * sampleRate));
            }

    void setLevel(float inGain) {
        v = inGain;
        repaint();
    }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();
        if (bounds.getWidth() <= 0.0f || bounds.getHeight() <= 0.0f)
            return;

        g.setColour(DXLookNFeel::background);
        g.fillRoundedRectangle(bounds, 2.0f);

        g.setColour(DXLookNFeel::borderOutline);
        g.drawRoundedRectangle(bounds.reduced(0.5f), 2.0f, 1.0f);

        float normLevel = 0.0f;
        if (v > 0.00001f) {
            float db = (v < 0.0f) ? v : juce::Decibels::gainToDecibels(v, minDb);
            normLevel = juce::jmap(db, minDb, maxDb, 0.0f, 1.0f);
        } else if (v < 0.0f) {
            normLevel = juce::jmap(v, minDb, maxDb, 0.0f, 1.0f);
        }
        normLevel = juce::jlimit(0.0f, 1.0f, normLevel);

        auto innerArea = bounds.reduced(2.0f, 2.0f);
        const float totalWidth = innerArea.getWidth();
        const float gap = 1.0f;
        const float segWidth =
            (totalWidth - (gap * (totalBlocks - 1))) / static_cast<float>(totalBlocks);
        const int activeSegments = static_cast<int>(std::round(normLevel * totalBlocks));

        constexpr int yellowStart = numGreenBlocks;                 
        constexpr int redStart = numGreenBlocks + numYellowBlocks;  

        for (int i = 0; i < totalBlocks; ++i) {
            const float x = innerArea.getX() + i * (segWidth + gap);
            juce::Rectangle<float> segRect(x, innerArea.getY(), segWidth, innerArea.getHeight());

            const bool isActive = (i < activeSegments);

            juce::Colour baseColor;
            if (i >= redStart)
                baseColor = isActive ? juce::Colour(0xFFFF3B4B) : juce::Colour(0xFF401217);
            else if (i >= yellowStart)
                baseColor = isActive ? juce::Colour(0xFFFFB834)
                                     : juce::Colour(0xFF382A0E); 
            else
                baseColor =
                    isActive ? juce::Colour(0xFF39C6B0) : juce::Colour(0xFF13322C);  

            g.setColour(baseColor);
            g.fillRect(segRect);

            if (isActive) {
                g.setColour(juce::Colours::white.withAlpha(0.22f));
                g.fillRect(segRect.removeFromTop(segRect.getHeight() * 0.35f));
            }
        }
    }

private:
    float minDb = -50.0f;
    float maxDb = 6.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VuMeterOutput)
};