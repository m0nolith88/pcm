#include <SDL2/SDL.h>
#define PI 3.14
#define BUFFOR 5000

struct MyAudioEngine
{
    SDL_AudioSpec spec {};
    unsigned      pitch = 0, wave_counter = 0, cycle = 0.3;

    MyAudioEngine()
    {
        spec.freq     = 96000;         // 96000 samples per second (96 kHz)
        spec.format   = AUDIO_F32SYS;  // Sample format: 32-bit float (system-defined byte order)
        spec.channels = 1;             // Number of channels: 1 (mono)
        spec.samples  = 4096;          // Buffer size in samples (chosen rather arbitrarily)
        spec.userdata = this;
        spec.callback = [](void* param, Uint8* stream, int len)
                        {
                            ((MyAudioEngine*)param)->callback((float*) stream, len / sizeof(float));((MyAudioEngine*)param)->callback((float*) stream, len / sizeof(float));
                        };
        auto dev = SDL_OpenAudioDevice(nullptr, 0, &spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
        SDL_PauseAudioDevice(dev, 0);
    }

    void callback(float* target, int num_samples) {
		float coeficients[BUFFOR];
	 	coeficients[0] = cycle - 0.5;
		for (int i = 1; i < spec.freq / pitch; i++) {
			coeficients[i] = sin(i * cycle * PI) * 2 / (i * PI);
		}		
		float sample = spec.freq * PI *2 / wave_counter;
		for (int i = 0; i < num_samples; i++) {
			float temp = sample * i;
			float val = coeficients[0];
			for(int j = 1; j < spec.freq / pitch; j++) {
				val += cos(temp * j) * coeficients[j];
				target[j] = val;
			}
		}
	}
};

int main()
{
    // Initialize SDL audio.
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    MyAudioEngine beeper;

    // Toggle the pitch between 440 Hz and 352 Hz every 0.5 seconds
    for(;;)
    {
        beeper.pitch = 440;
        SDL_Delay(500);

        beeper.pitch = 352;
        SDL_Delay(500);
    }
}

