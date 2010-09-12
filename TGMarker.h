#ifndef TG_MARKER_H
#define TG_MARKER_H

#include "TGFactory.h"
#include "TGColor.h"

class TGMarker 
{
protected:
    const static TGColor DEFAULT_COLOR = COLOR_RED;
    
    int measure;
    TGStdString title;
    TGColor color;
    
public:
    TGMarker(TGFactory* factory) {
        measure = 0;
        title = "Untitled";
        color = DEFAULT_COLOR;
    }
    
    int getMeasure() {
        return measure;
    }
    
    void setMeasure(int _measure) {
        measure = _measure;
    }
    
    TGStdString& getTitle() {
        return title;
    }
    
    void setTitle(TGStdString _title) {
        title = _title;
    }
    
    TGColor& getColor() {
        return color;
    }
    
    void setColor(TGColor _color) {
        color = _color;
    }
    
    TGMarker* clone(TGFactory* factory){
        TGMarker *marker = factory->newMarker();
        marker->setMeasure(getMeasure());
        marker->setTitle(getTitle());
        marker->setColor(getColor());
        return marker;
    }
    
};

#endif
