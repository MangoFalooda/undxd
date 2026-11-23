

#include "PluginEditor.h"
#include "window/ParamDialog.h"
#include "midi/SysexComm.h"
#include "window/TuningShow.h"
#include "../debugger/Logger.h"

#include "../msfa/fm_op_kernel.h"

//==============================================================================
DexedAudioProcessorEditor::DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard),
      cartManager(this), global(*ownerFilter, this) {
    processor = ownerFilter;

    setExplicitFocusOrder(1);

    File themeFile = DexedAudioProcessor::dexedAppDir.getChildFile("theme.json");
    DXLookNFeel::saveDefaultThemeIfMissing(themeFile);
    DXLookNFeel::loadTheme(themeFile);
    lookAndFeel->setDefaultLookAndFeel(lookAndFeel);

     //operators
    for (int i=0;i<6;i++) {
        operators[i] = std::make_unique<OperatorEditor>(*processor, i);
        operators[i]->setLayoutRow(layoutGrid);
        addAndMakeVisible(operators[i].get());
    }
     
    // midi keyboard
    addAndMakeVisible (&midiKeyboard);

    midiKeyboard.setLowestVisibleKey(24);
    midiKeyboard.setKeyWidth(24.0f);
    midiKeyboard.setTitle("Keyboard keys");

    // globals
    addAndMakeVisible(&global);
    global.setBounds(2,436,864,144);


    cartButton = std::make_unique<juce::TextButton>("CART");
    parmButton = std::make_unique<juce::TextButton>("PARM");
    initButton = std::make_unique<juce::TextButton>("INIT");
    storeButton = std::make_unique<juce::TextButton>("STORE");

    addAndMakeVisible(*cartButton);
    addAndMakeVisible(*parmButton);
    addAndMakeVisible(*initButton);
    addAndMakeVisible(*storeButton);

    cartButton->onClick = [this]() { cartShow(); };
    parmButton->onClick = [this]() { parmShow(); };
    initButton->onClick = [this]() { initProgram(); };
    storeButton->onClick = [this]() { storeProgram(); };

    
    aboutButton = std::make_unique<juce::TextButton>("UNDXD");
    aboutButton->setLookAndFeel(&logoLAF);
    addAndMakeVisible(*aboutButton);

    aboutBox = std::make_unique<AboutBox>();
    aboutBox->setVisible(false);
    aboutButton->onClick = [this]() {
        if (auto* parent = getParentComponent()) {
            aboutBox->centreAroundComponent(this, 460, 500);
            aboutBox->enterModalState(true, nullptr, false);
        }
    };


    layoutButton.setLookAndFeel(&layoutToggleLAF);
    layoutButton.setClickingTogglesState(false);
    layoutButton.setToggleState(!layoutGrid, juce::dontSendNotification);

    layoutButton.onClick = [this]() {
        layoutGrid = !layoutGrid;
        processor->layoutRow = layoutGrid;
        layoutButton.setToggleState(!layoutGrid, juce::dontSendNotification);

        for (auto& op : operators) {
            if (op != nullptr)
                op->setLayoutRow(layoutGrid);
        }

        resized(); 
        processor->savePreference();
    };

    addAndMakeVisible(&layoutButton);


    vuOutput = std::make_unique<VuMeterOutput>();
    vuOutput->setName("vuOutput");
    addAndMakeVisible(*vuOutput);

    observer = std::make_unique<ParameterObserver>(this, processor->parameters);
    addAndMakeVisible(*observer);

    programSelector = std::make_unique<StepperComboBox>("programSelector");
    programSelector->setLookAndFeel(&customLaf);
    addAndMakeVisible(*programSelector);
    programs = programSelector.get();

    rebuildProgramCombobox();
    programs->addListener(this);

    addChildComponent(&cartManagerCover);
    cartManagerCover.addChildComponent(&cartManager);
    cartManager.setVisible(true);



    setResizable(true, false);
    setResizeLimits(1200, 800, 1800, 1200);
    getConstrainer()->setFixedAspectRatio(1.5);


    layoutGrid = processor->layoutRow;
    layoutButton.setToggleState(!layoutGrid, juce::dontSendNotification);
    for (auto& op : operators) {
        if (op != nullptr)
            op->setLayoutRow(layoutGrid);
    }
    resized();


    addKeyListener(this);
    updateUI();
    startTimer(20);
}

DexedAudioProcessorEditor::~DexedAudioProcessorEditor() {
    stopTimer();
    aboutButton->setLookAndFeel(nullptr);
    programSelector->setLookAndFeel(nullptr);
    layoutButton.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}

//==============================================================================
void DexedAudioProcessorEditor::paint (Graphics& g) {
    g.fillAll(lookAndFeel->background);
}

void DexedAudioProcessorEditor::resized() {

    auto area = getLocalBounds();
    const int w = area.getWidth();
    const int h = area.getHeight();

    auto header = area.removeFromTop(h * 0.05);

    const int ws = w*0.1;
    aboutButton->setBounds(header.removeFromLeft(ws));
    header.reduce(0, 5);
    header.removeFromLeft(ws*0.5);
    layoutButton.setBounds(header.removeFromLeft(ws * 0.2));
    cartButton->setBounds(header.removeFromLeft(ws*0.8));
    parmButton->setBounds(header.removeFromLeft(ws*0.8));
    initButton->setBounds(header.removeFromLeft(ws*0.8));
    storeButton->setBounds(header.removeFromLeft(ws*0.8));
    programSelector->setBounds(header.removeFromLeft(ws*1.2));
    
    vuOutput->setBounds(header.removeFromRight(ws*1.3).reduced(4,8));
    observer->setBounds(header.reduced(8,0));


    constexpr int columns = 3;
    constexpr int rows = 2;
    constexpr int gap = 2;
    constexpr int margin = 2;

    area.reduce(margin, margin);

    midiKeyboard.setBounds(area.removeFromBottom(h * 0.08));
    area.removeFromBottom(2);

    auto globalArea = area.removeFromBottom(h*0.22);
    global.setBounds(globalArea);

    area.removeFromBottom(2);

    // operators
    if (layoutGrid)
    {
         const int opWidth = (area.getWidth() - gap * (columns - 1)) / columns;
         const int opHeight = (area.getHeight() - gap * (rows - 1)) / rows;

         for (int i = 0; i < 6; ++i) {
             const int col = i % columns;
             const int row = i / columns;

            operators[i]->setBounds(area.getX() + col * (opWidth + gap),
                                    area.getY() + row * (opHeight + gap), opWidth, opHeight);
        }
    }
    else
    {

        const int opHeight = (area.getHeight()) / 6;
        for (int i = 0; i < 6; ++i) {
            operators[i]->setBounds(area.removeFromTop(opHeight));
        }
    }

    if (cartManagerCover.isVisible()) {
        cartManagerCover.setBounds(getLocalBounds());
        cartManager.setBounds(cartManagerCover.getLocalBounds().reduced(100, 80));
    }
}

void DexedAudioProcessorEditor::cartShow() {
    stopTimer();
    cartManager.resetActiveSysex();
    cartManager.updateCartFilename();
    cartManagerCover.setVisible(true);
    resized();
    cartManager.initialFocus();
}

std::unique_ptr<ComponentTraverser> DexedAudioProcessorEditor::createFocusTraverser() {
    return std::make_unique<FocusTraverser>();
}

void DexedAudioProcessorEditor::loadCart(File file) {
    Cartridge cart;

    int rc = cart.load(file);

    if ( rc < 0 ) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Unable to open: " + file.getFullPathName());
        return;
    }

    if ( rc != 0 ) {
        AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Unable to find DX7 sysex cartridge in file",
                                     "This sysex file is not for the DX7 or it is corrupted. "
                                     "Do you still want to load this file as random data ?", {}, {}, this,
                                     ModalCallbackFunction::create([this, cart, file](int result) {
                                         if ( result != 0 )
                                             applyCart(cart, file);
                                     }));
        return;
    }

    applyCart(cart, file);
}

void DexedAudioProcessorEditor::applyCart(Cartridge cart, File file) {
    processor->loadCartridge(cart);
    rebuildProgramCombobox();
    processor->setCurrentProgram(0);
    programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
    processor->updateHostDisplay();

    processor->activeFileCartridge = file;
}

void DexedAudioProcessorEditor::saveCart() {
    fileChooser = std::make_unique<FileChooser>("Export DX sysex...", processor->dexedCartDir, "*.syx;*.SYX", true);
    fileChooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::warnAboutOverwriting,
                             [this](const FileChooser& fc) {
                                 if ( fc.getResults().isEmpty() )
                                     return;
                                 if ( ! processor->currentCart.saveVoice(fc.getResult()) ) {
                                     AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                                       "Error",
                                                                       "Unable to write: " + fc.getResult().getFullPathName());
                                 }
                             });
}

void DexedAudioProcessorEditor::tuningShow() {
    auto te = new TuningShow();
    te->setTuning( processor->synthTuningState->getTuning() );

    DialogWindow::LaunchOptions options;
    options.content.setOwned(te);
    options.dialogTitle = "Current Tuning";
    options.dialogBackgroundColour = Colour(0xFF323E44);
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = false;
    options.resizable = false;

    auto dialogwindow = options.launchAsync();
}

class DexedDialogWindow : public juce::DialogWindow {
public:
    DexedDialogWindow(const juce::String& title, juce::Colour backgroundColour)
        : juce::DialogWindow(title, backgroundColour, true, false) {

    }
    void closeButtonPressed() override {
        setVisible(false);
    }
};

void DexedAudioProcessorEditor::parmShow() {
    auto param = new ParamDialog(processor->parameters, *processor);
    param->setColour(AlertWindow::backgroundColourId, DXLookNFeel::lightBackground);
    param->setDialogValues(processor->controllers, processor->sysexComm, processor->showKeyboard);
    param->setIsStandardTuning(processor->synthTuningState->is_standard_tuning() );
    param->setTuningCallback([this](ParamDialog *p, ParamDialog::TuningAction which) {
                                switch(which) {
                                case ParamDialog::LOAD_SCL:
                                    this->processor->applySCLTuning();
                                    break;
                                case ParamDialog::LOAD_KBM:
                                    this->processor->applyKBMMapping();
                                    break;
                                case ParamDialog::RESET_TUNING:
                                    this->processor->retuneToStandard();
                                    break;
                                case ParamDialog::SHOW_TUNING:
                                    // consider https://forum.juce.com/t/closing-a-modal-dialog-window/2961
                                    this->tuningShow();
                                    break;
                                }
                                p->setIsStandardTuning(this->processor->synthTuningState->is_standard_tuning() );
                            } );

    auto generalCallback = [this](ParamDialog *param) {
                                   bool ret = param->getDialogValues(this->processor->controllers, this->processor->sysexComm, &this->processor->showKeyboard);
                                   this->processor->savePreference();

                                   if ( ret == false ) {
                                       AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Midi Interface", "Error opening midi ports");
                                   }
                               };
    param->setGeneralCallback(generalCallback);

    dexedParameterDialog = std::make_unique<DexedDialogWindow>("dexed Parameters", DXLookNFeel::lightBackground);
    dexedParameterDialog->setContentOwned(param, true);
    addAndMakeVisible(dexedParameterDialog.get());
    dexedParameterDialog->centreAroundComponent(this, param->getWidth(), param->getHeight() + dexedParameterDialog->getTitleBarHeight());
    dexedParameterDialog->enterModalState(true,{});
}

void DexedAudioProcessorEditor::initProgram() {
    processor->resetToInitVoice();
}

void DexedAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor->setCurrentProgram(programs->getSelectedId()-1);
    processor->updateHostDisplay();
}

void DexedAudioProcessorEditor::timerCallback() {
    if ( processor->programChanged.compareAndSetBool(false, true) ) {
        updateUI();
    }

    if ( ! processor->peekVoiceStatus() )
        return;

    for(int i=0;i<6;i++) {

        const int amp_min = 1036152;
        const float one_per_amp_diff = (float)(1.0 / (259037922 - amp_min));
 
        int amp = processor->voiceStatus.amp[5 - i] - amp_min;
        if (amp <= 0) amp = 0;
        operators[i]->updateGain(amp * one_per_amp_diff);
        operators[i]->updateEnvPos(processor->voiceStatus.ampStep[5 - i]);
    }
    global.updatePitchPos(processor->voiceStatus.pitchStep);
    if (vuOutput) {
        vuOutput->v = processor->vuSignal;
        vuOutput->repaint();
    }
}

void DexedAudioProcessorEditor::updateUI() {
    for(int i=0;i<6;i++) {
        operators[i]->updateDisplay();
    }
    rebuildProgramCombobox();
    global.updateDisplay();
    cartManager.updateCartFilename();
}

void DexedAudioProcessorEditor::rebuildProgramCombobox() {
    programs->clear(dontSendNotification);

    processor->currentCart.getProgramNames(processor->programNames);

    for(int i=0;i<processor->getNumPrograms();i++) {
        String id;
        id << (i+1) << ". " << processor->getProgramName(i);
        programs->addItem(id, i+1);
    }

    programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);

    String name = processor->activeProgram.getName();
    cartManager.setActiveProgram(processor->getCurrentProgram(), name);
    if ( name != processor->getProgramName(processor->getCurrentProgram()) )
        programs->setText("**. " + name, dontSendNotification);

    cartManager.resetActiveSysex();
}

void DexedAudioProcessorEditor::storeProgram() {
    storeProgramShowDialog(processor->currentCart, File());
}

void DexedAudioProcessorEditor::storeProgramShowDialog(Cartridge destSysex, File externalFile) {
    String currentName = processor->activeProgram.getName();
    bool activeCartridgeFound = processor->activeFileCartridge.exists();
    bool hasExternalFile = externalFile.exists();
    String msg;

    if ( ! hasExternalFile ) {
        if ( activeCartridgeFound )
            msg = "Store program to current (" + processor->activeFileCartridge.getFileName() + ") / new cartridge";
        else
            msg = "Store program to current / new cartridge";
    } else {
        msg = "Store program to " + externalFile.getFileName();
    }

    auto dialog = new AlertWindow("Store Program", msg, AlertWindow::NoIcon, this);
    dialog->addTextEditor("Name", currentName, String("Name"), false);
    dialog->getTextEditor("Name")->setInputRestrictions(10);

    StringArray programs;
    destSysex.getProgramNames(programs);
    dialog->addComboBox("Dest", programs, "Program Destination");

    if ( ! hasExternalFile ) {
        StringArray saveAction;
        saveAction.add("Store program to DAW plugin song state");
        saveAction.add("Store program and create a new copy of the .syx cartridge");
        if ( activeCartridgeFound )
            saveAction.add("Store program and overwrite current .syx cartridge");

        dialog->addComboBox("SaveAction", saveAction, "Store Action");
    }

    dialog->addButton("OK", 0, KeyPress(KeyPress::returnKey));
    dialog->addButton("CANCEL", 1, KeyPress(KeyPress::escapeKey));
    dialog->addButton("EXTERNAL FILE", 2, KeyPress());

    dialog->enterModalState(true, ModalCallbackFunction::create(
        [this, dialog, destSysex, externalFile, hasExternalFile](int response) {
            storeProgramResponse(dialog, response, destSysex, externalFile, hasExternalFile);
        }), true);
}

void DexedAudioProcessorEditor::storeProgramResponse(AlertWindow *dialog, int response, Cartridge destSysex, File externalFile, bool hasExternalFile) {
    if ( response == 2 ) {
        fileChooser = std::make_unique<FileChooser>("Destination Sysex", processor->dexedCartDir, "*.syx;*.SYX;*.*", true);
        fileChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
                                 [this, destSysex](const FileChooser& fc) mutable {
                                     if ( fc.getResults().isEmpty() ) {
                                         cartManager.resetActiveSysex();
                                         return;
                                     }
                                     File chosenFile = fc.getResult();
                                     if ( destSysex.load(chosenFile) == 0 ) {
                                         storeProgramShowDialog(destSysex, chosenFile);
                                         return;
                                     }
                                     AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Read error", "Unable to read file");
                                     cartManager.resetActiveSysex();
                                 });
        return;
    }

    if ( response == 0 ) {
        TextEditor *name = dialog->getTextEditor("Name");
        ComboBox *dest = dialog->getComboBoxComponent("Dest");

        int programNum = dest->getSelectedItemIndex();
        String programName(name->getText());

        if ( ! hasExternalFile ) {
            rebuildProgramCombobox();
            processor->setCurrentProgram(programNum);
            processor->updateHostDisplay();

            int action = dialog->getComboBoxComponent("SaveAction")->getSelectedItemIndex();
            if ( action > 0 ) {
                if ( action == 1 ) {
                    fileChooser = std::make_unique<FileChooser>("Destination Sysex", processor->dexedCartDir, "*.syx;*.SYX", true);
                    fileChooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::warnAboutOverwriting,
                                             [this](const FileChooser& fc) {
                                                 if ( ! fc.getResults().isEmpty() ) {
                                                     File destination = fc.getResult();
                                                     processor->currentCart.saveVoice(destination);
                                                     processor->activeFileCartridge = destination;
                                                 }
                                                 cartManager.resetActiveSysex();
                                             });
                    return;
                }

                File destination = processor->activeFileCartridge;
                processor->currentCart.saveVoice(destination);
                processor->activeFileCartridge = destination;
            }
        } else {
            if ( ! destSysex.saveVoice(externalFile)) {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Write error", "Unable to write file");
            }
        }
    }

    cartManager.resetActiveSysex();
}

bool DexedAudioProcessorEditor::isInterestedInFileDrag (const StringArray &files) {
    if( files.size() != 1 ) return false;

    for( auto i = files.begin(); i != files.end(); ++i ) {
        if( i->endsWithIgnoreCase( ".scl" ) || i->endsWithIgnoreCase( ".kbm" ) )
            return true;
    }
    return false;
}

void DexedAudioProcessorEditor::filesDropped (const StringArray &files, int x, int y ) {
    if( files.size() != 1 ) return;
    auto fn = files[0];
    try {
        std::ifstream in(fn.toStdString(), std::ifstream::ate | std::ifstream::binary);
        std::ifstream::pos_type filesize = in.tellg();
        if (fn.endsWithIgnoreCase(".scl")) {
            if (filesize == 0) {
                AlertWindow::showMessageBoxAsync(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' is empty."
                );
            }
            else if (filesize > MAX_SCL_KBM_FILE_SIZE) {
                AlertWindow::showMessageBoxAsync(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' has " + std::to_string(filesize) + " bytes, exceeding the maximum limit ("+std::to_string(MAX_SCL_KBM_FILE_SIZE)+")."
                );
            }
            else {
                processor->applySCLTuning(File(fn));
            }
        }
        if (fn.endsWithIgnoreCase(".kbm")) {
            if (filesize == 0) {
                AlertWindow::showMessageBoxAsync(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' is empty."
                );
            }
            else if (filesize > MAX_SCL_KBM_FILE_SIZE) {
                AlertWindow::showMessageBoxAsync(
                    AlertWindow::WarningIcon,
                    "File size error!",
                    "File \'" + fn.toStdString() + "\' has " + std::to_string(filesize) + " bytes, exceeding the maximum limit (" + std::to_string(MAX_SCL_KBM_FILE_SIZE) + ")."
                );
            }
            else {
                processor->applyKBMMapping(File(fn));
            }
        }
    }
    catch (const std::ios_base::failure& ex) {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, 
            "I/O error!", 
            "Related to file \'" + fn.toStdString() + "\', an exception (std::ios_base::failure) occured: " + ex.what()
        );
    }
    catch (std::bad_alloc& ex) {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, 
            "I/O error!", 
            "Related to file \'" + fn.toStdString() + "\', an exception (std::bad_alloc) occured: " + ex.what());
    }
    catch (...) {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, 
            "I/O error!", 
            "Related to file \'"+fn.toStdString()+"\', an unknown exception occured.");
    };
}

bool DexedAudioProcessorEditor::keyPressed(const KeyPress& key, Component* originatingComponent) {
    int keycode = key.getKeyCode();
    ModifierKeys mods = key.getModifiers();

    #ifdef DEXED_EVENT_DEBUG
        TRACE("key pressed: %d\n", keycode);
    #endif

    if ( (keycode >= '1' && keycode <= '6') && mods.isCtrlDown() ) {
        int op = keycode - '1';

        if ( mods.isShiftDown() ) {
            operators[op]->toggleOpSwitch();
            return true;
        }

        operators[op]->grabKeyboardFocus();
        return true;
    }

    if ( keycode == 'G' && mods.isCtrlDown() ) {
        global.grabKeyboardFocus();
        return true;
    }

    if ( keycode == 'L' && mods.isCtrlDown() ) {
        cartShow();
        return true;
    }

    if ( keycode == 'P' && mods.isCtrlDown() ) {
        parmShow();
        return true;
    }

    if ( key.getKeyCode() == KeyPress::escapeKey ) {
        cartManager.hideCartridgeManager();
        return true;
    }

    return false;
}
