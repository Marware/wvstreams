/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Gzip encoder/decoder based on zlib.
 */
#ifndef __WVGZIP_H
#define __WVGZIP_H

#include "wvencoder.h"
#include "wvencoderstream.h"

/**
 * An encoder implementing Gzip encryption and decryption.
 */
struct z_stream_s;
class WvGzipEncoder : public WvEncoder
{
public:
    enum Mode {
        Deflate, // compress using deflate
        Inflate  // decompress using inflate
    };
    
    WvGzipEncoder(Mode _mode);
    virtual ~WvGzipEncoder();
    
protected:
    virtual bool _encode(WvBuffer &inbuf, WvBuffer &outbuf, bool flush);
    virtual bool _finish(WvBuffer &outbuf);

private:
    struct z_stream_s *zstr;
    WvInPlaceBuffer tmpbuf;
    Mode mode;

    void prepare(WvBuffer *inbuf);
    bool process(WvBuffer &outbuf, bool flush, bool finish);
};


#endif // __WVGZIP_H
