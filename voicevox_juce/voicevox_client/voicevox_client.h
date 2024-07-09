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
    double getSampleRate() const;
    std::int64_t getSongTeacherSpeakerId() const;

    //==============================================================================
    // High level API
    std::optional<std::vector<std::byte>> synthesis(juce::uint32 speaker_id, const juce::String& audio_query_json);
    std::optional<std::vector<std::byte>> tts(juce::uint32 speaker_id, const juce::String& speak_words);

    //==============================================================================
    // Song API
    std::optional<std::vector<std::int64_t>> predictSingConsonantLength(juce::uint32 speaker_id, const std::vector<std::int64_t>& note_consonant_vector, const std::vector<std::int64_t>& note_vowel_vector, const std::vector<std::int64_t>& note_length_vector);
    std::optional<std::vector<float>> predictSingF0(juce::uint32 speaker_id, const std::vector<std::int64_t>& phoneme_flatten, const std::vector<std::int64_t>& note_vector);
    std::optional<std::vector<float>> predictSingVolume(juce::uint32 speaker_id, const std::vector<std::int64_t>& phoneme, const std::vector<std::int64_t>& note, const std::vector<float>& f0);
    std::optional<std::vector<float>> singBySfDecode(juce::uint32 speaker_id, const VoicevoxSfDecodeSource& decode_source);

private:
    //==============================================================================
    std::atomic<bool> isConnected_;
    std::unique_ptr<voicevox::SharedVoicevoxCoreHost> sharedVoicevoxCoreHost;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxClient)
};

}
