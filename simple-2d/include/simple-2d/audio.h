#ifndef SIMPLE_2D_AUDIO_H
#define SIMPLE_2D_AUDIO_H
#include "error_type.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <map>

namespace simple_2d {

    typedef std::shared_ptr<Mix_Chunk> ManagedSound;

    typedef std::shared_ptr<Mix_Music> ManagedMusic;

    /**
     * @class AudioSubsystem
     * @brief Manages audio operations such as loading, playing, and stopping sounds and music.
     *
     * This class provides an interface to initialize the audio subsystem, load audio files,
     * and control playback of sounds and music. It uses SDL_mixer for audio processing.
     */
    class AudioSubsystem {
    public:
        /**
         * @brief Default constructor.
         */
        AudioSubsystem() = default;

        /**
         * @brief Destructor to clean up resources.
         */
        ~AudioSubsystem() = default;

        /**
         * @brief Initializes the audio subsystem.
         *
         * This method must be called before any audio operations can be performed.
         * It separates construction from initialization to handle potential failures.
         * @return Error code indicating success or failure of initialization.
         */
        Error Init();

        /**
         * @brief Deinitializes the audio subsystem.
         */
        void Deinit();

        /**
         * @brief Loads a sound from a .wav file.
         *
         * @param path The file path to the .wav file.
         * @return ManagedSound A shared pointer to the loaded sound.
         */
        ManagedSound LoadSoundFromWavFile(const std::string &path);

        /**
         * @brief Loads music from a .mp3 file.
         *
         * @param path The file path to the .mp3 file.
         * @return ManagedMusic A shared pointer to the loaded music.
         */
        ManagedMusic LoadMusicFromMp3File(const std::string &path);

        /**
         * @brief Plays a sound.
         *
         * @param sound The sound to play.
         * @param isLoop Whether the sound should loop.
         * @return Error code indicating success or failure of playback.
         */
        Error PlaySound(const ManagedSound &sound, bool isLoop = false);

        /**
         * @brief Plays music.
         *
         * @param music The music to play.
         * @param isLoop Whether the music should loop.
         * @return Error code indicating success or failure of playback.
         */
        Error PlayMusic(const ManagedMusic &music, bool isLoop = true);

        /**
         * @brief Stops a sound.
         *
         * @param sound The sound to stop.
         */
        void StopSound(const ManagedSound &sound);

        /**
         * @brief Stops the currently playing music.
         */
        void StopCurrentMusic();

        /**
         * @brief Periodically cleans up audio resources.
         *
         * This method should be called periodically to free up resources used by
         * audio that is no longer playing.
         */
        void PeriodicCleanUp();

    private:
        std::map<int, ManagedSound> mPlayingSounds; ///< Map of currently playing sounds.
        ManagedMusic mPlayingMusic; ///< Currently playing music.
    };
}

#endif // SIMPLE_2D_AUDIO_H
