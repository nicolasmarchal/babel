#include <fstream>
#include <string.h>
#include <cstring>
#include "AudioEncapsuler.hh"

std::mutex mtx;

AudioEncapsuler::AudioEncapsuler()
{
    _encoder = new EncodeEncapsuler;
    _inputStream = NULL;
    _outputStream = NULL;
    _err = paNoError;
    _sizePlayBuffer = 0;
    _sizeRecBuffer = 0;
    samples = new std::vector<SAMPLE *>;
}

AudioEncapsuler::~AudioEncapsuler()
{
    _err = Pa_Terminate();
    errorAudio();
}

int AudioEncapsuler::initialize()
{
  _err = Pa_Initialize();
  this->errorAudio();
  if (initInput() == -1)
      return (1);
  if (initOutput() == -1)
      return (2);
  initStreams();
  return (0);
}

int AudioEncapsuler::initInput()
{
  _inputParameters.device = Pa_GetDefaultInputDevice();
  if (_inputParameters.device == paNoDevice){
      std::cerr << "Error : No default input device." << std::endl;
      this->errorAudio();
      return (-1);
    }
  _inputParameters.channelCount = NUM_CHANNELS;
  _inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  _inputParameters.suggestedLatency = Pa_GetDeviceInfo(_inputParameters.device)->defaultLowInputLatency;
  _inputParameters.hostApiSpecificStreamInfo = NULL;
  return (0);
}

int AudioEncapsuler::initOutput()
{
  _outputParameters.device = Pa_GetDefaultOutputDevice();
  if (_outputParameters.device == paNoDevice){
      std::cerr << "Error : No default output device." << std::endl;
      this->errorAudio();
      return (-1);
    }
  _outputParameters.channelCount = NUM_CHANNELS;
  _outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  _outputParameters.suggestedLatency = Pa_GetDeviceInfo(_outputParameters.device)->defaultLowOutputLatency;
  _outputParameters.hostApiSpecificStreamInfo = NULL;
  return (0);
}

int recordSoundCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  SAMPLE * input = (SAMPLE *)inputBuffer;
  AudioEncapsuler * other = (AudioEncapsuler *)userData;
  SAMPLE * input2 = new SAMPLE[FRAMES_PER_BUFFER * 2];
  unsigned int j = 0;
  int sizeOutput = 0;

  (void)outputBuffer;
  (void)framesPerBuffer;
  (void)timeInfo;
  (void)statusFlags;

  {
    std::unique_lock<std::mutex> lock(mtx);

    for (unsigned int i = 0; i < FRAMES_PER_BUFFER ; ++i)
    {
      SAMPLE b = *input++;

      input2[j] = b;
      ++j;
      if (NUM_CHANNELS == 2)
      {
        b = *input++;
        input2[j] = b;
        ++j;
      }
    }

    unsigned char * encodeBuffer = other->getEncoder()->encodeAudio(input2, &sizeOutput);
    other->notifySubscribers(encodeBuffer, sizeOutput);
  }
  return paContinue;
}

int playBackSoundCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *samples)
{
  SAMPLE *output = (SAMPLE *)outputBuffer;
  std::vector<SAMPLE *> * otherSamples = (std::vector<SAMPLE *> *)samples;
  int j = 0;

  (void)statusFlags;
  (void)timeInfo;
  (void)framesPerBuffer;
  (void)inputBuffer;

  {
  std::unique_lock<std::mutex> lock(mtx);
  if (otherSamples->size() > 0)
  {
    SAMPLE * output2 = (*otherSamples)[0];
    for (unsigned int i = 0; i < FRAMES_PER_BUFFER ; ++i)
    {
      SAMPLE b = output2[j++];

      *output++ = b;
      if (NUM_CHANNELS == 2)
      {
        b = output2[j++];
        *output++ = b;
      }
    }
    otherSamples->erase(otherSamples->begin());
  }
   else {
      for (unsigned int i = 0 ; i < FRAMES_PER_BUFFER ; ++i)
      {
        *output++ = 0;
        if (NUM_CHANNELS == 2)
          *output++ = 0;
      }
    }
  }
  return paContinue;
}

void AudioEncapsuler::initStreams()
{
  if (_inputStream != NULL)
  {
      Pa_AbortStream(_inputStream);
      Pa_CloseStream(_inputStream);
  }
  if (_outputStream != NULL)
  {
      Pa_AbortStream(_outputStream);
      Pa_CloseStream(_outputStream);
  }
  _err = Pa_OpenStream(&_inputStream, &_inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordSoundCallback, this);
  if (_err != paNoError)
    {
      errorAudio();
      return;
    }
  _err = Pa_OpenStream(&_outputStream, NULL, &_outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playBackSoundCallback, this->samples);
  if (_err != paNoError)
    {
      errorAudio();
      return;
    }
}

void AudioEncapsuler::startStreams()
{
  _err = Pa_StartStream(_inputStream);
  if (_err != paNoError)
    {
      errorAudio();
      return;
    }

   _err = Pa_StartStream(_outputStream);
   if (_err != paNoError)
    {
      errorAudio();
      return;
    }
}

void AudioEncapsuler::errorAudio()
{
  if (_err != paNoError)
    {
      if (_inputStream != NULL)
	    {
	  Pa_AbortStream(_inputStream);
	  Pa_CloseStream(_inputStream);
	}
  if (_outputStream != NULL)
	{
	  Pa_AbortStream(_outputStream);
	  Pa_CloseStream(_outputStream);
	}
  Pa_Terminate();
  std::cerr << "An error occured while using the portaudio system." << std::endl;
  std::cerr << "Error number: " << _err << std::endl;
  std::cerr << "Error message: " << Pa_GetErrorText(_err) << std::endl;
  _err = 1;
    }
}

void AudioEncapsuler::stopStreams()
{
  this->_sizeRecBuffer = 0;
  this->_sizePlayBuffer = 0;
  _err = Pa_CloseStream(_inputStream);
  _err = Pa_CloseStream(_outputStream);
}

unsigned char * AudioEncapsuler::getRecBuffer() const
{
    return (unsigned char *)_recBuffer;
}

int AudioEncapsuler::getSizeRecBuffer() const
{
    return _sizeRecBuffer;
}

void AudioEncapsuler::addSizeRecBuffer(int value)
{
    _sizeRecBuffer += value;
}

EncodeEncapsuler * AudioEncapsuler::getEncoder() const
{
    return _encoder;
}

void AudioEncapsuler::setPlayBuffer(SAMPLE *sample)
{
  std::unique_lock<std::mutex> lock(mtx);
  samples->push_back(sample);
}

int AudioEncapsuler::getSizePlayBuffer() const
{
  return _sizePlayBuffer;
}

void AudioEncapsuler::setSizePlayBuffer(int sizePlayBuffer)
{
    _sizePlayBuffer = sizePlayBuffer;
}

unsigned char * AudioEncapsuler::getPlayBuffer() const
{
   return (unsigned char *)_playBuffer;
}

unsigned char * AudioEncapsuler::getData() const
{
  return (unsigned char *)_data;
}

void AudioEncapsuler::setData(unsigned char * data)
{
  _data = data;
}

int AudioEncapsuler::getSizeData() const
{
  return _sizeData;
}

void AudioEncapsuler::setSizeData(int size)
{
  _sizeData = size;
}

void
AudioEncapsuler::subscribeOnRecord(IAudioObserver *ob)
{
  obs.push_back(ob);
}

void
AudioEncapsuler::notifySubscribers(unsigned char const *audio, int size)
{
  for (unsigned int i = 0; i < obs.size(); i++)
    obs[i]->onRecordAudio(audio, size);
}

// std::vector<SAMPLE *> *
// AudioEncapsuler::getSamples()
// {
//   return &samples;
// }
