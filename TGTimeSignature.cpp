#include "TGTimeSignature.h"
#include "TGDuration.h"

TGTimeSignature::~TGTimeSignature()
{
#ifndef AUTOMATIC_DELETE
    delete denominator;
#endif
}

void TGTimeSignature::copy(TGTimeSignature *timeSignature){
    timeSignature->setNumerator(getNumerator());
    getDenominator()->copy(timeSignature->getDenominator());
}

bool TGTimeSignature::isEqual(TGTimeSignature *ts){
    return (getNumerator() == ts->getNumerator() && getDenominator()->isEqual(ts->getDenominator()));
}
