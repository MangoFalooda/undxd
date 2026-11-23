

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OperatorEditor.h"
#include "GlobalEditor.h"
#include "DXLookNFeel.h"
#include "window/CartManager.h"
#include "window/AboutBox.h"
#include "component/VUMeter.h"
#include "component/ParameterObserver.h"


class CartManagerCover : public juce::Component {
public:
    void paint(juce::Graphics& g) override { g.fillAll(juce::Colour(0xBB000000)); }
};


//==============================================================================

class DexedAudioProcessorEditor  : public AudioProcessorEditor, public ComboBox::Listener, public Timer,
                                   public FileDragAndDropTarget, public KeyListener {
    DXMidiKeyboard midiKeyboard;

    GlobalEditor global;
    std::unique_ptr<OperatorEditor> operators[6];

    CartManager cartManager;

    //Component cartManagerCover;
    CartManagerCover cartManagerCover;

    SharedResourcePointer<DXLookNFeel> lookAndFeel;
    std::unique_ptr<juce::DialogWindow> dexedParameterDialog;
    std::unique_ptr<FileChooser> fileChooser;
    void applyCart(Cartridge cart, File file);
    void storeProgramShowDialog(Cartridge destSysex, File externalFile);
    void storeProgramResponse(AlertWindow *dialog, int response, Cartridge destSysex, File externalFile, bool hasExternalFile);
public:
    DexedAudioProcessor *processor;

    DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter);
    ~DexedAudioProcessorEditor();
    virtual void timerCallback() override;

    virtual void paint (Graphics& g) override;
    void resized() override;

    virtual void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void updateUI();
    void rebuildProgramCombobox();
    void loadCart(File file);
    void saveCart();
    void initProgram();
    void storeProgram();
    void cartShow();
    void parmShow();
    void tuningShow();


    virtual bool isInterestedInFileDrag (const StringArray &files) override;
    virtual void filesDropped (const StringArray &files, int x, int y ) override;
    std::unique_ptr<ComponentTraverser> createFocusTraverser() override;

    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    static const int WINDOW_SIZE_X = 866;
    static const int WINDOW_SIZE_Y = 674;

   

private:

    std::unique_ptr<juce::TextButton> initButton;
    std::unique_ptr<juce::TextButton> parmButton;
    std::unique_ptr<juce::TextButton> cartButton;
    std::unique_ptr<juce::TextButton> storeButton;

    bool layoutGrid{false};
    juce::ToggleButton layoutButton;
    LayoutToggleLAF layoutToggleLAF;

    LogoButtonLAF logoLAF;
    std::unique_ptr<juce::TextButton> aboutButton;
    std::unique_ptr<AboutBox> aboutBox;

    std::unique_ptr<VuMeterOutput> vuOutput;

    std::unique_ptr<ParameterObserver> observer;
    SimpleComboBoxLaf customLaf;
    std::unique_ptr<StepperComboBox> programSelector;
    StepperComboBox* programs = nullptr;

#ifdef DEXED_EVENT_DEBUG
    FocusLogger focusLogger;
#endif
};


#endif  
