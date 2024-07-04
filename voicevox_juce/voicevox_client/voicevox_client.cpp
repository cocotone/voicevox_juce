#include "voicevox_client.h"

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
juce::var VoicevoxClient::getMetasJson()
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost.get()->getObject().getMetasJson();
    }

    return juce::var();
}

juce::Result VoicevoxClient::loadModel(int64_t speaker_id)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost.get()->getObject().loadModel(speaker_id);
    }

    return juce::Result::fail("Disconnected");
}

std::optional<juce::MemoryBlock> VoicevoxClient::synthesis(int64_t speaker_id, const juce::String& audio_query_json)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost.get()->getObject().synthesis(speaker_id, audio_query_json);
    }

    return std::nullopt;
}

std::optional<juce::MemoryBlock> VoicevoxClient::tts(int64_t speaker_id, const juce::String& speak_words)
{
    if (isConnected())
    {
        return sharedVoicevoxCoreHost.get()->getObject().tts(speaker_id, speak_words);
    }

    return std::nullopt;
}

}
