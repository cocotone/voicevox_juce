#include "voicevox_client.h"
#include "../voicevox_core_host/voicevox_core_host.h"

namespace voicevox
{

//==============================================================================
VoicevoxClient::VoicevoxClient()
    : isConnected_(false)
    , sharedVoicevoxCoreHost(nullptr)
{
}

VoicevoxClient::~VoicevoxClient()
{
}

//==============================================================================
void VoicevoxClient::connect()
{
    sharedVoicevoxCoreHost = std::make_unique<voicevox::SharedVoicevoxCoreHost>();
    isConnected_ = true;

    juce::Logger::outputDebugString("[voicevox_juce] voicevox_core version: " + sharedVoicevoxCoreHost->getObject().getVersion());

    const auto devices = juce::JSON::toString(sharedVoicevoxCoreHost->getObject().getSupportedDevicesJson());
    juce::Logger::outputDebugString("[voicevox_juce] voicevox_core supported devices: " + devices);

    const juce::String str_is_gpu_mode = sharedVoicevoxCoreHost->getObject().isGPUMode() ? "true" : "false";
    juce::Logger::outputDebugString("[voicevox_juce] voicevox_core is gpu mode: " + str_is_gpu_mode);
}

void VoicevoxClient::disconnect()
{
    sharedVoicevoxCoreHost.reset();
    isConnected_ = false;
}

bool VoicevoxClient::isConnected() const
{
    return isConnected_.load();
}

//==============================================================================
juce::var VoicevoxClient::getMetasJson() const
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().getMetasJson();
    }

    return juce::var();
}

juce::Result VoicevoxClient::loadModel(juce::uint32 speaker_id)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().loadModel(speaker_id);
    }

    return juce::Result::fail("Disconnected");
}

bool VoicevoxClient::isModelLoaded(juce::uint32 speaker_id) const
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().isModelLoaded(speaker_id);
    }

    return false;
}

double VoicevoxClient::getSampleRate() const
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().getSampleRate();
    }

    return 0.0;
}

std::int64_t VoicevoxClient::getSongTeacherSpeakerId() const
{
    return 6000;
}

//==============================================================================
std::optional<juce::MemoryBlock> VoicevoxClient::synthesis(juce::uint32 speaker_id, const juce::String& audio_query_json)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().synthesis(speaker_id, audio_query_json);
    }

    return std::nullopt;
}

std::optional<juce::MemoryBlock> VoicevoxClient::tts(juce::uint32 speaker_id, const juce::String& speak_words)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().tts(speaker_id, speak_words);
    }

    return std::nullopt;
}

std::optional<std::vector<std::int64_t>> VoicevoxClient::predictSingConsonantLength(juce::uint32 speaker_id, const std::vector<std::int64_t>& note_consonant_vector, const std::vector<std::int64_t>& note_vowel_vector, const std::vector<std::int64_t>& note_length_vector)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().predict_sing_consonant_length_forward(speaker_id, note_consonant_vector, note_vowel_vector, note_length_vector);
    }

    return std::nullopt;
}

std::optional<std::vector<float>> VoicevoxClient::predictSingF0(juce::uint32 speaker_id, const std::vector<std::int64_t>& phoneme_flatten, const std::vector<std::int64_t>& note_vector)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().predict_sing_f0_forward(speaker_id, phoneme_flatten, note_vector);
    }

    return std::nullopt;
}

std::optional<std::vector<float>> VoicevoxClient::predictSingVolume(juce::uint32 speaker_id, const std::vector<std::int64_t>& phoneme, const std::vector<std::int64_t>& note, const std::vector<float>& f0)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().predict_sing_volume_forward(speaker_id, phoneme, note, f0);
    }

    return std::nullopt;
}

std::optional<std::vector<float>> VoicevoxClient::singBySfDecode(juce::uint32 speaker_id, const VoicevoxSfDecodeSource& decode_source)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost->getObject().sf_decode_forward(speaker_id, decode_source.phonemeVector, decode_source.f0Vector, decode_source.volumeVector);
    }

    return std::nullopt;
}

}
