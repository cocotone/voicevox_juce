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
#else
        return true;
#endif
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

    // NOTE: Some audio plugin host process like DAW can't handle GPU environment.
    options.acceleration_mode = VoicevoxAccelerationMode::VOICEVOX_ACCELERATION_MODE_AUTO;

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

double VoicevoxCoreHost::getSampleRate() const
{
    // NOTE: decode function is processed under 24kHz due to hard coding in core library.
    return 24000.0;
}

//==============================================================================
juce::var VoicevoxCoreHost::getSupportedDevicesJson() const
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    juce::var result_json;

    const auto devices_json = voicevox_get_supported_devices_json();
    juce::JSON::parse(juce::CharPointer_UTF8(devices_json), result_json);

    return result_json;
}

bool VoicevoxCoreHost::isGPUMode() const
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    return voicevox_is_gpu_mode();
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


std::optional<juce::Array<float>> VoicevoxCoreHost::predictDuration()
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    uintptr_t length = 256;

    std::array<int64_t, 256> phoneme_vector;
    int64_t* pv = phoneme_vector.data();

    uint32_t speaker_id = 0;

    uintptr_t output_data_length = 0;

    std::array<float, 256> output_data;
    float* odv = output_data.data();

    /**
     * 音素ごとの長さを推論する
     * @param [in] length phoneme_vector, output のデータ長
     * @param [in] phoneme_vector  音素データ
     * @param [in] speaker_id 話者ID
     * @param [out] output_predict_duration_length 出力データのサイズ
     * @param [out] output_predict_duration_data データの出力先
     * @return 結果コード #VoicevoxResultCode
     *
     * # Safety
     * @param phoneme_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param output_predict_duration_data_length uintptr_t 分のメモリ領域が割り当てられていること
     * @param output_predict_duration_data 成功後にメモリ領域が割り当てられるので ::voicevox_predict_duration_data_free で解放する必要がある
     */
    VoicevoxResultCode result = voicevox_predict_duration(length, pv, speaker_id, &output_data_length, &odv);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
        return std::nullopt;
    }

    juce::Array<float> output_buffer(output_data.data(), output_data_length);

    voicevox_predict_duration_data_free(odv);

    return output_buffer;
}

std::optional<juce::Array<float>> VoicevoxCoreHost::predictIntonation()
{
    jassert(sharedVoicevoxCoreLibrary->isHandled());

    uintptr_t length = 256;

    std::array<int64_t, 256> vowel_phoneme_vector;
    int64_t* vpv = vowel_phoneme_vector.data();

    std::array<int64_t, 256> consonant_phoneme_vector;
    int64_t* cpv = consonant_phoneme_vector.data();

    std::array<int64_t, 256> start_accent_vector;
    int64_t* sav = start_accent_vector.data();

    std::array<int64_t, 256> end_accent_vector;
    int64_t* eav = end_accent_vector.data();

    std::array<int64_t, 256> start_accent_phrase_vector;
    int64_t* sapv = start_accent_phrase_vector.data();

    std::array<int64_t, 256> end_accent_phrase_vector;
    int64_t* eapv = end_accent_phrase_vector.data();

    uint32_t speaker_id = 0;

    uintptr_t output_data_length = 0;

    std::array<float, 256> output_data;
    float* odv = output_data.data();

    /**
     * モーラごとのF0を推論する
     * @param [in] length vowel_phoneme_vector, consonant_phoneme_vector, start_accent_vector, end_accent_vector, start_accent_phrase_vector, end_accent_phrase_vector, output のデータ長
     * @param [in] vowel_phoneme_vector 母音の音素データ
     * @param [in] consonant_phoneme_vector 子音の音素データ
     * @param [in] start_accent_vector アクセントの開始位置のデータ
     * @param [in] end_accent_vector アクセントの終了位置のデータ
     * @param [in] start_accent_phrase_vector アクセント句の開始位置のデータ
     * @param [in] end_accent_phrase_vector アクセント句の終了位置のデータ
     * @param [in] speaker_id 話者ID
     * @param [out] output_predict_intonation_data_length 出力データのサイズ
     * @param [out] output_predict_intonation_data データの出力先
     * @return 結果コード #VoicevoxResultCode
     *
     * # Safety
     * @param vowel_phoneme_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param consonant_phoneme_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param start_accent_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param end_accent_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param start_accent_phrase_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param end_accent_phrase_vector 必ずlengthの長さだけデータがある状態で渡すこと
     * @param output_predict_intonation_data_length uintptr_t 分のメモリ領域が割り当てられていること
     * @param output_predict_intonation_data 成功後にメモリ領域が割り当てられるので ::voicevox_predict_intonation_data_free で解放する必要がある
     */
    VoicevoxResultCode result = voicevox_predict_intonation(length, vpv, cpv, sav, eav, sapv, eapv, speaker_id, &output_data_length, &odv);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
        return std::nullopt;
    }

    juce::Array<float> output_buffer(output_data.data(), output_data_length);

    voicevox_predict_intonation_data_free(odv);

    return output_buffer;
}

std::optional<juce::Array<float>> VoicevoxCoreHost::decode(juce::uint32 speaker_id, std::vector<float> f0_vector, std::vector<float> phoneme_vector)
{
    // NOTE: decode function is processed under 24kHz due to hard coded in core library.

    jassert(sharedVoicevoxCoreLibrary->isHandled());

    uintptr_t output_data_length = 0;
    float* output_data = nullptr;

    /**
     * decodeを実行する
     * @param [in] length f0 , output のデータ長及び phoneme のデータ長に関連する
     * @param [in] phoneme_size 音素のサイズ phoneme のデータ長に関連する
     * @param [in] f0 基本周波数
     * @param [in] phoneme_vector 音素データ
     * @param [in] speaker_id 話者ID
     * @param [out] output_decode_data_length 出力先データのサイズ
     * @param [out] output_decode_data データ出力先
     * @return 結果コード #VoicevoxResultCode
     *
     * # Safety
     * @param f0 必ず length の長さだけデータがある状態で渡すこと
     * @param phoneme_vector 必ず length * phoneme_size の長さだけデータがある状態で渡すこと
     * @param output_decode_data_length uintptr_t 分のメモリ領域が割り当てられていること
     * @param output_decode_data 成功後にメモリ領域が割り当てられるので ::voicevox_decode_data_free で解放する必要がある
     */
    VoicevoxResultCode result = voicevox_decode(length, phoneme_size, f0_vector.data(), phoneme_vector.data(), speaker_id, &output_data_length, &output_data);

    if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK) {
        const char* utf8Str = voicevox_error_result_to_message(result);
        juce::Logger::outputDebugString(juce::CharPointer_UTF8(utf8Str));
        return std::nullopt;
    }

    juce::Array<float> output_buffer(output_data, output_data_length);

    voicevox_decode_data_free(output_data);

    return output_buffer;
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
    // NOTE: Ouptut wav binary is sample rate converted to request value, therefore, the processing is effected after decode in core library.

    jassert(sharedVoicevoxCoreLibrary->isHandled());

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
    // NOTE: Ouptut wav binary is sample rate converted to request value, therefore, the processing is effected after decode in core library.

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


