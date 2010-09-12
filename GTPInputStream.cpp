
#include "GTPInputStream.h"
#include <algorithm>
#include <cstring>

GTPInputStream::GTPInputStream() : stream(NULL)
{
    for (int i=0; i<MAX_SUPPORTED_VERSION; ++i)
    {
        versions[i] = NULL;
    }
    versionIndex = -1;
}

void GTPInputStream::init(TGFactory *aFactory, std::ifstream *aStream) {
    factory = aFactory;
    stream = aStream;
}

bool GTPInputStream::isSupportedVersion(const char* aVersion) {
    for (int i=0; i<MAX_SUPPORTED_VERSION; ++i)
    {
        if (versions[i] && 0 == strcmp( aVersion, versions[i] ) )
        {
            versionIndex = i;
            return true;
        }
    }
    return false;
}

bool GTPInputStream::isSupportedVersion(){
    readVersion();
    return isSupportedVersion(version.c_str());
}

void GTPInputStream::readVersion(){
    version = readStringByte(30); //, DEFAULT_VERSION_CHARSET);
}

int GTPInputStream::read() {
    return stream->get();
}

int GTPInputStream::read(char* bytes, int len) {
    stream->read(bytes, len);
    return stream->good()?len:0;
}

int GTPInputStream::read(char* bytes, int off,int len) {
    stream->seekg( off, std::ios_base::beg );
    stream->read(bytes, len);
    return stream->good()?len:0;
}

void GTPInputStream::skip(int bytes) {
    //stream->seekg(bytes, std::ios_base::cur);
    stream->ignore(bytes);
}

unsigned char GTPInputStream::readUnsignedByte() {
    int c = stream->get();
    return (c & 0xff);
}

signed char GTPInputStream::readByte() {
    return ((signed char) stream->get());
}

bool GTPInputStream::readBoolean() {
    return (stream->get() == 1);
}

int GTPInputStream::readInt() {
    unsigned char bytes[4] = {0,};
    unsigned int result = 0;
    stream->read((char*)bytes, 4);

    //DEBUG("readInt: %x %x %x %x\n", bytes[0], bytes[1], bytes[2], bytes[3]);
    for (int i=0; i<4; ++i)
    {
        unsigned int r = bytes[i];
        result |= (r << (i*8));
    }
    return *((int*)&result);
}

long GTPInputStream::readLong() {
    char bytes[8] = {0,};
    long result = 0;
    stream->read(bytes, 8);
    for (int i=0; i<8; ++i)
    {
        long r = bytes[i];
        result |= (r << (i*8));
    }
    return result;
}

TGStdString GTPInputStream::readString(int size, int len) {
    int s = size > 0 ? size : len;
    //char *bytes = new char[ s + 1 ];
    char bytes[s+1]; // C99
    stream->read(bytes, s);
    bytes[s] = 0;

    if (len >= 0 && len <= s)
        bytes[len] = 0;

    TGStdString result(bytes);
    //delete [] bytes;
    return result;
}

TGStdString GTPInputStream::readString(int length) {
    return readString(length, length);
}

TGStdString GTPInputStream::readStringInteger() {
    return readString( readInt() );
}

TGStdString GTPInputStream::readStringByte(int size) {
    return readString( size, readUnsignedByte());
}

TGStdString GTPInputStream::readStringByteSizeOfByte() {
    return readStringByte( (readUnsignedByte() - 1) );
}

TGStdString GTPInputStream::readStringByteSizeOfInteger() {
    return readStringByte( (readInt() - 1) );
}

void GTPInputStream::close() {
    stream->close();
}

