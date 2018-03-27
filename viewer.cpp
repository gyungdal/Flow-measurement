#include "viewer.h"


Viewer::Viewer(): width(128), height(64) {
    frameBuffer = new uint8_t[this->height];
    for(size_t i = 0;i<this->width;i++){
        frameBuffer[i] = new uint8_t[this->width];
    }
}


Viewer::~Viewer() {
    for(size_t i = 0;i<this->width;i++){
        delete[] frameBuffer[i];
    }
    delete[] frameBuffer;
}