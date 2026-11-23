
#include "../../debugger/Logger.h"
#include "parameter/Model.h"

#include "../PluginEditor.h"
#include "ParamDialog.h"


//==============================================================================
ParamDialog::ParamDialog (DexedApvts &apvts, DexedAudioProcessor &proc) : apvts_(apvts), proc_(proc), attachments_(*this, apvts) {


    pitchRangeDn.reset(new juce::Slider(IDs::pitchRangeDown.name));
    addAndMakeVisible(pitchRangeDn.get());
    pitchRangeDn->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pitchRangeDn->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    pitchRangeDn->addListener(this);
    pitchRangeDn->setBounds(264, 16, 72, 24);
    attachments_.attach(pitchRangeDn.get());

    pitchStep.reset(new juce::Slider(IDs::pitchStep.name));
    addAndMakeVisible(pitchStep.get());
    pitchStep->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pitchStep->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    pitchStep->addListener(this);
    pitchStep->setBounds(264, 56, 72, 24);
    attachments_.attach(pitchStep.get());

    sysexIn.reset (new juce::ComboBox ("sysexIn"));
    addAndMakeVisible (sysexIn.get());
    sysexIn->setExplicitFocusOrder (7);
    sysexIn->setEditableText (false);
    sysexIn->setJustificationType (juce::Justification::centredLeft);
    sysexIn->setTextWhenNothingSelected (juce::String());
    sysexIn->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    sysexIn->addListener (this);

    sysexIn->setBounds (104, 244, 224, 24);

    sysexChl.reset (new juce::Slider ("sysexChl"));
    addAndMakeVisible (sysexChl.get());
    sysexChl->setExplicitFocusOrder (9);
    sysexChl->setRange (1, 16, 1);
    sysexChl->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sysexChl->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sysexChl->addListener (this);

    sysexChl->setBounds (252, 320, 72, 24);

    engineType.reset(new juce::ComboBox(IDs::engineType.name));
    addAndMakeVisible(engineType.get());
    engineType->setJustificationType(juce::Justification::centredLeft);
    engineType->setBounds(160, 188, 168, 24);
    attachments_.attach(engineType.get());

    //showKeyboard.reset (new LightedToggleButton ("showKeyboard"));
    //addAndMakeVisible (showKeyboard.get());
    //showKeyboard->setExplicitFocusOrder (4);
    //showKeyboard->setButtonText (juce::String());
    //showKeyboard->addListener (this);

    //showKeyboard->setBounds (264, 96, 56, 24);

    auto whlRange = std::make_unique<juce::Slider>(IDs::modWheel.name);
    whlRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    whlRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    whlRange->setBounds (448, 16, 72, 24);
    whlRange->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(whlRange));

    auto whlPitch = std::make_unique<LightedToggleButton>(IDs::modWheelPitch.name);
    whlPitch->setBounds (528, 16, 56, 24);
    whlPitch->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(whlPitch));

    auto whlAmp = std::make_unique<LightedToggleButton>(IDs::modWheelAmp.name);
    whlAmp->setBounds (584, 16, 56, 24);
    whlAmp->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(whlAmp));

    auto whlEg = std::make_unique<LightedToggleButton>(IDs::modWheelEgBias.name);
    whlEg->setBounds (640, 16, 56, 24);
    whlEg->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(whlEg));

    auto ftRange = std::make_unique<juce::Slider>(IDs::modFoot.name);
    ftRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    ftRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    ftRange->setBounds (448, 56, 72, 24);
    ftRange->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(ftRange));

    auto ftPitch = std::make_unique<LightedToggleButton>(IDs::modFootPitch.name);
    ftPitch->setBounds (528, 56, 56, 24);
    ftPitch->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(ftPitch));

    auto ftAmp = std::make_unique<LightedToggleButton>(IDs::modFootAmp.name);
    ftAmp->setBounds (584, 56, 56, 24);
    ftAmp->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(ftAmp));

    auto ftEg = std::make_unique<LightedToggleButton>(IDs::modFootEgBias.name);
    ftEg->setBounds (640, 56, 56, 24);
    ftEg->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(ftEg));

    auto brRange = std::make_unique<juce::Slider>(IDs::modBreath.name);
    brRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    brRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    brRange->setBounds (448, 96, 72, 24);
    brRange->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(brRange));

    auto brPitch = std::make_unique<LightedToggleButton>(IDs::modBreathPitch.name);
    brPitch->setBounds (528, 96, 56, 24);
    brPitch->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(brPitch));

    auto brAmp = std::make_unique<LightedToggleButton>(IDs::modBreathAmp.name);
    brAmp->setBounds (584, 96, 56, 24);
    brAmp->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(brAmp));

    auto brEg = std::make_unique<LightedToggleButton>(IDs::modBreathEgBias.name);
    brEg->setBounds (640, 96, 56, 24);
    brEg->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(brEg));

    auto atRange = std::make_unique<juce::Slider>(IDs::modAftertouch.name);
    atRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    atRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    atRange->setBounds (448, 136, 72, 24);
    atRange->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(atRange));

    auto atPitch = std::make_unique<LightedToggleButton>(IDs::modAftertouchPitch.name);
    atPitch->setBounds (528, 136, 56, 24);
    atPitch->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(atPitch));

    auto atAmp = std::make_unique<LightedToggleButton>(IDs::modAftertouchAmp.name);
    atAmp->setBounds (584, 136, 56, 24);
    atAmp->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(atAmp));

    auto atEg = std::make_unique<LightedToggleButton>(IDs::modAftertouchEgBias.name);
    atEg->setBounds (640, 136, 56, 24);
    atEg->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(atEg));

    sclButton.reset (new juce::TextButton ("scl button"));
    addAndMakeVisible (sclButton.get());
    sclButton->setExplicitFocusOrder (26);
    sclButton->setButtonText (TRANS ("SCL"));
    sclButton->addListener (this);

    sclButton->setBounds (448, 205, 56, 30);

    kbmButton.reset (new juce::TextButton ("kbm button"));
    addAndMakeVisible (kbmButton.get());
    kbmButton->setExplicitFocusOrder (27);
    kbmButton->setButtonText (TRANS ("KBM"));
    kbmButton->addListener (this);

    kbmButton->setBounds (512, 205, 56, 30);

    showTunButton.reset (new juce::TextButton ("show tuning button"));
    addAndMakeVisible (showTunButton.get());
    showTunButton->setExplicitFocusOrder (28);
    showTunButton->setButtonText (TRANS ("Show"));
    showTunButton->addListener (this);

    showTunButton->setBounds (576, 205, 48, 30);

    resetTuningButton.reset (new juce::TextButton ("reset tuning button"));
    addAndMakeVisible (resetTuningButton.get());
    resetTuningButton->setExplicitFocusOrder (29);
    resetTuningButton->setButtonText (TRANS ("Reset"));
    resetTuningButton->addListener (this);

    resetTuningButton->setBounds (632, 205, 48, 30);

    auto transposeScale = std::make_unique<LightedToggleButton>(IDs::transposeTuningScale.name);
    transposeScale->setBounds(576, 240, 56, 30);
    transposeScale->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(transposeScale));

    mpePBRange.reset (new juce::Slider ("mpePBRange"));
    addAndMakeVisible (mpePBRange.get());
    mpePBRange->setExplicitFocusOrder (32);
    mpePBRange->setRange (0, 96, 1);
    mpePBRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    mpePBRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    mpePBRange->addListener (this);

    mpePBRange->setBounds (616, 319, 72, 24);

    mpeEnabled.reset (new LightedToggleButton ("mpeEnabled"));
    addAndMakeVisible (mpeEnabled.get());
    mpeEnabled->setExplicitFocusOrder (31);
    mpeEnabled->setButtonText (juce::String());
    mpeEnabled->addListener (this);

    mpeEnabled->setBounds (456, 320, 48, 30);

    transposeHelp.reset(new juce::TextButton("Help"));
    addAndMakeVisible(transposeHelp.get());
    transposeHelp->addListener(this);

    transposeHelp->setBounds(500, 245, 50, 20);

    pitchRangeUp.reset(new juce::Slider(IDs::pitchRangeUp.name));
    addAndMakeVisible(pitchRangeUp.get());
    pitchRangeUp->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    pitchRangeUp->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    pitchRangeUp->addListener(this);
    pitchRangeUp->setBounds(168, 16, 72, 24);
    attachments_.attach(pitchRangeUp.get());

    auto glissando = std::make_unique<LightedToggleButton>(IDs::glissando.name);
    glissando->setBounds(576, 276, 56, 30);
    glissando->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(glissando));

    auto portamentoTm = std::make_unique<juce::Slider>(IDs::portamentoTm.name);
    portamentoTm->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    portamentoTm->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    portamentoTm->setBounds(504, 276, 72, 24);
    portamentoTm->setWantsKeyboardFocus(true);
    attachments_.addAndAttach(std::move(portamentoTm));


    profileSelector.reset(new juce::ComboBox("profileSelector"));
    addAndMakeVisible(profileSelector.get());
    profileSelector->addListener(this);
    profileSelector->setBounds(100, 378, 450, 24);

    storeButton.reset(new juce::TextButton("storeButton"));
    addAndMakeVisible(storeButton.get());
    storeButton->setButtonText("Store");
    storeButton->addListener(this);
    storeButton->setBounds(558, 378, 130, 24);


    setSize (710, 414);


    //[Constructor] You can add your own custom stuff here..
    pitchRangeUp->setEnabled(pitchStep->getValue() == 0);
    pitchRangeDn->setEnabled(pitchStep->getValue() == 0);

    StringArray input;
    input.add("None");
    // TODO: refactor this since JUCE 8
    //input.addArray(MidiInput::getDevices());
    sysexIn->addItemList(input, 2);

    if ( JUCEApplication::isStandaloneApp() ) {
        sysexIn->setVisible(false);
    }

    // ACCESSIBLITY (modulation component titles set by AudioComponentContainer::attach)
    pitchRangeUp->setTitle("Pitch Bend Range Up");
    pitchRangeDn->setTitle("Pitch Bend Range Down");
    pitchStep->setTitle("Pitch Bend Step");
    sysexIn->setTitle("Sysex Input");
    sysexChl->setTitle("Sysex Channel");
    engineType->setTitle("Engine Resolution");
    //showKeyboard->setTitle("Show Keyboard");
    sclButton->setTitle("Scale Button");
    kbmButton->setTitle("Keyboard Mapping Button");
    showTunButton->setTitle("Show Tuning Button");
    resetTuningButton->setTitle("Reset Tuning Button");
    mpePBRange->setTitle("MPE Pitch Bend Range");
    mpeEnabled->setTitle("MPE Enabled");
    transposeHelp->setTitle("Transpose Help");
    //scalingFactor->setTitle("Scaling Factor");
    profileSelector->setTitle("Profile");
    storeButton->setTitle("Store Profile");

    pitchRangeUp->setWantsKeyboardFocus(true);
    pitchRangeDn->setWantsKeyboardFocus(true);
    pitchStep->setWantsKeyboardFocus(true);
    sysexIn->setWantsKeyboardFocus(true);
    sysexChl->setWantsKeyboardFocus(true);
    engineType->setWantsKeyboardFocus(true);
    //showKeyboard->setWantsKeyboardFocus(true);
    sclButton->setWantsKeyboardFocus(true);
    kbmButton->setWantsKeyboardFocus(true);
    showTunButton->setWantsKeyboardFocus(true);
    resetTuningButton->setWantsKeyboardFocus(true);
    mpePBRange->setWantsKeyboardFocus(true);
    mpeEnabled->setWantsKeyboardFocus(true);
    transposeHelp->setWantsKeyboardFocus(true);
    //scalingFactor->setWantsKeyboardFocus(true);
    profileSelector->setWantsKeyboardFocus(true);
    storeButton->setWantsKeyboardFocus(true);

    refreshProfileList();
    setWantsKeyboardFocus(true);
    startTimer(100);
    //[/Constructor]
}

ParamDialog::~ParamDialog() {
    //[Destructor_pre]. You can add your own custom destruction code here..
    attachments_.freeAttachments();
    //[/Destructor_pre]

    pitchRangeDn = nullptr;
    pitchStep = nullptr;
    sysexIn = nullptr;
    sysexChl = nullptr;
    engineType = nullptr;
    //showKeyboard = nullptr;
    sclButton = nullptr;
    kbmButton = nullptr;
    showTunButton = nullptr;
    resetTuningButton = nullptr;
    mpePBRange = nullptr;
    mpeEnabled = nullptr;
    transposeHelp = nullptr;
    pitchRangeUp = nullptr;
    //scalingFactor = nullptr;
    profileSelector = nullptr;
    storeButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    stopTimer();
    //[/Destructor]
}

//==============================================================================
void ParamDialog::paint (juce::Graphics& g) {


    //[UserPrePaint] Add your own custom painting code here..
    juce::Font font(juce::FontOptions().withHeight(15.00f).withStyle("Regular"));

    //[/UserPrePaint]

    g.fillAll (DXLookNFeel::lightBackground); 
    
    auto bounds = getLocalBounds().toFloat();
    g.setColour(DXLookNFeel::secondaryText);
    g.drawRoundedRectangle(bounds.reduced(0.5f), 8.0f, 1.0f);

    {
        int x = 20, y = 16, width = 276, height = 23;
        juce::String text (TRANS ("Pitch Bend Range"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 20, y = 56, width = 276, height = 23;
        juce::String text (TRANS ("Pitch Bend Step"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 20, y = 319, width = 245, height = 23;
        juce::String text (TRANS ("DX7 Channel"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 20, y = 190, width = 276, height = 23;
        juce::String text (TRANS ("Engine Resolution"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 22, y = 174, width = 306, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    } {
        int x = 22, y = 227, width = 306, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    } 
    //{
    //    int x = 20, y = 96, width = 276, height = 23;
    //    juce::String text (TRANS ("Show Keyboard"));
    //    juce::Colour fillColour = juce::Colours::white;
    //    //[UserPaintCustomArguments] Customize the painting arguments here..
    //    //[/UserPaintCustomArguments]
    //    g.setColour (fillColour);
    //    g.setFont (font);
    //    g.drawText (text, x, y, width, height,
    //                juce::Justification::centredLeft, true);
    //} 
    {
        int x = 352, y = 12, width = 1, height = 349;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    } {
        int x = 368, y = 16, width = 276, height = 23;
        juce::String text (TRANS ("Wheel"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 368, y = 96, width = 276, height = 23;
        juce::String text (TRANS ("Breath"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 368, y = 56, width = 276, height = 23;
        juce::String text (TRANS ("Foot"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 368, y = 136, width = 276, height = 23;
        juce::String text (TRANS ("After Touch"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 528, y = 163, width = 48, height = 23;
        juce::String text (TRANS ("PITCH"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    } {
        int x = 584, y = 163, width = 48, height = 23;
        juce::String text (TRANS ("AMP"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    } {
        int x = 640, y = 163, width = 48, height = 23;
        juce::String text (TRANS ("EG BIAS"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    } {
        int x = 376, y = 194, width = 325, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    } {
        int x = 371, y = 208, width = 276, height = 25;
        juce::String text (TRANS ("Tuning"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 371, y = 242, width = 157, height = 25;
        juce::String text (TRANS ("Transposition 12 as:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 368, y = 310, width = 328, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    } {
        int x = 368, y = 318, width = 276, height = 27;
        juce::String text (TRANS ("MPE"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 528, y = 318, width = 119, height = 27;
        juce::String text (TRANS ("Bend Range"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 548, y = 242, width = 37, height = 25;
        juce::String text (TRANS ("12"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 628, y = 242, width = 45, height = 25;
        juce::String text (TRANS ("SCL"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 147, y = 16, width = 20, height = 23;
        juce::String text (TRANS ("up"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 240, y = 16, width = 20, height = 23;
        juce::String text (TRANS ("dn"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } 
    {
        int x = 371, y = 277, width = 184, height = 23;
        juce::String text (TRANS ("Portamento Rate"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    } {
        int x = 628, y = 274, width = 75, height = 27;
        juce::String text (TRANS ("Glissando"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (font);
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    //[UserPaint] Add your own custom painting code here..
    if ( ! JUCEApplication::isStandaloneApp() ) {
        g.setColour (Colours::white);
        g.setFont (font);
        g.drawText (translate("DX7 In"),
                    20, 245, 131, 23,
                    Justification::centredLeft, true);

    }

    g.setColour (Colours::black);
    g.fillRect (0, 370, 710, 1);

    g.setColour (Colours::white);
    g.setFont (font);
    g.drawText (translate("Profile:"), 20, 378, 76, 24, Justification::centredLeft, true);
    //[/UserPaint]
}

void ParamDialog::resized() {
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ParamDialog::sliderValueChanged (juce::Slider* sliderThatWasMoved) {
    //[UsersliderValueChanged_Pre]
    bool handled = false;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == pitchStep.get()) {
        //[UserSliderCode_pitchStep] -- add your slider handling code here..
        pitchRangeUp->setEnabled(pitchStep->getValue() == 0);
        pitchRangeDn->setEnabled(pitchStep->getValue() == 0);
        //[/UserSliderCode_pitchStep]
    }

    //[UsersliderValueChanged_Post]
    if( ! handled )
        general_callback_(this);
    //[/UsersliderValueChanged_Post]
}

void ParamDialog::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) {
    //[UsercomboBoxChanged_Pre]
    bool handled = false;
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == sysexIn.get()) {
        //[UserComboBoxCode_sysexIn] -- add your combo box handling code here..
        //[/UserComboBoxCode_sysexIn]
    }
    //else if (comboBoxThatHasChanged == scalingFactor.get()) {
    //    //[UserComboBoxCode_scalingFactor] -- add your combo box handling code here..
    //    //[/UserComboBoxCode_scalingFactor]
    //}

    else if (comboBoxThatHasChanged == profileSelector.get()) {
        int idx = profileSelector->getSelectedItemIndex();
        if (idx > 0) {
            juce::File profileDir = DexedAudioProcessor::dexedAppDir.getChildFile("Profiles");
            juce::Array<juce::File> files;
            profileDir.findChildFiles(files, juce::File::findFiles, false, "*.xml");
            files.sort();
            int fileIdx = idx - 1;
            if (fileIdx < files.size())
                loadProfileFromFile(files[fileIdx]);
        }
        handled = true;
    }

    //[UsercomboBoxChanged_Post]
    if( ! handled )
        general_callback_(this);
    //[/UsercomboBoxChanged_Post]
}

void ParamDialog::buttonClicked (juce::Button* buttonThatWasClicked) {
    bool handled = false;
        if (buttonThatWasClicked == sclButton.get()) {
        //[UserButtonCode_sclButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::LOAD_SCL);
        handled = true;
        //[/UserButtonCode_sclButton]
    }
    else if (buttonThatWasClicked == kbmButton.get()) {
        //[UserButtonCode_kbmButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::LOAD_KBM);
        handled = true;
        //[/UserButtonCode_kbmButton]
    }
    else if (buttonThatWasClicked == showTunButton.get()) {
        //[UserButtonCode_showTunButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::SHOW_TUNING);
        handled = true;
        //[/UserButtonCode_showTunButton]
    }
    else if (buttonThatWasClicked == resetTuningButton.get()) {
        //[UserButtonCode_resetTuningButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::RESET_TUNING);
        handled = true;
        //[/UserButtonCode_resetTuningButton]
    }
    else if (buttonThatWasClicked == mpeEnabled.get()) {
        //[UserButtonCode_mpeEnabled] -- add your button handler code here..
        //[/UserButtonCode_mpeEnabled]
    }
    else if (buttonThatWasClicked == transposeHelp.get()) {
        //[UserButtonCode_transposeHelp] -- add your button handler code here..
        auto te = new TextEditor();
        te->setReadOnly(true);
        te->setMultiLine(true);
        te->setBounds( 0, 0, 250, 200 );
        te->setText( R"TUNINGHELP(
Help on Tuning Transposition

Many DX7 cartridges use a transposition of -12 or -24 to acheive a bass sound. The DX7 transposes in key space, and so this transposition remaps C4 to C3 and so on, leading to an octave drop.

When applying non-12-note scales, though, this transposition results in an irregular retuning. If you have a 15 note scale, a transposition of -12 will move you to the  third note of the scale, not the root.

To allow existing patches to work, we have added a feature which replaces transpositions of -24, -12, 12, and 24 with the length of the scale. This behavior is controlled by the switch here.

With the switch in the SCL (lighted) position, a transposition of -24 is replaced with  -2 scale length, -12 by - scale length, and so on. This results in octave transpositions being scale aware, but has the strange effect that the jump from -11 to -12 to -13 in  transposition space is no longer continuous in frequency space. With an 18 note octave transpose -11 will play note 7, -12 note 0, and -13 note 5.

With the switch in the 12 (unlighted) position, transposition stays with the keyboard. In this case transposition through 11, 12 and 13 is continous, but transposition by 12 is no a single scale length drop.
)TUNINGHELP" );

        DialogWindow::LaunchOptions options;
        options.content.setOwned(te);
        options.dialogTitle = "Transposition Help";
        options.dialogBackgroundColour = DXLookNFeel::lightBackground;
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = false;
        options.resizable = false;

        auto dialogwindow = options.launchAsync();

        //[/UserButtonCode_transposeHelp]
    }
    else if (buttonThatWasClicked == storeButton.get()) {
        auto* aw = new juce::AlertWindow("Save Profile", "Enter a name for this profile:", juce::MessageBoxIconType::NoIcon);
        aw->addTextEditor("name", "", "Profile name:");
        aw->addButton("Save", 1);
        aw->addButton("Cancel", 0);
        aw->enterModalState(true, juce::ModalCallbackFunction::create([this, aw](int result) {
            if (result == 1) {
                juce::String name = aw->getTextEditorContents("name").trim();
                if (name.isNotEmpty())
                    saveProfileToFile(name);
            }
        }), true);
        handled = true;
    }

    //[UserbuttonClicked_Post]
    if( ! handled ) {
        general_callback_(this);
    }
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ParamDialog::setDialogValues(Controllers &c, SysexComm &mgr, bool showKey) {
    sysexChl->setValue(mgr.getChl() + 1);

    // Modulation components (whl, ft, br, at) are synced via parameter attachments

    mpeEnabled->setToggleState(c.mpeEnabled, dontSendNotification);
    mpePBRange->setValue(c.mpePitchBendRange, dontSendNotification);

    // TODO: fix for JUCE 8 migration
    // StringArray inputs = MidiInput::getDevices();
    // int idx = inputs.indexOf(mgr.getInput());
    // idx = idx == -1 ? 0 : idx + 1;
    // sysexIn->setSelectedItemIndex(idx);

    //showKeyboard->setToggleState(showKey, NotificationType::dontSendNotification);

    //if ( dpiScaleFactor == 1.25 ) {
    //    scalingFactor->setSelectedItemIndex(1);
    //} else if ( dpiScaleFactor == 1.50 ) {
    //    scalingFactor->setSelectedItemIndex(2);
    //}
    //else if (dpiScaleFactor == 2.00) {
    //    scalingFactor->setSelectedItemIndex(3);
    //}
    //else if (dpiScaleFactor == 3.00) {
    //    scalingFactor->setSelectedItemIndex(4);
    //}
    //else if (dpiScaleFactor == 4.00) {
    //    scalingFactor->setSelectedItemIndex(5);
    //} else {
    //    scalingFactor->setSelectedItemIndex(0);
    //}
}

bool ParamDialog::getDialogValues(Controllers &c, SysexComm &mgr, bool *showKey) {
    bool ret = true;

    // Modulation components (whl, ft, br, at) are synced via parameter attachments

    c.mpeEnabled = mpeEnabled->getToggleState();
    c.mpePitchBendRange = mpePBRange->getValue();

    c.refresh();

    if ( ! JUCEApplication::isStandaloneApp() ) {
        ret &= mgr.setInput(sysexIn->getItemText(sysexIn->getSelectedItemIndex()));
    }

    mgr.setChl(sysexChl->getValue() - 1);

    //*showKey = showKeyboard->getToggleState();

    //switch(scalingFactor->getSelectedItemIndex()) {
    //    case 1 :
    //        *dpiScaleFactor = 1.25;
    //        break;
    //    case 2 :
    //        *dpiScaleFactor = 1.5;
    //        break;
    //    case 3:
    //        *dpiScaleFactor = 2.0;
    //        break;
    //    case 4:
    //        *dpiScaleFactor = 3.0;
    //        break;
    //    case 5:
    //        *dpiScaleFactor = 4.0;
    //        break;
    //    default:
    //        *dpiScaleFactor = 1.0;
    //}
    return ret;
}

void ParamDialog::setIsStandardTuning( bool b ) {
    is_standard_tuning_ = b;

    // Allow to see the actual tuning always.
/*
    // Enable to ''showTunButton'' only if the tuning
    // is not the standard one.
    if( showTunButton != nullptr )
        showTunButton->setEnabled( ! b );
*/
    if( resetTuningButton != nullptr )
        resetTuningButton->setEnabled( ! b );
}

// Force the internal component dialog to have keyboard focus. Ugly, but it is
// the only way I have found (including on the JUCE forum).
void ParamDialog::timerCallback() {
    stopTimer();
    grabKeyboardFocus();
}

static juce::StringArray dialogParamIds() {
    return {
        IDs::pitchRangeUp.name,    IDs::pitchRangeDown.name,  IDs::pitchStep.name,
        IDs::transposeTuningScale.name, IDs::glissando.name,  IDs::portamentoTm.name,
        IDs::modWheel.name,        IDs::modWheelPitch.name,   IDs::modWheelAmp.name,   IDs::modWheelEgBias.name,
        IDs::modFoot.name,         IDs::modFootPitch.name,    IDs::modFootAmp.name,    IDs::modFootEgBias.name,
        IDs::modBreath.name,       IDs::modBreathPitch.name,  IDs::modBreathAmp.name,  IDs::modBreathEgBias.name,
        IDs::modAftertouch.name,   IDs::modAftertouchPitch.name, IDs::modAftertouchAmp.name, IDs::modAftertouchEgBias.name,
        IDs::engineType.name
    };
}

void ParamDialog::refreshProfileList() {
    profileSelector->clear();
    profileSelector->addItem("Select profile...", 1);

    juce::File profileDir = DexedAudioProcessor::dexedAppDir.getChildFile("Profiles");
    if (profileDir.exists()) {
        juce::Array<juce::File> files;
        profileDir.findChildFiles(files, juce::File::findFiles, false, "*.xml");
        files.sort();
        for (int i = 0; i < files.size(); ++i)
            profileSelector->addItem(files[i].getFileNameWithoutExtension(), i + 2);
    }
    profileSelector->setSelectedItemIndex(0, juce::dontSendNotification);
}

void ParamDialog::saveProfileToFile(const juce::String& name) {
    juce::File profileDir = DexedAudioProcessor::dexedAppDir.getChildFile("Profiles");
    profileDir.createDirectory();

    juce::ValueTree profileVt("DexedProfile");
    for (const auto& paramId : dialogParamIds()) {
        if (auto* param = apvts_.getParameter(paramId)) {
            juce::ValueTree paramVt("param");
            paramVt.setProperty(IDs::id, paramId, nullptr);
            paramVt.setProperty(IDs::value, param->convertFrom0to1(param->getValue()), nullptr);
            profileVt.addChild(paramVt, -1, nullptr);
        }
    }

    juce::ValueTree tuningVt("tuning");
    tuningVt.setProperty("scl", juce::String(proc_.currentSCLData), nullptr);
    tuningVt.setProperty("kbm", juce::String(proc_.currentKBMData), nullptr);
    profileVt.addChild(tuningVt, -1, nullptr);

    auto xml = profileVt.createXml();
    xml->writeTo(profileDir.getChildFile(name + ".xml"));
    refreshProfileList();
}

void ParamDialog::loadProfileFromFile(const juce::File& file) {
    auto xml = juce::XmlDocument::parse(file);
    if (!xml) return;

    auto profileVt = juce::ValueTree::fromXml(*xml);
    for (int i = 0; i < profileVt.getNumChildren(); ++i) {
        auto child = profileVt.getChild(i);
        if (child.hasType("param")) {
            juce::String paramId = child.getProperty(IDs::id).toString();
            float value = static_cast<float>(child.getProperty(IDs::value));
            if (auto* param = apvts_.getParameter(paramId))
                param->setValueNotifyingHost(param->convertTo0to1(value));
        } else if (child.hasType("tuning")) {
            juce::String sclData = child.getProperty("scl").toString();
            juce::String kbmData = child.getProperty("kbm").toString();
            proc_.retuneToStandard();
            if (sclData.isNotEmpty())
                proc_.applySCLTuning(sclData.toStdString());
            if (kbmData.isNotEmpty())
                proc_.applyKBMMapping(kbmData.toStdString());
        }
    }
}

//==============================================================================
#if 0

#endif

