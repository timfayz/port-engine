#include "port.h"

// Minimal working example:
// blue screen with a white falling pixel that slowly stops
int main() {
  Window *w = newWindow(128, 128);// create port instance/window

  w->setLogicalSize(16, 16);      // set each 1x1 pixel to be 16x16 of physical
  w->setBorder(no);               // set 'yes' to see standard window borders
  w->open();

  w->setClearColor(clrBlue);
  w->drawSetColor(clrWhite, 255);
  for (int i = 0; i < 100; i++) { // 1 cycle = 1 frame
    w->clear();                   // clear previous frame
    w->drawPx(8, i % 16);         // fill px with color
    w->wait(10+i);                // not too fast!
  }

  w->waitForClose();              
}