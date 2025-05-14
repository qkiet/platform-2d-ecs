#ifndef SIMPLE_2D_AUDIO_H
#define SIMPLE_2D_AUDIO_H
#include "error_type.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <map>

namespace simple_2d {

    typedef std::shared_ptr<Mix_Chunk> ManagedSound;

    typedef std::shared_ptr<Mix_Music> ManagedMusic;

    enum AudioFileType {
        WAV,
        MP3,
    };

    class AudioSubsystem {
    public:
        AudioSubsystem() = default;
        ~AudioSubsystem();
        // Initialize audio subsystem. Separate constructor and initialization is deliberate choice because initialization
        // may fail and we cannot handle failure in constructor.
        Error Init();

        // Load a sound from a .wav file.
        // @todo: support other file types.
        ManagedSound LoadSoundFromWavFile(const std::string &path);

        // Load a music from a .mp3 file.
        // @todo: support other file types.
        ManagedMusic LoadMusicFromMp3File(const std::string &path);

        // Play a sound.
        Error PlaySound(const ManagedSound &sound, bool isLoop = false);

        // Play a music.
        Error PlayMusic(const ManagedMusic &music, bool isLoop = true);

        // Stop a sound.
        void StopSound(const ManagedSound &sound);

        // Stop a music.
        void StopCurrentMusic();

        // Background clean up of audio resources in periodic manner.
        void PeriodicCleanUp();

    private:
        std::map<int, ManagedSound> mPlayingSounds;
        ManagedMusic mPlayingMusic;
    };
}

#endif // SIMPLE_2D_AUDIO_H
