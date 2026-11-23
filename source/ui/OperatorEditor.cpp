

#include "OperatorEditor.h"
#include "PluginEditor.h"
#include "component/OperatorDisplay.h"
#include "util/ContextMenuAdapter.h"

#ifndef M_LN10
#    define M_LN10 2.30258509299404568402
#endif

#pragma once

#include <JuceHeader.h>

class OperatorSwitch : public juce::ToggleButton {
public:
    explicit OperatorSwitch(const juce::String& name) : juce::ToggleButton(name) {
        setClickingTogglesState(true);
        setTitle("Operator enable");
        setTooltip("Enable or disable operator");
    }
};

class OperatorContextMenu : public ContextMenuAdapter {
    int op;

public:
    OperatorContextMenu(DexedAudioProcessor& processor, int op)
        : ContextMenuAdapter(processor), op(op) {}

    void addSpecificItems(PopupMenu& menu, juce::Component* source) override {
        menu.addItem("Copy Operator Values", [this] { processor.copyToClipboard(op); });
        menu.addItem("Paste Envelope Values", DexedClipboard().isOperatorData(), false,
                     [this] { processor.pasteEnvFromClipboard(op); });
        menu.addItem("Paste Operator Values", DexedClipboard().isOperatorData(), false,
                     [this] { processor.pasteOpFromClipboard(op); });
        menu.addSeparator();
    }
};


//==============================================================================
#include "OperatorEditor.h"

OperatorEditor::OperatorEditor(DexedAudioProcessor& processor, int num)
    : processor(processor), internalOp(5 - num), opNum(juce::String(num + 1)) {
    binder = std::make_unique<AudioComponentContainer>(*this, processor.parameters);

    setTitle("Operator " + opNum);
    setFocusContainerType(juce::Component::FocusContainerType::focusContainer);
    setWantsKeyboardFocus(true);

    frequencySection = std::make_unique<OperatorSection>("FREQUENCY");
    envelopeSection = std::make_unique<OperatorSection>("ENVELOPE");
    outputSection = std::make_unique<OperatorSection>("OUTPUT");
    scalingSection = std::make_unique<OperatorSection>("KEYBOARD SCALING");

    addAndMakeVisible(*frequencySection);
    addAndMakeVisible(*envelopeSection);
    addAndMakeVisible(*outputSection);
    addAndMakeVisible(*scalingSection);

    addSlider(IDs::detune.op(num).name, "Tune", 1, frequencyControls);

    addSlider(IDs::frequencyCoarse.op(num).name, "Coarse", 2, frequencyControls);

    addSlider(IDs::frequencyFine.op(num).name, "Fine", 3, frequencyControls);

    for (int i = 0; i < 4; ++i) { 
        addSlider(IDs::egLevel.op(num).idx(i).name, "" + juce::String(i + 1), 4 + i * 2, envelopeControls);
        addSlider(IDs::egRate.op(num).idx(i).name, "", 5 + i * 2,  envelopeControls);
    }

    
    EGLevelLabel = std::make_unique<juce::Label>("pitchLevelLabel", "EG Level");
    EGLevelLabel->setJustificationType(juce::Justification::centred);
    EGLevelLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*EGLevelLabel);

    EGRateLabel = std::make_unique<juce::Label>("pitchRateLabel", "EG Rate");
    EGRateLabel->setJustificationType(juce::Justification::centred);
    EGRateLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*EGRateLabel);


    addSlider(IDs::ampModeSens.op(num).name, "A MOD Sens", 13, outputControls);

    addSlider(IDs::keyVelocity.op(num).name, "Key Vel", 14, outputControls);

    addSlider(IDs::outputLevel.op(num).name, "Level", 15, outputControls);

    auto leftDepth = addSlider(IDs::lScaleDepth.op(num).name, "L Depth", 16, scalingControls);

    leftDepth->setTooltip(TRANS("Keyboard Scale Level Left Depth"));

    auto breakpoint = addSlider(IDs::breakpoint.op(num).name, "Breakpoint", 17, scalingControls,
                                juce::Slider::LinearHorizontal);

    breakpoint->setTooltip(TRANS("Scale Level Breakpoint"));

    auto rightDepth = addSlider(IDs::rScaleDepth.op(num).name, "R Depth", 18, scalingControls);

    rightDepth->setTooltip(TRANS("Keyboard Scale Level Right Depth"));

    auto rateScaling = addSlider(IDs::rateScaling.op(num).name, "Rate Scaling", 20, scalingControls);

    rateScaling->setTooltip(TRANS("Keyboard Rate Scaling"));

    opMode = std::make_unique<juce::ToggleButton>(IDs::mode.op(num).name);

    opMode->setExplicitFocusOrder(12);
    opMode->setButtonText("FIXED");
    opMode->setColour(juce::ToggleButton::textColourId, juce::Colour(0xFFF0F2F5));

    opMode->onClick = [this] { repaint(); };

    opMode->setLookAndFeel(&modeLookAndFeel);
    addAndMakeVisible(*opMode);
    binder->attach(opMode.get());

    addCurveCombo(IDs::lKeyScale.op(internalOp).name, "L CURVE", 19, leftCurve);
    addCurveCombo(IDs::rKeyScale.op(internalOp).name, "R CURVE", 21, rightCurve);
    

    auto details = std::make_unique<OperatorDisplay>(processor.parameters, num);

    operatorDisplay = details.get();
    binder->add(std::move(details));

    opSwitch = std::make_unique<OperatorSwitch>(IDs::on.op(num).name);
    opSwitch->setButtonText(opNum);
    opSwitch->setTitle("Operator switch");

    opSwitch->setLookAndFeel(&powerLookAndFeel);
    addAndMakeVisible(*opSwitch);
    binder->attach(opSwitch.get());

    envDisplay = std::make_unique<EnvDisplay>(processor.parameters, num);

    addAndMakeVisible(*envDisplay);

    vu = std::make_unique<VuMeter>();
    addAndMakeVisible(vu.get());

    contextMenuAdapter = std::make_unique<OperatorContextMenu>(processor, internalOp);

    addMouseListener(contextMenuAdapter.get(), true);
}

OperatorEditor::~OperatorEditor() {
   
    if (opSwitch)
        opSwitch->setLookAndFeel(nullptr);

    if (opMode)
        opMode->setLookAndFeel(nullptr);

    if (leftCurve != nullptr)
        leftCurve->setLookAndFeel(nullptr);

    if (rightCurve != nullptr)
        rightCurve->setLookAndFeel(nullptr);

    envDisplay = nullptr;
    vu = nullptr;
    contextMenuAdapter = nullptr;
}

//==============================================================================
void OperatorEditor::paint(juce::Graphics& g) {

    auto bounds = getLocalBounds().toFloat();
    
    g.setColour(DXLookNFeel::lightBackground);
    g.fillRoundedRectangle(bounds.reduced(1.0f), 7.0f);

    g.setColour(DXLookNFeel::borderOutline);
    const float strokeThickness = 1.0f;
    const float gap = 4.0f;
    const float pad = 10.0f;

    if (layoutGrid)
    {
         g.drawLine(std::floor(verticalDividerX) + 0.5f, bounds.getY() + pad,
                   std::floor(verticalDividerX) + 0.5f, bounds.getBottom() - pad,
                   strokeThickness);

         g.drawLine((float)bounds.getX() + pad, std::floor(leftDividerY) + 0.5f,
                    verticalDividerX - (gap * 0.5f), std::floor(leftDividerY) + 0.5f,
                    strokeThickness);

         float rightStartX = verticalDividerX + (gap * 0.5f);
         float rightEndX = (float)bounds.getRight();

         g.drawLine(rightStartX, std::floor(rightDivider2Y) + 0.5f, rightEndX - pad,
                    std::floor(rightDivider2Y) + 0.5f, strokeThickness);
    } 
    else 
    {

        g.drawLine(std::floor(verticalDividerX) + 0.5f, bounds.getY(),
                   std::floor(verticalDividerX) + 0.5f,
                   bounds.getBottom(), strokeThickness);
        g.drawLine(std::floor(leftDividerY) + 0.5f, bounds.getY() + pad,
                   std::floor(leftDividerY) + 0.5f,
                   bounds.getBottom() - pad, strokeThickness);
        g.drawLine(std::floor(rightDivider1Y) + 0.5f, bounds.getY() + pad,
                   std::floor(rightDivider1Y) + 0.5f,
                   bounds.getBottom() - pad, strokeThickness);
        g.drawLine(std::floor(rightDivider2Y) + 0.5f, bounds.getY() + pad,
                   std::floor(rightDivider2Y) + 0.5f,
                   bounds.getBottom() - pad, strokeThickness);
    }

}

void OperatorEditor::resized() {

    auto area1 = getLocalBounds().reduced(2);

    const int w = area1.getWidth();
    const int h = area1.getHeight();
    const int gap = 4;

    if (layoutGrid)
    {
        auto sectionLeft = area1.removeFromLeft(w * 0.5);
        verticalDividerX = (float)sectionLeft.getRight() + (gap * 0.5f);  // for paint
        area1.removeFromLeft(gap);

        auto sectionTop = sectionLeft.removeFromTop(h * 0.33);
        leftDividerY = (float)sectionTop.getBottom() + (gap * 0.5f);  // for paint
        sectionLeft.removeFromTop(gap);

        auto sectionPower = area1.removeFromTop(h * 0.18);
        auto sectionRight = area1.removeFromTop(h * 0.3);
        area1.removeFromTop(gap);
        rightDivider2Y = (float)sectionRight.getBottom() + (gap * 0.5f);  // for paint
        auto sectionBottom = area1;

        frequencySection->setBounds(sectionTop.removeFromLeft(w * 0.8));
        envelopeSection->setBounds(sectionLeft);
        outputSection->setBounds(sectionRight);
        scalingSection->setBounds(sectionBottom);

        opSwitch->setBounds(sectionPower.removeFromRight(w * 0.12));
        vu->setBounds(sectionPower.reduced(5, 15));

        auto freqArea = frequencySection->getBounds().reduced(4);
        auto freqCol = freqArea.getWidth() / 4;

        operatorDisplay->setBounds(freqArea.removeFromTop(16));

        for (int i = 0; i < (int)frequencyControls.size(); ++i) {
            auto cell = freqArea.removeFromLeft(freqCol);
            layoutControl(frequencyControls[i], cell);
        }

        opMode->setBounds(freqArea.removeFromLeft(freqCol).reduced(8));

        auto envArea = envelopeSection->getBounds().reduced(4);
        auto envCol = envArea.getWidth() / 4;

        envDisplay->setBounds(envArea.removeFromTop(h * 0.16f).reduced(5, 0));

        EGLevelLabel->setBounds(envArea.removeFromTop(12));
        EGRateLabel->setBounds(envArea.getX(), envArea.getBottom() - 12, envArea.getWidth(), 12);

        const int cellHeight = envArea.getHeight() / 2;

        for (int i = 0; i < 4; ++i) {
            if (envelopeControls[i * 2].label != nullptr)
                envelopeControls[i * 2].label->setText(juce::String(i + 1),
                                                       juce::dontSendNotification);
            auto levelCell = juce::Rectangle<int>(envArea.getX() + i * envCol, envArea.getY(),
                                                  envCol, cellHeight);
            layoutControl(envelopeControls[i * 2], levelCell);

            auto rateCell = juce::Rectangle<int>(envArea.getX() + i * envCol,
                                                 envArea.getY() + cellHeight, envCol, cellHeight);
            layoutControl(envelopeControls[i * 2 + 1], rateCell);
        }

        auto OutArea = outputSection->getBounds().reduced(4);
        auto OutCol = OutArea.getWidth() / 3;

        layoutControl(outputControls[0], OutArea.removeFromLeft(OutCol));
        layoutControl(outputControls[1], OutArea.removeFromLeft(OutCol));
        layoutControl(outputControls[2], OutArea);

        auto scaleArea = scalingSection->getBounds().reduced(4);
        auto scaleCol = scaleArea.getWidth() / 4;

        const int rowHeight = scaleArea.getHeight() / 2;
        const int sideWidth = scaleArea.getWidth() / 4;
        const int centerWidth = scaleArea.getWidth() - sideWidth * 2;

        auto topRow = scaleArea.removeFromTop(rowHeight);

        layoutControl(scalingControls[0], {topRow.removeFromLeft(scaleCol)});
        layoutControl(scalingControls[1], {topRow.removeFromLeft(scaleCol * 2)});
        layoutControl(scalingControls[2], {topRow.removeFromLeft(scaleCol)});

        auto bottomRow = scaleArea;

        auto leftSlot = bottomRow.removeFromLeft(scaleCol);
        leftCurveLabel->setBounds(leftSlot.removeFromBottom(14));
        leftCurve->setBounds(leftSlot.reduced(5, 10));

        layoutControl(scalingControls[3], bottomRow.removeFromLeft(scaleCol * 2));

        auto rightSlot = bottomRow.removeFromLeft(scaleCol);
        rightCurveLabel->setBounds(rightSlot.removeFromBottom(14));
        rightCurve->setBounds(rightSlot.reduced(5, 10));
    }
    else
    {
        opSwitch->setBounds(area1.removeFromLeft(w * 0.04));
        verticalDividerX = (float)area1.getX() + (gap * 0.5f);  // for paint
        const int colwidth = w * 0.96 / 6;

        frequencySection->setBounds(area1.removeFromLeft(colwidth));
        leftDividerY = (float)area1.getX() + (gap * 0.5f);  // for paint
        auto envLabelSec = area1.removeFromLeft(colwidth * 0.4);
        envelopeSection->setBounds(area1.removeFromLeft(colwidth));
        envDisplay->setBounds(area1.removeFromLeft(colwidth * 0.6).reduced(6, 18));
        rightDivider1Y = (float)area1.getX() + (gap * 0.5f);  // for paint
        scalingSection->setBounds(area1.removeFromLeft(colwidth * 2));
        rightDivider2Y = (float)area1.getX() + (gap * 0.5f);  // for paint
        outputSection->setBounds(area1.removeFromLeft(colwidth));

        auto freqArea = frequencySection->getBounds().reduced(4);
        auto freqCol = freqArea.getWidth() / 4;

        operatorDisplay->setBounds(freqArea.removeFromTop(16));

        for (int i = 0; i < (int)frequencyControls.size(); ++i) {
            auto cell = freqArea.removeFromLeft(freqCol);
            layoutControl(frequencyControls[i], cell);
        }

        opMode->setBounds(freqArea.removeFromLeft(freqCol).reduced(8));

        auto envArea = envelopeSection->getBounds();
        auto envCol = envArea.getWidth() / 4;

        EGLevelLabel->setBounds(envLabelSec.removeFromTop(h * 0.5));
        EGRateLabel->setBounds(envLabelSec);

        const int cellHeight = envArea.getHeight();

        for (int i = 0; i < 4; ++i) {
            if (envelopeControls[i * 2].label != nullptr)
                envelopeControls[i * 2].label->setText("", juce::dontSendNotification);
            auto levelCell = juce::Rectangle<int>(envArea.getX() + i * envCol, envArea.getY(),
                                                  envCol, cellHeight*0.55);
            layoutControl(envelopeControls[i * 2], levelCell);

            auto rateCell = juce::Rectangle<int>(envArea.getX() + i * envCol,
                                                 envArea.getY() + cellHeight*0.55, envCol, cellHeight*0.45);
            layoutControl(envelopeControls[i * 2 + 1], rateCell);
        }

        auto OutArea = outputSection->getBounds().reduced(8, 4);
        vu->setBounds(OutArea.removeFromTop(10).reduced(5,0));
        auto OutCol = OutArea.getWidth() / 3;

        layoutControl(outputControls[0], OutArea.removeFromLeft(OutCol));
        layoutControl(outputControls[1], OutArea.removeFromLeft(OutCol));
        layoutControl(outputControls[2], OutArea);

        auto scaleArea = scalingSection->getBounds().reduced(4, 8);
        auto scaleCol = scaleArea.getWidth() / 8;

        layoutControl(scalingControls[0], {scaleArea.removeFromLeft(scaleCol)});
        layoutControl(scalingControls[1], {scaleArea.removeFromLeft(scaleCol * 2)});
        layoutControl(scalingControls[2], {scaleArea.removeFromLeft(scaleCol)});

        auto leftSlot = scaleArea.removeFromLeft(scaleCol);
        leftCurveLabel->setBounds(leftSlot.removeFromBottom(14));
        leftCurve->setBounds(leftSlot.reduced(5, 10));

        layoutControl(scalingControls[3], scaleArea.removeFromLeft(scaleCol * 2));

        auto rightSlot = scaleArea.removeFromLeft(scaleCol);
        rightCurveLabel->setBounds(rightSlot.removeFromBottom(14));
        rightCurve->setBounds(rightSlot.reduced(5, 10));
    }
}

//void OperatorEditor::layoutControl(Control& control, juce::Rectangle<int> area) {
//    if (control.slider == nullptr || control.label == nullptr)
//        return;
//
//    auto labelHeight = juce::jmin(11, area.getHeight() / 4);
//    control.label->setBounds(area.removeFromBottom(labelHeight));
//    control.slider->setBounds(area.reduced(1));
//}

void OperatorEditor::layoutControl(Control& control, juce::Rectangle<int> area) {
    if (control.slider == nullptr || control.label == nullptr)
        return;

    if (control.label->getText().isNotEmpty()) {
        auto labelHeight = juce::jmin(11, area.getHeight() / 4);
        control.label->setBounds(area.removeFromBottom(labelHeight));
    } else {
        control.label->setBounds(0, 0, 0, 0);
    }

    control.slider->setBounds(area.reduced(1));
}

void OperatorEditor::drawSection(juce::Graphics& g, juce::Rectangle<float> area,
                                 const juce::String& title) {
    const auto panel = juce::Colour(0xFF222731);
    const auto border = juce::Colour(0xFF303846);
    const auto accent = juce::Colour(0xFF39C6B0);
    const auto secondary = juce::Colour(0xFF9299A5);

    g.setColour(panel);
    g.fillRoundedRectangle(area, 5.0f);

    g.setColour(accent);
    g.fillRoundedRectangle(area.getX() + 7.0f, area.getY() + 8.0f, 2.0f, 9.0f, 1.0f);
}

juce::Slider* OperatorEditor::addSlider(const juce::String& parameter,
                                        const juce::String& labelText, int focusOrder,
                                        std::vector<Control>& group,
                                        juce::Slider::SliderStyle style) {
    auto slider = std::make_unique<juce::Slider>();

    slider->setName(parameter);
    slider->setSliderStyle(style);

    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    slider->setExplicitFocusOrder(focusOrder);

    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF39C6B0));

    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF303846));

    slider->setColour(juce::Slider::thumbColourId, juce::Colour(0xFF39C6B0));


    auto* sliderPtr = slider.get();

    binder->addAndAttach(std::move(slider));

    auto label = std::make_unique<juce::Label>();


    label->setText(labelText, juce::dontSendNotification);
    label->setJustificationType(juce::Justification::centred);

    label->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));


    auto* labelPtr = label.get();

    binder->add(std::move(label));

    group.push_back({sliderPtr, labelPtr});

    return sliderPtr;
}


void OperatorEditor::addCurveCombo(const juce::String& parameter, const juce::String& labelText,
                                   int focusOrder, juce::ComboBox*& destination) {
    auto combo = std::make_unique<juce::ComboBox>();
    combo->setName(parameter);
    combo->setExplicitFocusOrder(focusOrder);

    bool isRightSide =
        parameter.containsIgnoreCase("rKeyScale") || parameter.containsIgnoreCase("right");

    combo->setLookAndFeel(isRightSide ? &rightCurveLaf : &leftCurveLaf);
    destination = combo.get();

    binder->attach(combo.get());
    binder->add(std::move(combo));

    auto* menu = destination->getRootMenu();
    menu->clear();

    static const char* labels[] = {"-LIN", "-EXP", "+EXP", "+LIN"};

    for (int i = 0; i < 4; ++i) {
        menu->addCustomItem(i + 1, std::make_unique<CurveMenuItem>(i, labels[i], isRightSide));
    }

    if (destination->getSelectedId() == 0)
        destination->setSelectedId(1, juce::dontSendNotification);

    
    auto label = std::make_unique<juce::Label>();
    label->setText(labelText, juce::dontSendNotification);
    label->setJustificationType(juce::Justification::centred);
    label->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*label);

    if (isRightSide)
        rightCurveLabel = std::move(label);
    else
        leftCurveLabel = std::move(label);

    binder->add(std::move(label));
    destination->repaint();
}

void OperatorEditor::updateGain(float v) {
    if (vu != nullptr)
        vu->setLevel(v);
}

void OperatorEditor::updateDisplay() {}

void OperatorEditor::updateEnvPos(char pos) {
    envDisplay->vPos = pos;
    envDisplay->repaint();
}

void OperatorEditor::mouseDown(const MouseEvent& event) {}

void OperatorEditor::toggleOpSwitch() {
    opSwitch->setToggleState(!opSwitch->getToggleState(), dontSendNotification);
}


//==============================================================================
#if 0
#endif
