#ifndef TG_STRING_H
#define TG_STRING_H

#include "TGFactory.h"

class TGString 
{
protected:
    int number;
    int value;
    
public:
    TGString(){
        number = 0;
        value = 0;
    }
    
    int getNumber() {
        return number;
    }
    
    int getValue() {
        return value;
    }
    
    void setNumber(int aNumber) {
        number = aNumber;
    }
    
    void setValue(int aValue) {
        value = aValue;
    }
    
    bool isEqual(TGString *aString){
        return (getNumber() == aString->getNumber() && getValue() == aString->getValue());
    }
    
    TGString* clone(TGFactory* factory){
        TGString *string = factory->newString();
        copy(string);
        return string;
    }
    
    void copy(TGString *aString){
        aString->setNumber(getNumber());
        aString->setValue(getValue());
    }
    
};

#endif
