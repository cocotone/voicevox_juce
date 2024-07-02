﻿#include "voicevox_core_host.h"
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
        return (voicevoxCoreLibrary->getNativeHandle() != nullptr);
    }

private:

    std::unique_ptr<juce::DynamicLibrary> voicevoxCoreLibrary;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicevoxCoreLibraryLoader)
};


//==============================================================================
VoicevoxCoreHost::VoicevoxCoreHost()
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    juce::Logger::outputDebugString(juce::String(voicevox_get_version()));

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
juce::var VoicevoxCoreHost::getMetasJson()
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    juce::var result_json;

    const auto metas_json = voicevox_get_metas_json();
    juce::JSON::parse(juce::CharPointer_UTF8(metas_json), result_json);

    return result_json;
}

juce::Result VoicevoxCoreHost::loadModel(int64_t speaker_id)
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

std::optional<juce::MemoryBlock> VoicevoxCoreHost::tts(int64_t speaker_id, const juce::String& speak_words)
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    VoicevoxTtsOptions tts_options = voicevox_make_default_tts_options();

    unsigned long long output_binary_size = 0;
    uint8_t* output_wav = nullptr;

    VoicevoxResultCode result = voicevox_tts(speak_words.toRawUTF8(), speaker_id, tts_options, &output_binary_size, &output_wav);

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

