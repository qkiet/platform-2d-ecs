#include <simple-2d/audio.h>
#include <simple-2d/utils.h>
#include "internal_utils.h"
#include <climits>

#define TARGET_AUDIO_DEVICE SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK

static auto soundDeleter = [](Mix_Chunk *c) {
    SIMPLE_2D_LOG_DEBUG << "Destroy sound " << c;
    Mix_FreeChunk(c);
};
static auto musicDeleter = [](Mix_Music *m) {
    SIMPLE_2D_LOG_DEBUG << "Destroy music " << m;
    Mix_FreeMusic(m);
};


static bool getAudioSpec(SDL_AudioSpec &spec) {
    return Mix_QuerySpec(&spec.freq, &spec.format, &spec.channels);
}

static std::string audioFormatString(SDL_AudioFormat format) {
    switch (format)
    {
    case SDL_AUDIO_U8:
        return "unsigned 8bit";
    case SDL_AUDIO_S8:
        return "signed 8 bit";
    case SDL_AUDIO_S16LE:
        return "signed, low-endian 16 bit";
    case SDL_AUDIO_S16BE:
        return "signed, big-endian 16 bit";
    case SDL_AUDIO_S32LE:
        return "signed, low-endian 32 bit";
    case SDL_AUDIO_S32BE:
        return "signed, big-endian 32 bit";
    case SDL_AUDIO_F32LE:
        return "float, low-endian 32 bit";
    case SDL_AUDIO_F32BE:
        return "float, big-endian 32 bit";
    default:
        return "unknown format";
    }
}

simple_2d::Error simple_2d::AudioSubsystem::Init() {
    SIMPLE_2D_LOG_INFO << "Initializing audio subsystem...";
    SDL_Init(SDL_INIT_AUDIO);
    // @todo: support changing audio device and different audio device spec
    SDL_AudioSpec spec = {
        .format = MIX_DEFAULT_FORMAT,
        .channels = MIX_DEFAULT_CHANNELS,
        .freq = MIX_DEFAULT_FREQUENCY,
    };
    if (!Mix_OpenAudio(TARGET_AUDIO_DEVICE, &spec)) {
        SIMPLE_2D_LOG_ERROR << "Got error \"" << SDL_GetError() << "\" when try to open audio device id " << TARGET_AUDIO_DEVICE;
        return Error::INIT;
    }
    SDL_AudioSpec actual_spec;
    if (!getAudioSpec(actual_spec)) {
        SIMPLE_2D_LOG_ERROR << "Got unexpected error when try to query opended audio device id" << TARGET_AUDIO_DEVICE;
        return Error::INIT;
    }
    auto audio_format = (actual_spec.channels > 2) ? "surround" : (actual_spec.channels > 1) ? "stereo" : "mono";
    SIMPLE_2D_LOG_INFO << "Open audio device at "  << actual_spec.freq << "Hz "
                                                        << audioFormatString(actual_spec.format)
                                                        << " " << audio_format;
    SIMPLE_2D_LOG_INFO << "Audio subsystem initialized successfully!";
    return Error::OK;
}

void simple_2d::AudioSubsystem::Deinit() {
    SIMPLE_2D_LOG_INFO << "Closing audio subsystem...";
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    SIMPLE_2D_LOG_INFO << "Audio subsystem closed successfully!";
}

simple_2d::ManagedSound simple_2d::AudioSubsystem::LoadSoundFromWavFile(const std::string &path) {
    auto fullWavFilePath = GetRootPath() /= std::filesystem::path(path);
    auto loadedSound = Mix_LoadWAV(fullWavFilePath.string().c_str());
    if (loadedSound == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to load sound .wav file " << path;
        return nullptr;
    }
    return ManagedSound(loadedSound, soundDeleter);
}

simple_2d::ManagedMusic simple_2d::AudioSubsystem::LoadMusicFromMp3File(const std::string &path) {
    auto full_wav_file_path = GetRootPath() /= std::filesystem::path(path);
    auto loaded_music = Mix_LoadMUS(full_wav_file_path.string().c_str());
    if (loaded_music == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to load music .wav file " << path;
        return nullptr;
    }
    return ManagedMusic(loaded_music, musicDeleter);
}

simple_2d::Error simple_2d::AudioSubsystem::PlaySound(const ManagedSound &sound, bool isLoop) {
    int loopNum = 0;
    if (isLoop) {
        loopNum = INT_MAX;
    }
    int playingChannel = Mix_PlayChannel(-1, sound.get(), loopNum);
    if (playingChannel == -1) {
        SIMPLE_2D_LOG_ERROR << "Cannot play sound at the moment!" << SDL_GetError();
        return Error::AUDIO;
    }
    mPlayingSounds[playingChannel] = sound;
    return Error::OK;
}

simple_2d::Error simple_2d::AudioSubsystem::PlayMusic(const ManagedMusic &music, bool isLoop) {
    StopCurrentMusic();
    int loopNum = 0;
    if (isLoop) {
        loopNum = INT_MAX;
    }
    if (!Mix_PlayMusic(music.get(), loopNum)) {
        SIMPLE_2D_LOG_ERROR << "Cannot play music at the moment! Get error \"" << SDL_GetError() << "\"";
        return Error::AUDIO;
    }
    // Play music is simple: just stop current music and play this music
    mPlayingMusic.reset(music.get(), musicDeleter);
    return Error::OK;
}

void simple_2d::AudioSubsystem::StopSound(const ManagedSound &sound) {
    for (auto &[channel, playingSound] : mPlayingSounds) {
        if (playingSound == sound) {
            Mix_HaltChannel(channel);
            mPlayingSounds[channel].reset();
            break;
        }
    }
}

void simple_2d::AudioSubsystem::StopCurrentMusic() {
    Mix_HaltMusic();
}

void simple_2d::AudioSubsystem::PeriodicCleanUp() {
    for (auto &[channel, playingSound] : mPlayingSounds) {
        if (!Mix_Playing(channel)) {
            mPlayingSounds[channel].reset();
        }
    }
}
