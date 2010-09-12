#ifndef TG_CHANNEL_H
#define TG_CHANNEL_H

#include "TGFactory.h"

#define TGChannel_DEFAULT_PERCUSSION_CHANNEL 9
#define TGChannel_DEFAULT_INSTRUMENT  25
#define TGChannel_DEFAULT_VOLUME  127
#define TGChannel_DEFAULT_BALANCE  64
#define TGChannel_DEFAULT_CHORUS  0
#define TGChannel_DEFAULT_REVERB  0
#define TGChannel_DEFAULT_PHASER  0
#define TGChannel_DEFAULT_TREMOLO  0

class TGChannel 
{
protected:
    short channel;
    short effectChannel;
    short instrument;
    short volume;
    short balance;
    short chorus;
    short reverb;
    short phaser;
    short tremolo;
    
public:
    TGChannel() {
        channel = 0;
        effectChannel = 0;
        instrument = TGChannel_DEFAULT_INSTRUMENT;
        volume = TGChannel_DEFAULT_VOLUME;
        balance = TGChannel_DEFAULT_BALANCE;
        chorus = TGChannel_DEFAULT_CHORUS;
        reverb = TGChannel_DEFAULT_REVERB;
        phaser = TGChannel_DEFAULT_PHASER;
        tremolo = TGChannel_DEFAULT_TREMOLO;
    }
    
    short getBalance() {
        return balance;
    }
    
    void setBalance(short _balance) {
        balance = _balance;
    }
    
    short getChannel() {
        return channel;
    }
    
    void setChannel(short _channel) {
        channel = _channel;
    }
    
    short getEffectChannel() {
        return effectChannel;
    }
    
    void setEffectChannel(short _effectChannel) {
        effectChannel = _effectChannel;
    }
    
    short getChorus() {
        return chorus;
    }
    
    void setChorus(short _chorus) {
        chorus = _chorus;
    }
    
    short getInstrument() {
        return (!isPercussionChannel()?instrument:0);
    }
    
    void setInstrument(short _instrument) {
        instrument = _instrument;
    }
    
    short getPhaser() {
        return phaser;
    }
    
    void setPhaser(short _phaser) {
        phaser = _phaser;
    }
    
    short getReverb() {
        return reverb;
    }
    
    void setReverb(short _reverb) {
        reverb = _reverb;
    }
    
    short getTremolo() {
        return tremolo;
    }
    
    void setTremolo(short _tremolo) {
        tremolo = _tremolo;
    }
    
    short getVolume() {
        return volume;
    }
    
    void setVolume(short _volume) {
        volume = _volume;
    }
    
    bool isPercussionChannel(){
        return (channel == TGChannel_DEFAULT_PERCUSSION_CHANNEL);
    }
    
    void setPercusionChannel(){
        setChannel(TGChannel_DEFAULT_PERCUSSION_CHANNEL);
        setEffectChannel(TGChannel_DEFAULT_PERCUSSION_CHANNEL);
    }
    
    static TGChannel* newPercusionChannel(TGFactory* factory){
        TGChannel* channel = factory->newChannel();
        channel->setPercusionChannel();
        return channel;
    }
    
    TGChannel *clone(TGFactory* factory){
        TGChannel *channel = factory->newChannel();
        copy(channel);
        return channel; 
    }
    
    void copy(TGChannel *channel){
        channel->setChannel(getChannel());
        channel->setEffectChannel(getEffectChannel());
        channel->setInstrument(getInstrument());
        channel->setVolume(getVolume());
        channel->setBalance(getBalance());
        channel->setChorus(getChorus());
        channel->setReverb(getReverb());
        channel->setPhaser(getPhaser());
        channel->setTremolo(getTremolo());
    }
};

#endif
