
#include "GlobalEditor.h"
#include "DXLookNFeel.h"
#include "parameter/Model.h"
#include "ui/util/ContextMenuAdapter.h"
#include "window/AboutBox.h"
#include "../debugger/ValueTreeDebugger.h"


#ifdef IMPLEMENT_MidiMonitor
class MidiMonitor : public Component {
    SysexComm *midi;
    Image light;
    int imageHeight;
    int imageHeight2;
    int imageWidth;

    SharedResourcePointer<DXLookNFeel> lookAndFeel;
public:
    MidiMonitor(SysexComm *sysexComm) {
        midi = sysexComm;
        imageHeight = light.getHeight();
        imageHeight2 = imageHeight / 2;
        imageWidth = light.getWidth();

        TRACE("WARNING! This functionality is a candidate for deprecation/obsolescence!");
    }

    void paint(Graphics &g) {
        if ( ! (midi->isInputActive() || midi->isOutputActive() ) )
            return;
        g.setColour(Colours::white);

        if ( midi->isInputActive() ) {
            g.drawSingleLineText("DX7 IN", 24, 18);
            g.drawImage(light, 0, 0, imageWidth, imageHeight2, 0, midi->inActivity ? imageHeight2 : 0, imageWidth, imageHeight2);
            midi->inActivity = false;
        }

        if ( midi->isOutputActive() ) {
            g.drawSingleLineText("DX7 OUT", 24, 36);
            g.drawImage(light, 0, 18, imageWidth, imageHeight2, 0, midi->outActivity ? imageHeight2 : 0, imageWidth, imageHeight2);
            midi->outActivity = false;
        }
    }
};
#endif 


class GlobalEditorContextMenu : public ContextMenuAdapter {
public:
    GlobalEditorContextMenu(DexedAudioProcessor &processor) : ContextMenuAdapter(processor) {
    }

};

//==============================================================================
GlobalEditor::GlobalEditor(DexedAudioProcessor& processor, juce::Component* parent)
    : processor(processor) {
    binder = std::make_unique<AudioComponentContainer>(*this, processor.parameters);

    // MASTER
    addRotary(IDs::masterTuneAdj.name, "Tune", 5, masterControls, true);
    addRotary(IDs::cutoff.name, "Cutoff", 6, masterControls, true);
    addRotary(IDs::resonance.name, "Reso", 7, masterControls, true);
    addRotary(IDs::output.name, "Lavel", 8, masterControls, true);
    addRotary(IDs::transpose.name, "Transpose", 9, masterControls, true);

    monoModeLabel = std::make_unique<juce::Label>("monoLabel", "Monophonic");
    monoModeLabel->setJustificationType(juce::Justification::centred);
    monoModeLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*monoModeLabel);

    lfoSyncLabel = std::make_unique<juce::Label>("lfoSyncLabel", "LFO Key Sync");
    lfoSyncLabel->setJustificationType(juce::Justification::centred);
    lfoSyncLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*lfoSyncLabel);

    oscSyncLabel = std::make_unique<juce::Label>("oscSyncLabel", "OSC Key Sync");
    oscSyncLabel->setJustificationType(juce::Justification::centred);
    oscSyncLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*oscSyncLabel);

    pitchLevelLabel = std::make_unique<juce::Label>("pitchLevelLabel", "Pitch EG Level");
    pitchLevelLabel->setJustificationType(juce::Justification::centred);
    pitchLevelLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*pitchLevelLabel);

    pitchRateLabel = std::make_unique<juce::Label>("pitchRateLabel", "Pitch EG Rate");
    pitchRateLabel->setJustificationType(juce::Justification::centred);
    pitchRateLabel->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));
    addAndMakeVisible(*pitchRateLabel);



    addRotary(IDs::algorithm.name, "Algorithm", 11, algorithmControls);
    addRotary(IDs::feedback.name, "Feedback", 12, algorithmControls);
    algoDisplay = std::make_unique<AlgoDisplay>(processor.parameters);
    addAndMakeVisible(*algoDisplay);

    addRotary(IDs::lfoRate.name, "Speed", 14, lfoControls);
    addRotary(IDs::lfoDelay.name, "Delay", 15, lfoControls);
    addRotary(IDs::lfoAmpDepth.name, "AMD", 19, lfoControls);
    addRotary(IDs::lfoPmDepth.name, "PMD", 18, lfoControls);
    addRotary(IDs::pitchModSens.name, "P Mod Sens", 17, lfoControls);

    const int levelFocus[] = {21, 23, 25, 27};
    const int rateFocus[] = {23, 24, 26, 28};

    for (int i = 0; i < 4; ++i) {
        addRotary(IDs::pitchEgLevel.idx(i).name, "" + juce::String(i + 1), levelFocus[i],pitchControls);
    }

    for (int i = 0; i < 4; ++i) {
        addRotary(IDs::pitchEgRate.idx(i).name, "", rateFocus[i], pitchControls);
    }

    lfoType = std::make_unique<juce::ComboBox>();
    lfoType->setName(IDs::lfoWaveform.name);
    lfoType->setExplicitFocusOrder(13);
    lfoType->setLookAndFeel(&lfoComboLaf);

    binder->attach(lfoType.get());
    addAndMakeVisible(*lfoType);

    auto* menu = lfoType->getRootMenu();
    menu->clear();

    static const char* lfoLabels[] = {"TRI", "SAW D", "SAW U", "SQR", "SIN", "S/H"};
    for (int i = 0; i < 6; ++i) {
        menu->addCustomItem(i + 1, std::make_unique<LfoMenuItem>(i, lfoLabels[i]));
    }

    if (lfoType->getSelectedId() == 0)
        lfoType->setSelectedId(1, juce::dontSendNotification);

    lfoType->repaint();

    monoMode = std::make_unique<juce::ToggleButton>(IDs::monoMode.name);

    monoMode->setExplicitFocusOrder(10);
    monoMode->setTitle("Mono mode");
    binder->attach(monoMode.get());
    addAndMakeVisible(*monoMode);

    lfoSync = std::make_unique<juce::ToggleButton>(IDs::lfoKeySync.name);

    lfoSync->setExplicitFocusOrder(16);
    lfoSync->setTitle("LFO key sync");
    binder->attach(lfoSync.get());
    addAndMakeVisible(*lfoSync);

    oscSync = std::make_unique<juce::ToggleButton>(IDs::oscKeySync.name);

    oscSync->setExplicitFocusOrder(20);
    oscSync->setTitle("Oscillator key sync");
    binder->attach(oscSync.get());
    addAndMakeVisible(*oscSync);

    pitchEnvDisplay = std::make_unique<PitchEnvDisplay>(processor.parameters);

    addAndMakeVisible(*pitchEnvDisplay);


#ifdef IMPLEMENT_MidiMonitor
    midiMonitor = std::make_unique<MidiMonitor>(&processor.sysexComm);

    addAndMakeVisible(*midiMonitor);
#endif

    contextMenuAdapter = std::make_unique<GlobalEditorContextMenu>(processor);

    addMouseListener(contextMenuAdapter.get(), true);

    setTitle("Global Parameters");
    setFocusContainerType(juce::Component::FocusContainerType::focusContainer);

    setWantsKeyboardFocus(true);

}

GlobalEditor::~GlobalEditor() {
    if (binder)
        binder->freeAttachments();

    if (lfoType != nullptr)
        lfoType->setLookAndFeel(nullptr);

    contextMenuAdapter = nullptr;

    monoMode = nullptr;
    lfoSync = nullptr;
    oscSync = nullptr;

    pitchEnvDisplay = nullptr;
    algoDisplay = nullptr;

    lfoType = nullptr;
}

void GlobalEditor::updateDisplay() {
    repaint();
}

void GlobalEditor::updatePitchPos(int pos) {
    if (pitchEnvDisplay) {
        pitchEnvDisplay->vPos = pos;
        pitchEnvDisplay->repaint();
    }
}

//==============================================================================
void GlobalEditor::paint(juce::Graphics& g) {
    drawSection(g, masterArea, "MASTER");
    drawSection(g, filterArea, "FILTER");
    drawSection(g, algorithmArea, "ALGORITHM");
    drawSection(g, lfoArea, "LFO");
    drawSection(g, pitchArea, "PITCH ENVELOPE");
}

void GlobalEditor::resized() {
    auto bounds = getLocalBounds().reduced(4);
    const int w = bounds.getWidth();
    const int h = bounds.getHeight();

    constexpr int gap = 4;

    const int availableWidth = w - gap * 4;

    const int masterWidth = availableWidth * 0.2;
    const int algorithmWidth = availableWidth * 0.35;
    const int lfoWidth = availableWidth * 0.25;

    masterArea = bounds.removeFromLeft(masterWidth);
    bounds.removeFromLeft(gap);

    algorithmArea = bounds.removeFromLeft(algorithmWidth);
    bounds.removeFromLeft(gap);

    lfoArea = bounds.removeFromLeft(lfoWidth);
    bounds.removeFromLeft(gap);

    pitchArea = bounds;

    // MASTER
    auto masterContent = masterArea.reduced(5);
    masterContent.reduce(0, h * 0.1);
    layoutControls(masterControls, masterContent, 3, 2);

    const int columns = 3;
    const int rows = 2;
    const int cellWidth = masterContent.getWidth() / columns;
    const int cellHeight = masterContent.getHeight() / rows;

    const int col = 2;
    const int row = 1; 

    auto monoCell =
        juce::Rectangle<int>(masterContent.getX() + col * cellWidth,
                             masterContent.getY() + row * cellHeight, cellWidth, cellHeight);

    const int labelHeight = juce::jmin(16, cellHeight / 4);
    monoModeLabel->setBounds(monoCell.removeFromBottom(labelHeight));
    monoMode->setBounds(monoCell.reduced(6));


    // ALGORITHM
    auto algorithmContent = algorithmArea.reduced(5);
    auto algoDisplayArea = algorithmContent.removeFromLeft(algorithmContent.getWidth() * 0.7);
    algoDisplay->setBounds(algoDisplayArea);
    algorithmContent.reduce(0, h * 0.1);
    layoutControls(algorithmControls, algorithmContent, 1,2);

    
    // LFO
    auto lfoContent = lfoArea.reduced(5);
    lfoType->setBounds(lfoContent.removeFromTop(h * 0.25));

    auto lfoBottom = lfoContent.removeFromBottom(h*0.25);
    auto syncWidth = lfoBottom.getWidth() / 2;

    auto lfoCol = lfoBottom.removeFromLeft(syncWidth);
    lfoSyncLabel->setBounds(lfoCol.removeFromBottom(labelHeight));
    lfoSync->setBounds(lfoCol);

    auto oscCol = lfoBottom;
    oscSyncLabel->setBounds(oscCol.removeFromBottom(labelHeight));
    oscSync->setBounds(oscCol);

    layoutControls(lfoControls, lfoContent.reduced(0,4), 5);



    // PITCH
    auto pitchContent = pitchArea.reduced(5);
    pitchEnvDisplay->setBounds(pitchContent.removeFromTop(h*0.25));
    pitchLevelLabel->setBounds(pitchContent.removeFromTop(16));
    pitchRateLabel->setBounds(pitchContent.getX(), pitchContent.getBottom() - labelHeight, pitchContent.getWidth(), labelHeight);
    layoutControls(pitchControls, pitchContent, 4, 2);
   

#ifdef IMPLEMENT_MidiMonitor
    midiMonitor->setBounds(masterArea.getX() + 5, masterArea.getY() + 20,
                           masterArea.getWidth() - 10, 25);
#endif
}


void GlobalEditor::addRotary(const juce::String& parameter, const juce::String& labelText,
                             int focusOrder, std::vector<SliderControl>& group, bool showValue) {
    auto slider = std::make_unique<juce::Slider>();

    slider->setName(parameter);
    slider->setSliderStyle(juce::Slider::RotaryVerticalDrag);

    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    slider->setExplicitFocusOrder(focusOrder);
    slider->setRotaryParameters(juce::MathConstants<float>::pi * 1.25f, juce::MathConstants<float>::pi * 2.75f, true);


    auto* sliderPtr = slider.get();

    binder->addAndAttach(std::move(slider));

    auto label = std::make_unique<juce::Label>();
    label->setText(labelText, juce::dontSendNotification);
    label->setJustificationType(juce::Justification::centred);
    label->setColour(juce::Label::textColourId, juce::Colour(0xFF9299A5));

    auto* labelPtr = label.get();
    addAndMakeVisible(labelPtr);

    labels.push_back(std::move(label));

    group.push_back({sliderPtr, labelPtr});
}

void GlobalEditor::layoutControls(std::vector<SliderControl>& controls, juce::Rectangle<int> area,
                                  int columns, int rows) {
    if (controls.empty() || columns <= 0 || rows <= 0)
        return;

    const int cellWidth = area.getWidth() / columns;
    const int cellHeight = area.getHeight() / rows;

    for (int i = 0; i < (int)controls.size(); ++i) {
        const int column = i % columns;
        const int row = i / columns;

        if (row >= rows)
            break;

        auto cell = juce::Rectangle<int>(area.getX() + column * cellWidth,
                                         area.getY() + row * cellHeight, cellWidth, cellHeight);

        const int labelHeight = juce::jmin(16, cellHeight / 4);

        controls[i].label->setBounds(cell.removeFromBottom(labelHeight));
        controls[i].slider->setBounds(cell.reduced(2));
    }
}

void GlobalEditor::drawSection(juce::Graphics& g, juce::Rectangle<int> area,
                               const juce::String& title) {
    if (area.isEmpty())
        return;

    auto bounds = area.toFloat();

    g.setColour(DXLookNFeel::lightBackground);
    g.fillRoundedRectangle(bounds, 8.0f);
}




#if 0

#endif
