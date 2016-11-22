#ifndef ENCODEENCAPSULER_HH_
# define ENCODEENCAPSULER_HH_

#include <cstdlib>
#include <cstring>
#include "opus.h"
#include "opus_types.h"
#include "opus_defines.h"
#include "audiosettings.h"

class EncodeEncapsuler
{
    public:
        EncodeEncapsuler();
        ~EncodeEncapsuler();
    public:
        unsigned char * encodeAudio(const SAMPLE * toEncode, int * sizeOutput);
        SAMPLE * decodeAudio(const unsigned char * toDecode, int sizeToDecode);

    private:
        int _err;
        opus_int32 _size;
        OpusEncoder * _encoder;
        OpusDecoder * _decoder;
        unsigned char * _output;
};

#endif
