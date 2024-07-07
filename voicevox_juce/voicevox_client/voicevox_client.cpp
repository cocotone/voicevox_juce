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

}
