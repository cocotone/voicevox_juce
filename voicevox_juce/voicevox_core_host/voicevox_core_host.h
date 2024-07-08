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
    double getSampleRate() const;

    //==============================================================================
    juce::var getSupportedDevicesJson() const;
    bool isGPUMode() const;

    //==============================================================================
    juce::var getMetasJson() const;
    juce::Result loadModel(juce::uint32 speaker_id);
    bool isModelLoaded(juce::uint32 speaker_id) const;

    //==============================================================================
    std::optional<juce::Array<juce::uint64>> predict_sing_consonant_length_forward(juce::uint32 speaker_id, std::vector<std::int64_t> consonant, std::vector<std::int64_t> vowel, std::vector<std::int64_t> note_duration);
    std::optional<juce::Array<float>> predict_sing_f0_forward(juce::uint32 speaker_idd, std::vector<std::int64_t> phoneme, std::vector<std::int64_t> note);
    std::optional<juce::Array<float>> predict_sing_volume_forward(juce::uint32 speaker_id, std::vector<std::int64_t> phoneme, std::vector<std::int64_t> note, std::vector<float> f0);
    std::optional<juce::Array<float>> sf_decode_forward(juce::uint32 speaker_id, std::vector<std::int64_t> phoneme_vector, std::vector<float> f0_vector, std::vector<float> volume_vector);

    //==============================================================================
    std::optional<juce::String> makeAudioQuery(juce::uint32 speaker_id, const juce::String& speak_words);
    std::optional<juce::MemoryBlock> synthesis(juce::uint32 speaker_id, const juce::String& audio_query_json);
    std::optional<juce::MemoryBlock> tts(juce::uint32 speaker_id, const juce::String& speak_words);

private:
#if 0
    //==============================================================================
    std::optional<juce::Array<float>> decode(juce::uint32 speaker_id, std::vector<float> f0_vector, std::vector<float> phoneme_vector);
    std::optional<juce::Array<float>> predictDuration();
    std::optional<juce::Array<float>> predictIntonation();
#endif

    //==============================================================================
    juce::SharedResourcePointer<VoicevoxCoreLibraryLoader> sharedVoicevoxCoreLibrary;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxCoreHost)
};

using SharedVoicevoxCoreHost = juce::SharedResourcePointer<voicevox::VoicevoxCoreHost>;

}
