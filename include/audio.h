/*! \class tb_audio
    \brief Audio class.

    This class is essentially a wrapper class for the Allegro audio
    API, providing song and sound effect management on top
    of the base functionality.
*/

#ifndef TB_AUDIO
#define TB_AUDIO
class tb_audio {
    private:
        static vector<MIDI *> midi;
        static vector<SAMPLE *> sample;

        static bool created;
        static bool song_playing;

    public:


        static void init_config() {
            if (created) {
                cout << "TB_AUDIO: Reloading sound library...\n";
                remove_audio();
            } else {
                cout << "TB_AUDIO: Initializing sound library...\n";
            }

            midi.clear();
            sample.clear();

            midi.reserve( get_config_int("audio","maxmidis",32) );
            sample.reserve( get_config_int("audio","maxsamples",32) );

            created = 1;
        }


        //! \brief This function removes all audio and MIDI files from the array.
        static void remove_audio() {
            cout << "TB_AUDIO: Removing all MIDI files... ";
            vector<MIDI *>::iterator m_it;
            for (m_it = midi.begin(); m_it < midi.end(); m_it++)
                if (*m_it != NULL) destroy_midi(*m_it);
            cout << "DONE\n";


            cout << "TB_AUDIO: Removing all audio files... ";
            vector<SAMPLE *>::iterator s_it;
            for (s_it = sample.begin(); s_it < sample.end(); s_it++)
                if (*s_it != NULL) destroy_sample(*s_it);
            cout << "DONE\n";

            created = 0;
        }



        static int open_song(const char * filename) {
            cout << "TB_AUDIO: Loading song: " << filename;

            MIDI * midi_temp = load_midi(filename);
            if (midi_temp != NULL) {
                midi.push_back (midi_temp);
                cout << " Success!\n";

                return (midi.size()-1);

            } else {
                string error = "Failed to load song!\n";
                error += filename;
                allegro_message(error.c_str());
                cout << " Failed\n";
                return -1;

            }
        }


        static void play_song(int songnum) {
            if (midi.size() > 0) {
                if (song_playing) {
                    stop_midi();
                }
                play_midi(midi[songnum], TRUE);
                song_playing = 1;
            } else {
                cout << "No song to play...\n";
            }
        }

        static void stop_song() {
            stop_midi();
            song_playing = 0;
        }






        static int open_sound(const char * filename) {
            cout << "TB_AUDIO: Loading sound: " << filename;

            SAMPLE * sample_temp = load_wav(filename);
            if (sample_temp != NULL) {
                sample.push_back (sample_temp);
                cout << " Success!\n";

                return (sample.size()-1);

            } else {
                string error = "Failed to load song!\n";
                error += filename;
                allegro_message(error.c_str());
                cout << " Failed\n";
                return -1;

            }
        }


        static void play_sound(int soundnum) {
            if (sample.size() > 0) {
                play_sample(sample[soundnum], 255, 128, 1000, FALSE);
            } else {
                cout << "No sound to play...\n";
            }
        }

        static void stop_sound(int soundnum) {
            stop_sample(sample[soundnum]);
        }

};
#endif

