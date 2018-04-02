#include "viewer.h"


Viewer::Viewer() {
    u8g.firstPage();
}

Viewer::updatePage(){
    u8g.nextPage();
}