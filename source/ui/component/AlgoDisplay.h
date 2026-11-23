
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "parameter/DexedApvts.h"
#include "parameter/Model.h"
#include "../util/CachedParameter.h"

#include "../../debugger/Logger.h"

class AlgoDisplay : public juce::Component {
    CachedParameter<int> algoValue;
    CachedParameter<bool> opSwitch[6];

    void displayOp(juce::Graphics &g, char id, int x, int y, char link, char fb);
public:
    AlgoDisplay(DexedApvts &apvts) {
        algoValue.referTo(apvts, IDs::algorithm.name);
        algoValue.callback = [this]() {
            repaint();
        };

        for (int i=0;i<6;i++) {
            opSwitch[i].referTo(apvts, IDs::on.op(i).name);
            opSwitch[i].callback = [this]() {
                repaint();
            };
        }
    }

    void paint(juce::Graphics &g) override;
};
