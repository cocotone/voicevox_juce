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
    juce::String getVersion() const;

    //==============================================================================
    juce::var getSupportedDevicesJson() const;
    bool isGPUMode() const;

    //==============================================================================
    juce::var getMetasJson() const;
    juce::Result loadModel(juce::uint32 speaker_id);
    bool isModelLoaded(juce::uint32 speaker_id) const;

    //==============================================================================
    std::optional<juce::String> makeAudioQuery(juce::uint32 speaker_id, const juce::String& speak_words);
    std::optional<juce::MemoryBlock> synthesis(juce::uint32 speaker_id, const juce::String& audio_query_json);
    std::optional<juce::MemoryBlock> tts(juce::uint32 speaker_id, const juce::String& speak_words);

private:
    //==============================================================================
    juce::SharedResourcePointer<VoicevoxCoreLibraryLoader> sharedVoicevoxCoreLibrary;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxCoreHost)
};

using SharedVoicevoxCoreHost = juce::SharedResourcePointer<voicevox::VoicevoxCoreHost>;

}
