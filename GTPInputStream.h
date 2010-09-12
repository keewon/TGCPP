#ifndef GTP_INPUT_STREAM_H
#define GTP_INPUT_STREAM_H

#include "TGFactory.h"

#include <fstream>
#include <cstdio>
#define DEBUG printf

#define MAX_SUPPORTED_VERSION 4

class GTPInputStream // : public GTPFileFormat, TGInputStreamBase
{
protected:
    TGStdString version;
    int versionIndex;
    std::ifstream* stream;
    const char* versions[MAX_SUPPORTED_VERSION];

    TGFactory *factory;
    
public:
    GTPInputStream();
    void init(TGFactory *factory, std::ifstream *aStream);
    
protected:
    TGFactory *getFactory() { return factory; }
    bool isSupportedVersion(const char* aVersion);
    
public:
    bool isSupportedVersion();
    
protected:
    void readVersion();
    int read() ;
    int read(char* bytes, int len) ;
    int read(char* bytes, int off,int len) ;
    void skip(int bytes) ;
    unsigned char readUnsignedByte() ;
    signed char readByte() ;
    bool readBoolean() ;
    int readInt() ;
    long readLong() ;
    TGStdString readString(int size, int len) ;
    TGStdString readString(int length) ;
    TGStdString readStringInteger() ;
    TGStdString readStringByte(int size) ;
    TGStdString readStringByteSizeOfByte() ;
    TGStdString readStringByteSizeOfInteger() ;
    void close();
};

#endif
