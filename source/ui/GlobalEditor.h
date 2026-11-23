#pragma once

#include "PluginProcessor.h"
#include "component/AlgoDisplay.h"
#include "component/EnvDisplay.h"
#include "component/ParameterObserver.h"
#include "DXLookNFeel.h"
#include "component/VUMeter.h"
#include "util/AudioComponentContainer.h"

#ifdef IMPLEMENT_MidiMonitor
#    include "SysexComm.h"
#endif

class GlobalEditor : public juce::Component {
public:
    GlobalEditor(DexedAudioProcessor& processor, juce::Component* parent);
    ~GlobalEditor() override;

    void updatePitchPos(int pos);
    void updateDisplay();


#ifdef IMPLEMENT_MidiMonitor
    std::unique_ptr<juce::Component> midiMonitor;
#endif


    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    DexedAudioProcessor& processor;

    std::unique_ptr<AudioComponentContainer> binder;

    struct SliderControl {
        juce::Slider* slider = nullptr;
        juce::Label* label = nullptr;
    };

    std::vector<std::unique_ptr<juce::Label>> labels;

    std::vector<SliderControl> masterControls;
    std::vector<SliderControl> algorithmControls;
    std::vector<SliderControl> lfoControls;
    std::vector<SliderControl> pitchControls;

    juce::Rectangle<int> masterArea;
    juce::Rectangle<int> filterArea;
    juce::Rectangle<int> algorithmArea;
    juce::Rectangle<int> lfoArea;
    juce::Rectangle<int> pitchArea;

    std::unique_ptr<juce::ToggleButton> oscSync;
    std::unique_ptr<juce::ToggleButton> lfoSync;
    std::unique_ptr<juce::ToggleButton> monoMode;
    std::unique_ptr<juce::Label> monoModeLabel;
    std::unique_ptr<juce::Label> lfoSyncLabel;
    std::unique_ptr<juce::Label> oscSyncLabel;

    std::unique_ptr<juce::Label> pitchLevelLabel;
    std::unique_ptr<juce::Label> pitchRateLabel;

    std::unique_ptr<PitchEnvDisplay> pitchEnvDisplay;
    std::unique_ptr<AlgoDisplay> algoDisplay;

    LfoComboLookAndFeel lfoComboLaf;
    std::unique_ptr<juce::ComboBox> lfoType;

    std::unique_ptr<juce::MouseListener> contextMenuAdapter;


    void addRotary(const juce::String& parameter, const juce::String& label, int focusOrder,
                   std::vector<SliderControl>& group, bool showValue = false);

    void layoutControls(std::vector<SliderControl>& controls, juce::Rectangle<int> area,
                        int columns, int rows = 1);

    void drawSection(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& title);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalEditor)
};