#pragma once

#include "parameter/DexedApvts.h"
#include "../DXLookNFeel.h"


class ParameterObserver : public juce::Component, juce::ValueTree::Listener {
    const juce::String defaultMsg = "DEXED " DEXED_VERSION;
    DexedApvts &apvts;

    juce::RangedAudioParameter *parameter = nullptr;
public:
    ParameterObserver(juce::Component *topComponent, DexedApvts &apvts) : apvts(apvts) {
        topComponent->addMouseListener(this, true);
        apvts.state.addListener(this);
    }

    ~ParameterObserver() override {
        apvts.state.removeListener(this);
    }

    void mouseEnter(const juce::MouseEvent &event) override {
        juce::Component *comp = event.eventComponent;
        if ( comp != nullptr ) {
            juce::String name = comp->getName();
            if ( name.isEmpty() )
                return;
            juce::RangedAudioParameter *param = apvts.getParameter(name);
            if ( param != nullptr ) {
                parameter = param;
                repaint();
            }
        }
    }

    void paint(juce::Graphics &g) override {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);

        //g.setColour(DXLookNFeel::background);
        //g.fillRoundedRectangle(bounds, 3.0f);
        g.setColour(DXLookNFeel::borderOutline);
        g.drawRoundedRectangle(bounds.reduced(0.5f), 2.0f, 1.0f);

        g.setColour(DXLookNFeel::textColour);
        juce::RangedAudioParameter *param = parameter;
        if ( param != nullptr ) {
            g.drawFittedText(param->getName(100) + ": " + param->getCurrentValueAsText(),
                             bounds.toNearestInt(),
                        Justification::centred, false);
        } else {
            g.drawFittedText(defaultMsg, bounds.toNearestInt(),
                        Justification::centred, false);
        }
    }

    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override {
        juce::RangedAudioParameter *param = parameter;
        if ( apvts.pushToParameterInProgress )
            return;
        repaint();

    }
};
