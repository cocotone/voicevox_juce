#pragma once

#include <juce_core/juce_core.h>

#include "../voicevox_core_host/voicevox_core_host.h"

namespace voicevox
{

//==============================================================================
class VoicevoxClient final
{
public:
    //==============================================================================
    VoicevoxClient();
    ~VoicevoxClient();

    //==============================================================================
    void connect();
    void disconnect();
    bool isConnected() const;

    //==============================================================================
    juce::var getMetasJson();
    juce::Result loadModel(int64_t speaker_id);
    std::optional<juce::MemoryBlock> tts(int64_t speaker_id, const juce::String& speak_words);

private:
    //==============================================================================
    std::atomic<bool> isConnected_;
    std::unique_ptr<voicevox::SharedVoicevoxCoreHost> sharedVoicevoxCoreHost;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxClient)
};

}
