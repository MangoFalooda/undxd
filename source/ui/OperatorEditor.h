#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "component/EnvDisplay.h"
#include "component/VUMeter.h"
#include "DXLookNFeel.h"
#include "util/AudioComponentContainer.h"

class OperatorDisplay;
class OperatorSwitch;

class OperatorSection : public juce::Component {
public:
    OperatorSection(const juce::String& title) : title(title) {}

    void paint(juce::Graphics& g) override {
        //auto bounds = getLocalBounds().toFloat();

        //g.setColour(juce::Colour(0xFF171A20));
        //g.fillRoundedRectangle(bounds, 6.0f);
    }

private:
    juce::String title;
};



class OperatorEditor : public juce::Component {
public:
    OperatorEditor(DexedAudioProcessor& processor, int num);
    ~OperatorEditor() override;

    void updateGain(float v);
    void updateDisplay();
    void updateEnvPos(char pos);
    void mouseDown(const juce::MouseEvent& e) override;
    void toggleOpSwitch();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setLayoutRow(bool isGrid) {
        layoutGrid = isGrid;
        resized();
    }

private:
    struct Control {
        juce::Slider* slider = nullptr;
        juce::Label* label = nullptr;
    };

    DexedAudioProcessor& processor;
    int internalOp;
    juce::String opNum;
    bool layoutGrid{false};

    std::unique_ptr<EnvDisplay> envDisplay;
    std::unique_ptr<OperatorSwitch> opSwitch;
    std::unique_ptr<juce::ToggleButton> opMode;
    std::unique_ptr<VuMeter> vu;
    std::unique_ptr<juce::MouseListener> contextMenuAdapter;
    std::unique_ptr<AudioComponentContainer> binder;

    std::vector<std::unique_ptr<juce::Label>> labels;

    std::unique_ptr<OperatorSection> frequencySection;
    std::unique_ptr<OperatorSection> envelopeSection;
    std::unique_ptr<OperatorSection> outputSection;
    std::unique_ptr<OperatorSection> scalingSection;

    std::unique_ptr<juce::Label> EGLevelLabel;
    std::unique_ptr<juce::Label> EGRateLabel;

    std::vector<Control> frequencyControls;

    std::vector<Control> outputControls;

    std::vector<Control> envelopeControls;

    std::vector<Control> scalingControls;

    CurveComboLookAndFeel leftCurveLaf{false};  
    CurveComboLookAndFeel rightCurveLaf{true}; 
    juce::ComboBox* leftCurve = nullptr;
    juce::ComboBox* rightCurve = nullptr;
    std::unique_ptr<juce::Label> leftCurveLabel;
    std::unique_ptr<juce::Label> rightCurveLabel;

    OperatorDisplay* operatorDisplay = nullptr;

    juce::Slider* addSlider(const juce::String& parameter, const juce::String& label,
                            int focusOrder, std::vector<Control>& group,
                            juce::Slider::SliderStyle style = juce::Slider::RotaryVerticalDrag);

    void addCurveCombo(const juce::String& parameter, const juce::String& labelText, int focusOrder,
                       juce::ComboBox*& destination);
    void layoutControl(Control& control, juce::Rectangle<int> area);

    void drawSection(juce::Graphics& g, juce::Rectangle<float> area, const juce::String& title);


    float verticalDividerX = 0.0f;
    float leftDividerY = 0.0f;
    float rightDivider1Y = 0.0f;
    float rightDivider2Y = 0.0f;


    OperatorPowerLookAndFeel powerLookAndFeel;
    OperatorModeLookAndFeel modeLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OperatorEditor)
};