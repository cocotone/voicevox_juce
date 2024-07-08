#pragma once

#include <juce_core/juce_core.h>

namespace voicevox
{

//==============================================================================
class VoicevoxCoreHost;
using SharedVoicevoxCoreHost = juce::SharedResourcePointer<voicevox::VoicevoxCoreHost>;

struct VoicevoxSfDecodeSource
{
    std::vector<float> f0Vector{};
    std::vector<float> volumeVector{};
    std::vector<std::int64_t> phonemeVector{};

    JUCE_LEAK_DETECTOR(VoicevoxSfDecodeSource)
};

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
    std::optional<juce::Array<float>> singBySfDecode(juce::uint32 speaker_id, const VoicevoxSfDecodeSource& decode_source);

private:
    //==============================================================================
    std::atomic<bool> isConnected_;
    std::unique_ptr<voicevox::SharedVoicevoxCoreHost> sharedVoicevoxCoreHost;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxClient)
};

}
