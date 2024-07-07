#pragma once

#include <juce_core/juce_core.h>

namespace voicevox
{

//==============================================================================
class VoicevoxCoreHost;
using SharedVoicevoxCoreHost = juce::SharedResourcePointer<voicevox::VoicevoxCoreHost>;

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
    juce::var getMetasJson() const;
    juce::Result loadModel(juce::uint32 speaker_id);
    bool isModelLoaded(juce::uint32 speaker_id) const;

    //==============================================================================
    std::optional<juce::MemoryBlock> synthesis(juce::uint32 speaker_id, const juce::String& audio_query_json);
    std::optional<juce::MemoryBlock> tts(juce::uint32 speaker_id, const juce::String& speak_words);
    std::optional<juce::Array<juce::Array<float>>> humming(juce::uint32 speaker_id, const juce::var& humming_source);

private:
    //==============================================================================
    std::atomic<bool> isConnected_;
    std::unique_ptr<voicevox::SharedVoicevoxCoreHost> sharedVoicevoxCoreHost;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxClient)
};

}
