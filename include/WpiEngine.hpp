#ifndef WPIENGINE_HPP
#define WPIENGINE_HPP

#include "paengine.hpp"
#include "FFTEngine.hpp"
#include <vector>
#include <time.h>       // time_t, struct tm, difftime, time, mktime
#include <wiringPi.h>

//#define DITHER_FLAG   (paDitherOff)
typedef short SAMPLE;

struct WpiEngine : public PaEngine, public FFTEngine
{
    double      OUTPUT_FREQUENCY = 15e3;
    SAMPLE*     wavetable           = NULL;

    WpiEngine()
    {
        PA_SAMPLE_TYPE      = paInt16;
        NUM_CHANNELS        = 1;        // mono
        SAMPLE_RATE         = 44100;
        FRAMES_PER_BUFFER   = 2048;
        N                   = 8192;
        genSineWavetable    (15e3);
    }
    ~WpiEngine()
    {
        digitalWrite (1, LOW);
        delete [] wavetable;
        wavetable = NULL;
    }

    double F_max()                       { return SAMPLE_RATE / 2.f; }
    double resolution()                  { return SAMPLE_RATE / (double) N; }
    double Lowest_Detectable_Frequency() { return 5 * resolution(); }

    void init();
    void windoo();
    int  windooCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);
    double getFrequency();

    void genSineWavetable(double frequency);

    std::vector<double> header;
    std::vector<double> data;
    int currentMeasureType = 0;

    int finalizeHeader();
    void finalizeData();

    bool filterWind(double value);
    const int COUNT_WIND_DEFAULT = 3;

    double Time, Humidity, Temperature, Pressure, Wind;
    int nHumidity = 0, nTemperature = 0, nPressure = 0, nWind = 0;

    int fd;
    tm time2016 = tm();
    void initSerial();
    void serialWrite();
};

#endif