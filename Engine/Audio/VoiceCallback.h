#pragma once
#include <xaudio2.h>
class VoiceCallback : public IXAudio2VoiceCallback
{
public:
    VoiceCallback() { streamEnd_ = false; }
    ~VoiceCallback() {}

    //Called when the voice has just finished playing a contiguous audio stream.
    void OnStreamEnd() noexcept { streamEnd_ = true; }

    //Unused methods are stubs
    void OnVoiceProcessingPassEnd() noexcept { }
    void OnVoiceProcessingPassStart(UINT32 SamplesRequired) noexcept {    }
    void OnBufferEnd(void* pBufferContext) noexcept { }
    void OnBufferStart(void* pBufferContext) noexcept {    }
    void OnLoopEnd(void* pBufferContext) noexcept {    }
    void OnVoiceError(void* pBufferContext, HRESULT Error) noexcept { }

public:

    bool GetStreamEnd() { return streamEnd_; }

    void SetStreamEnd(bool streamEnd) { streamEnd_ = streamEnd; }

private:

    // 終了フラグ
    bool streamEnd_;

};

