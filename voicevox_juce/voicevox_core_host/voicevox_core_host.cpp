#include "voicevox_core_host.h"
#include "voicevox_core.h"

namespace voicevox
{

class VoicevoxCoreLibraryLoader final
{
public:
    VoicevoxCoreLibraryLoader()
    {
#if JUCE_WINDOWS
        auto dll_file_to_open =
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
            .getSiblingFile("voicevox_core.dll");

        jassert(dll_file_to_open.existsAsFile());

        voicevoxCoreLibrary = std::make_unique<juce::DynamicLibrary>(dll_file_to_open.getFullPathName());

        jassert(voicevoxCoreLibrary->getNativeHandle() != nullptr);
#endif
    }

    ~VoicevoxCoreLibraryLoader()
    {
    }

    bool isHandled() const
    {
#if JUCE_WINDOWS
        return (voicevoxCoreLibrary->getNativeHandle() != nullptr);
#endif
        return true;
    }

private:

    std::unique_ptr<juce::DynamicLibrary> voicevoxCoreLibrary;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxCoreLibraryLoader)
};


//==============================================================================
VoicevoxCoreHost::VoicevoxCoreHost()
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    auto options = voicevox_make_default_initialize_options();

    // Some audio plugin host process like DAW can't handle GPU environment.
    options.acceleration_mode = VoicevoxAccelerationMode::VOICEVOX_ACCELERATION_MODE_CPU;

    auto jtalk_dict_dir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile)
        .getParentDirectory()
        .getChildFile("open_jtalk_dic_utf_8")
        .getFullPathName();
    jtalk_dict_dir = jtalk_dict_dir.replace("\\", "/");
    const auto str_jtalk_dict_dir = jtalk_dict_dir.toStdString();
    options.open_jtalk_dict_dir = str_jtalk_dict_dir.c_str();

    VoicevoxResultCode result = voicevox_initialize(options);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
    }
}

VoicevoxCoreHost::~VoicevoxCoreHost()
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    voicevox_finalize();
}

//==============================================================================
juce::String VoicevoxCoreHost::getVersion() const
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    return juce::String(juce::CharPointer_UTF8(voicevox_get_version()));
}

//==============================================================================
juce::var VoicevoxCoreHost::getMetasJson() const
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    juce::var result_json;

    const auto metas_json = voicevox_get_metas_json();
    juce::JSON::parse(juce::CharPointer_UTF8(metas_json), result_json);

    return result_json;
}

juce::Result VoicevoxCoreHost::loadModel(juce::uint32 speaker_id)
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    try {
        VoicevoxResultCode result = voicevox_load_model(speaker_id);
        
        if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
            const char* utf8Str = voicevox_error_result_to_message(result);
            juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
            return juce::Result::fail(juce::CharPointer_UTF8(utf8Str));
        }
    }
    catch (std::exception e) {
        return juce::Result::fail(e.what());
    }

    return juce::Result::ok();
}

bool VoicevoxCoreHost::isModelLoaded(juce::uint32 speaker_id) const
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    return voicevox_is_model_loaded((uint32_t)speaker_id);
}

//==============================================================================
std::optional<juce::String> VoicevoxCoreHost::makeAudioQuery(juce::uint32 speaker_id, const juce::String& speak_words)
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    char* output_audio_query_json;

    VoicevoxAudioQueryOptions audio_query_options = voicevox_make_default_audio_query_options();

    VoicevoxResultCode result = voicevox_audio_query(speak_words.toRawUTF8(), (uint32_t)speaker_id, audio_query_options, &output_audio_query_json);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
        return std::nullopt;
    }

    const auto audio_query_json_string = juce::String(juce::CharPointer_UTF8(output_audio_query_json));

    juce::Logger::outputDebugString(audio_query_json_string);

    voicevox_audio_query_json_free(output_audio_query_json);

    return audio_query_json_string;
}

std::optional<juce::MemoryBlock> VoicevoxCoreHost::synthesis(juce::uint32 speaker_id, const juce::String& audio_query_json)
{
    VoicevoxSynthesisOptions synthesis_options = voicevox_make_default_synthesis_options();

    uintptr_t output_binary_size = 0;
    uint8_t* output_wav = nullptr;

    VoicevoxResultCode result = voicevox_synthesis(audio_query_json.toRawUTF8(), (uint32_t)speaker_id, synthesis_options, &output_binary_size, &output_wav);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
        return std::nullopt;
    }

    juce::MemoryBlock memory_block(output_wav, output_binary_size);

    voicevox_wav_free(output_wav);

    return memory_block;
}

std::optional<juce::MemoryBlock> VoicevoxCoreHost::tts(juce::uint32 speaker_id, const juce::String& speak_words)
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    VoicevoxTtsOptions tts_options = voicevox_make_default_tts_options();

    uintptr_t output_binary_size = 0;
    uint8_t* output_wav = nullptr;

    VoicevoxResultCode result = voicevox_tts(speak_words.toRawUTF8(), (uint32_t)speaker_id, tts_options, &output_binary_size, &output_wav);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
        return std::nullopt;
    }

    juce::MemoryBlock memory_block(output_wav, output_binary_size);

    voicevox_wav_free(output_wav);

    return memory_block;
}

}


