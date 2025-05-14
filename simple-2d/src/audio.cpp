#include <simple-2d/audio.h>
#include <boost/log/trivial.hpp>
#include "internal_utils.h"
#define TARGET_AUDIO_DEVICE SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK

static auto soundDeleter = [](Mix_Chunk *c) {
    BOOST_LOG_TRIVIAL(debug) << "Destroy sound " << c;
    Mix_FreeChunk(c);
};
static auto musicDeleter = [](Mix_Music *m) {
    BOOST_LOG_TRIVIAL(debug) << "Destroy music " << m;
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
    BOOST_LOG_TRIVIAL(info) << "Initializing audio subsystem...";
    SDL_Init(SDL_INIT_AUDIO);
    // @todo: support changing audio device and different audio device spec
    SDL_AudioSpec spec = {
        .format = MIX_DEFAULT_FORMAT,
        .channels = MIX_DEFAULT_CHANNELS,
        .freq = MIX_DEFAULT_FREQUENCY,
    };
    if (!Mix_OpenAudio(TARGET_AUDIO_DEVICE, &spec)) {
        BOOST_LOG_TRIVIAL(error) << "Got error \"" << SDL_GetError() << "\" when try to open audio device id " << TARGET_AUDIO_DEVICE;
        return Error::INIT;
    }
    SDL_AudioSpec actual_spec;
    if (!getAudioSpec(actual_spec)) {
        BOOST_LOG_TRIVIAL(error) << "Got unexpected error when try to query opended audio device id" << TARGET_AUDIO_DEVICE;
        return Error::INIT;
    }
    auto audio_format = (actual_spec.channels > 2) ? "surround" : (actual_spec.channels > 1) ? "stereo" : "mono";
    BOOST_LOG_TRIVIAL(info) << "Open audio device at "  << actual_spec.freq << "Hz "
                                                        << audioFormatString(actual_spec.format)
                                                        << " " << audio_format;
    BOOST_LOG_TRIVIAL(info) << "Audio subsystem initialized successfully!";
    return Error::OK;
}

simple_2d::AudioSubsystem::~AudioSubsystem() {
    BOOST_LOG_TRIVIAL(info) << "Closing audio subsystem...";
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    BOOST_LOG_TRIVIAL(info) << "Audio subsystem closed successfully!";
}

simple_2d::ManagedSound simple_2d::AudioSubsystem::LoadSoundFromWavFile(const std::string &path) {
    auto fullWavFilePath = GetRootPath() /= std::filesystem::path(path);
    auto loadedSound = Mix_LoadWAV(fullWavFilePath.c_str());
    if (loadedSound == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load sound .wav file " << path;
        return nullptr;
    }
    return ManagedSound(loadedSound, soundDeleter);
}

simple_2d::ManagedMusic simple_2d::AudioSubsystem::LoadMusicFromMp3File(const std::string &path) {
    auto full_wav_file_path = GetRootPath() /= std::filesystem::path(path);
    auto loaded_music = Mix_LoadMUS(full_wav_file_path.c_str());
    if (loaded_music == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load music .wav file " << path;
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
        BOOST_LOG_TRIVIAL(error) << "Cannot play sound at the moment!" << SDL_GetError();
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
        BOOST_LOG_TRIVIAL(error) << "Cannot play music at the moment! Get error \"" << SDL_GetError() << "\"";
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
