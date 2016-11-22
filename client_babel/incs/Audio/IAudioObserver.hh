#pragma once

class IAudioObserver
{
public:
  virtual void onRecordAudio(unsigned char const *audio, int size) = 0;
};

// typedef void (IAudioObserver::*IAudioObserverFunc)(unsigned char const *);
