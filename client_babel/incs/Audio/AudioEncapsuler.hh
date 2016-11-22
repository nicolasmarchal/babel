#ifndef AUDIOENCAPSULER_HH_
# define AUDIOENCAPSULER_HH_

#include <iostream>
#include <vector>
#include <mutex>
#include <cstring>
#include "portaudio.h"
#include "EncodeEncapsuler.hh"
#include "IAudioObserver.hh"

class AudioEncapsuler
{
    public:
        AudioEncapsuler();
        ~AudioEncapsuler();

    public:
        int initialize();
        void startStreams();
        void stopStreams();
        void initStreams();

    private:
        int initInput();
        int initOutput();
        void errorAudio();

    private:
        PaStream *  _inputStream;
        PaStream *  _outputStream;
        PaError     _err;
        PaStreamParameters _inputParameters;
        PaStreamParameters _outputParameters;

    private:
        unsigned char _recBuffer[65535];
        int _sizeRecBuffer;

        unsigned char _playBuffer[65535];
        int _sizePlayBuffer;

        unsigned char *_data;
        int _sizeData;
    private:
        EncodeEncapsuler * _encoder;

    public:
        unsigned char * getRecBuffer() const;
        int getSizeRecBuffer() const;
        void addSizeRecBuffer(int value);
        EncodeEncapsuler * getEncoder() const;
        void setPlayBuffer(SAMPLE *);
        int getSizePlayBuffer() const;
        void setSizePlayBuffer(int size);
        unsigned char * getPlayBuffer() const;
        unsigned char * getData() const;
        void setData(unsigned char *);
        void setSizeData(int);
        int getSizeData() const;
        // std::vector<SAMPLE *> *getSamples();

      public:
        void subscribeOnRecord(IAudioObserver *ob);
        void notifySubscribers(unsigned char const *, int);
      private:
        std::vector<SAMPLE *> * samples;
        std::vector<IAudioObserver *> obs;
};

#endif
