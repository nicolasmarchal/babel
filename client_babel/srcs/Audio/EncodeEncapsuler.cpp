#include "EncodeEncapsuler.hh"

EncodeEncapsuler::EncodeEncapsuler()
{
    _encoder = opus_encoder_create(SAMPLE_RATE, NUM_CHANNELS, OPUS_APPLICATION_VOIP, &_err);
    _decoder = opus_decoder_create(SAMPLE_RATE, NUM_CHANNELS, &_err);
    opus_encoder_ctl(_encoder, OPUS_GET_BANDWIDTH(&_size));

}

EncodeEncapsuler::~EncodeEncapsuler()
{
    opus_encoder_destroy(_encoder);
    opus_decoder_destroy(_decoder);
    delete _output;
}

SAMPLE * EncodeEncapsuler::decodeAudio(const unsigned char * toDecode, int sizeToDecode)
{
    SAMPLE * output = new SAMPLE[FRAMES_PER_BUFFER * NUM_CHANNELS];

    std::memset(output, 0, NUM_CHANNELS * FRAMES_PER_BUFFER);
    if (opus_decode_float(_decoder, toDecode, sizeToDecode, output, FRAMES_PER_BUFFER, 0) <= 0)
      return (NULL);
    return (output);
}

unsigned char * EncodeEncapsuler::encodeAudio(const SAMPLE * toEncode, int *sizeOutput)
{
    _output = new unsigned char[FRAMES_PER_BUFFER];
    if ((*sizeOutput = opus_encode_float(_encoder, toEncode, FRAMES_PER_BUFFER, _output, (opus_int32)FRAMES_PER_BUFFER)) <= 0)
      return (NULL);
    return _output;
}
