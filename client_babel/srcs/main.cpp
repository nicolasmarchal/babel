#include "AudioEncapsuler.hh"

int main()
{
  AudioEncapsuler * audio = new AudioEncapsuler;

  audio->initialize();
  audio->startStreams();
  while (1);
}
