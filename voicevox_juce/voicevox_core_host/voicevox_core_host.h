#pragma once

#include <juce_core/juce_core.h>

namespace voicevox
{

class VoicevoxCoreLibraryLoader;

//==============================================================================
class VoicevoxCoreHost final
{
public:
    //==============================================================================
    VoicevoxCoreHost();
    ~VoicevoxCoreHost();

    //==============================================================================
    juce::var getMetasJson();
    juce::Result loadModel(int64_t speaker_id);
    std::optional<juce::MemoryBlock> tts(int64_t speaker_id, const juce::String& speak_words);

private:
    //==============================================================================
    juce::SharedResourcePointer<VoicevoxCoreLibraryLoader> sharedVoicevoxCoreLibrary;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxCoreHost)
};

using SharedVoicevoxCoreHost = juce::SharedResourcePointer<voicevox::VoicevoxCoreHost>;

}
