#include "PluginProcessor.h"
#include "parameter/Model.h"
#include "StateLegacy.h"

//==============================================================================
void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
    auto state = parameters.copyState();
    juce::ValueTree storage { IDs::root };
    storage.setProperty(IDs::version, DexedApvts::MODEL_VERSION, nullptr);
    storage.addChild(state, -1, nullptr);
    storage.addChild(parameters.rootVt.getChildWithName(IDs::profile).createCopy(), -1, nullptr);

    juce::ValueTree tuningVt("tuning");
    tuningVt.setProperty("scl", juce::String(currentSCLData), nullptr);
    tuningVt.setProperty("kbm", juce::String(currentKBMData), nullptr);
    storage.addChild(tuningVt, -1, nullptr);

    std::unique_ptr<juce::XmlElement> xml(storage.createXml());
    copyXmlToBinary(*xml, destData);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (source, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(parameters.rootVt.getType())) {
            ValueTree storage = juce::ValueTree::fromXml(*xmlState);

            juce::String version = storage.getProperty(IDs::version);
            if ( version != juce::String(DexedApvts::MODEL_VERSION) ) {
                TRACE("Incompatible state version %s (current is %s)", version.toRawUTF8(), DexedApvts::MODEL_VERSION);
                return;
            }

            //parameters.replaceState(storage.getChildWithName(IDs::parameters));
            //if (parameters.state.getParent() != parameters.rootVt) {
            //    parameters.rootVt.removeChild(parameters.rootVt.getChildWithName(IDs::parameters), nullptr);
            //    parameters.rootVt.addChild(parameters.state, 0, nullptr);
            //}

            auto newParameters = storage.getChildWithName(IDs::parameters);

            if (!newParameters.isValid()) {
                TRACE("Invalid parameter state");
                return;
            }

            parameters.replaceState(newParameters);

            juce::ValueTree storageProfile = storage.getChildWithName(IDs::profile);
            parameters.rootVt.getChildWithName(IDs::profile).getChildWithName(IDs::midiCCMappings).
                copyPropertiesAndChildrenFrom(storageProfile.getChildWithName(IDs::midiCCMappings), nullptr);

            juce::ValueTree tuningVt = storage.getChildWithName("tuning");
            if (tuningVt.isValid()) {
                juce::String sclData = tuningVt.getProperty("scl").toString();
                juce::String kbmData = tuningVt.getProperty("kbm").toString();
                retuneToStandard();
                if (sclData.isNotEmpty())
                    applySCLTuning(sclData.toStdString());
                if (kbmData.isNotEmpty())
                    applyKBMMapping(kbmData.toStdString());
            }
        } else if ( xmlState->hasTagName("dexedState") ) {
            // Format is 1.0 legacy
            setStateInformation10(this, xmlState.get());
            // implement legacy loader
        } else {
            TRACE("No valid state found");
        }
    }
}

