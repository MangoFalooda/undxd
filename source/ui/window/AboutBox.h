
#pragma once

#include "../../debugger/Logger.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "../DXLookNFeel.h"

class AboutBox : public juce::DialogWindow {
public:
    AboutBox() : juce::DialogWindow("About", juce::Colour(0xFF171A20), true) {
        setUsingNativeTitleBar(false);
        setAlwaysOnTop(true);
        setOpaque(true);

        setSize(460, 550);

        undxd.setURL(juce::URL("https://github.com/MangoFalooda/undxd"));
        undxd.setButtonText("https://github.com/MangoFalooda/undxd");
        undxd.setColour(juce::HyperlinkButton::textColourId, DXLookNFeel::fillColour);
        undxd.setJustificationType(juce::Justification::left);

        dexed.setURL(juce::URL("https://asb2m10.github.io/dexed/"));
        dexed.setButtonText("https://asb2m10.github.io/dexed/");
        dexed.setColour(juce::HyperlinkButton::textColourId, DXLookNFeel::fillColour);
        dexed.setJustificationType(juce::Justification::left);

        surge.setURL(juce::URL("https://surge-synthesizer.github.io/"));
        surge.setButtonText("https://surge-synthesizer.github.io/");
        surge.setColour(juce::HyperlinkButton::textColourId, DXLookNFeel::fillColour);
        surge.setJustificationType(juce::Justification::left);

        addAndMakeVisible(&undxd);
        addAndMakeVisible(&dexed);
        addAndMakeVisible(&surge);

        undxd.setBounds(18, 412, getWidth() - 36, 26);
        dexed.setBounds(18, 436, getWidth() - 36, 26);
        surge.setBounds(18, 460, getWidth() - 36, 26);        
    }

    void closeButtonPressed() override { setVisible(false); }

    void paint(juce::Graphics& g) override {
        auto bounds = getLocalBounds().toFloat();

        g.fillAll(DXLookNFeel::background);

        g.setColour(DXLookNFeel::secondaryText);
        g.drawRoundedRectangle(bounds.reduced(0.5f), 8.0f, 1.0f);

        auto logoArea = getLocalBounds().withTrimmedTop(38).withHeight(90);

        g.setFont(juce::Font(64.0f, juce::Font::bold));
        g.setColour(DXLookNFeel::textColour);
        g.drawText("UNDXD", logoArea, juce::Justification::centred);

        g.setFont(juce::Font(12.0f, juce::Font::bold));
        g.setColour(DXLookNFeel::fillColour);
        g.drawText("FM SYNTHESIZER", 0, 128, getWidth(), 20, juce::Justification::centred);

        g.setFont(juce::Font(12.0f));
        g.setColour(DXLookNFeel::secondaryText);

        g.drawText("Version " DEXED_VERSION "  |  Build " __DATE__, 0, 157, getWidth(), 20,
                   juce::Justification::centred);

        g.setColour(DXLookNFeel::borderOutline);
        g.drawHorizontalLine(195, 22.0f, getWidth() - 22.0f);

        g.setFont(juce::Font(14.0f, juce::Font::bold));
        g.setColour(DXLookNFeel::secondaryText);
        g.drawText("Credits", 20, 215, getWidth() - 40, 24, juce::Justification::left);

        g.setFont(juce::Font(12.5f));
        g.setColour(juce::Colour(0xFF92969D));

        const juce::String credits = "This software is released under the GPL V3.\n\n"
                                     "DSP Engine: Original MSFA project by Raph Levin,\n"
                                     "enhancements by Pascal Gauthier.\n"
                                     "UI Programming: Pascal Gauthier.\n"
                                     "UI Design: AZur Studio.\n\n"
                                     "Credits to Surge Synthesizer Team for\n"
                                     "MPE and microtuning support.\n\n"
                                     "Credits to GitHub users: tico-tico, Sentinel77,\n"
                                     "jeremybernstein. Filters based on OB-Xd.";

        g.drawMultiLineText(credits, 22, 250, getWidth() - 44);

        g.setColour(DXLookNFeel::borderOutline);
        g.drawHorizontalLine(415, 22.0f, getWidth() - 22.0f);
    }

private:
    juce::HyperlinkButton dexed, surge, undxd;
};

