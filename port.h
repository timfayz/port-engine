/*
 *  port Engine v0.1
 *  Copyright 2020 Timur Fayzrakhmanov <tim.fayzrakhmanov@gmail.com>
 */

#ifndef _PORT_H
#define _PORT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
// vendor
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//////////////////////////////////////////////////////////////////////////////
// API ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// + implemented
// - not implemented
// . in progress
// ? in doubt

// [Window] Object

// [Window] Management operations
// newWindow +
// open +
// close +
// exit +
// center +
// resize +
// clear +
// update +
// setClearColor +
// setTitle +
// setOpacity +
// setResizable +
// setPosition +
// setFullscreen -
// setLogicalSize +
// setPxRaw +

// [Window] Event handling operations
// wait +
// waitForClose +
// waitForKey +
// pollFor .

// [Window] Drawing operations
// drawSetColor +
// drawPx +
// drawPxRaw +
// drawLine .
// drawHorLine -
// drawVerLine -
// drawCirc -
// drawRect -
// drawFill -
// drawFillAll -

// [Window] Printing operations
// print +
// printSetFont +
// printSetSize +
// printSetColor +

// [Window] Screen operations
// scrSetResolution .

// [Window] Misc operations
// info +

typedef struct Window {
  // window
  SDL_Window *window;     // [SDL] representation of 'window'
  int w, h;               // window width, height

  // window states
  bool isClosed;          // if window is closed
  bool isCentered;        // if window is centered
  int posx, poxy;         // window x,y position on the screen

  // window rendering driver
  SDL_Renderer *renderer; // [SDL] OpenGL / Vulkan / Software render / etc.

  // window vbuffer
  SDL_Texture *texture;   // [SDL] representation of 'video buffer'

  // physical vbuffer
  char *buf;              // physical rendering surface (matches window size)
  int bufw, bufh;         // its width/height in pixels
  size_t bufSize;         // its size in bytes (bufw * bufh * 4)

  // logical vbuffer
  char *vbuf;             // logical rendering surface (<= window size)
                          // (upon render, interpolated to physical one)
  int vbufw, vbufh;       // its width/height in pixels
  size_t vbufSize;        // its size in bytes (vbufw * vbufh * 4)

  // printing
  TTF_Font *font;         // [SDL]
  SDL_Color fontColor;
  unsigned int fontSize;
  char *fontPath;

  // colors
  Uint32 drawColor;       // default drawing color
  Uint32 clearColor;      // raw 'wipe out' color

  // window commands
  void (*open)();
  void (*center)();
  void (*close)();
  void (*exit)();
  void (*resize)(int w, int h);
  void (*update)();
  void (*clear)();

  void (*setClearColor)(Uint32 rgb);
  void (*setTitle)(const char *);
  void (*setOpacity)(float);
  void (*setPosition)(int x, int y);
  void (*setResizable)(int yesNoToggle);
  void (*setBorder)(int yesNoToggle);
  void (*setPxRaw)(int x, int y, Uint32 px);
  void (*setLogicalSize)(int w, int h);
  void (*UnsetLogicalSize)();

  // window events
  void (*wait)(Uint32);
  void (*waitForClose)();
  void (*waitFor)(int flags);
  void (*pollFor)(int flags);

  // drawing commands
  void (*drawSetColor)(Uint32 rgb, Uint8 a);
  void (*drawLine)(int x1, int y1, int x2, int y2);
  void (*drawCirc)();
  void (*drawRect)();
  void (*drawPx)(int x, int y);
  void (*drawPxRaw)(int x, int y, Uint32 px);

  // printing commands
  void (*printSetFont)(const char * fontpath);
  void (*printSetFontSize)(Uint32 fontsize);
  void (*printSetColor)(Uint32 rgb, Uint8 a);
  void (*print)(const char* str, int x, int y);

  // misc
  void (*info)();

} Window;

Window *win; // trick to eliminate passing struct in `win->method(win, ...)`

//////////////////////////////////////////////////////////////////////////////
// MISC //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define max(x, y) ((x) >= (y) ? (x) : (y))

// setter toggles
#define yes     1
#define no      0
#define toggle -1

// bits & bytes twiddling
// ----------------------
// single byte manipulation
#define bitSet(bits,posn)     ((bits) |= (1UL << (posn)))
#define bitUnset(bits,posn)   ((bits) &= ~(1UL << (posn)))
#define bitToggle(bits,posn)  ((bits) ^= (1UL << (posn)))
#define bitRead(bits,posn)   (!!((bits) & (1UL << (posn))))

// reverse byte order in 32bits/4bytes block
#define byteSwap32(b) \
  (Uint32)(\
    ((Uint8)b << 24) | \
    ((Uint8)(b >> 8) << 16) | \
    ((Uint8)(b >> 16) << 8) | \
    ((Uint8)(b >> 24)))

// write N times 32bit BLK into DST
static void memSet32(Uint32 *dst, Uint32 blk, size_t n) {
  // calc the end address to be multiple by 32
  Uint32 *end = (Uint32 *)(dst + (n & ~31)); // 0
  // blk = byteSwap32(blk); // [TODO] reverse byte order if Big-endian
  while (dst != end) { // 128byte blit per cycle (32*4)
    dst[0] = blk; // 4byte write
    dst[1] = blk;
    dst[2] = blk;
    dst[3] = blk;
    dst[4] = blk;
    dst[5] = blk;
    dst[6] = blk;
    dst[7] = blk;
    dst[8] = blk;
    dst[9] = blk;
    dst[10] = blk;
    dst[11] = blk;
    dst[12] = blk;
    dst[13] = blk;
    dst[14] = blk;
    dst[15] = blk;
    dst[16] = blk;
    dst[17] = blk;
    dst[18] = blk;
    dst[19] = blk;
    dst[20] = blk;
    dst[21] = blk;
    dst[22] = blk;
    dst[23] = blk;
    dst[24] = blk;
    dst[25] = blk;
    dst[26] = blk;
    dst[27] = blk;
    dst[28] = blk;
    dst[29] = blk;
    dst[30] = blk;
    dst[31] = blk;
    dst += 32;
  }
  end += (n & 31); // add the rest of un-multiple to 32 blocks
  while(dst != end) *dst++ = blk; // copy
}

// colors & pixel conversion
// -------------------------
// predefined colors
#define clrWhite  (0xffffff)
#define clrBlack  (0x000000)
#define clrBlue   (0x1E69D2)

// get separate rbga 8bit channels from ARGB 32bit pixel
#define A8(rgb) ((Uint8)((rgb) >> 24))
#define R8(rgb) ((Uint8)((rgb) >> 16))
#define G8(rgb) ((Uint8)((rgb) >> 8))
#define B8(rgb) ((Uint8) (rgb))

// generate ARGB 32bit pixel format from:
#define pxFromRGB(rgb) \
  ((Uint32)(((255) << 24) | (rgb)))
#define pxFromRGB_A(rgb,a) \
  ((Uint32)(((a) << 24) | (rgb)))
#define pxFromRGBA(r,g,b,a) \
  ((Uint32)((Uint8)(a) << 24 | (Uint8)(r) << 16 | (Uint8)(g) << 8 | (Uint8)(b)))

// supported keys
// --------------
#define pressANY        (-1)
#define pressCLOSE      (1)       // close button
#define pressENTER      (1 << 1)
#define pressRIGHT      (1 << 2)

//////////////////////////////////////////////////////////////////////////////
// ERROR HANDLING ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// assert or output an error message
#define assertWithMsg(exp, msg) \
  if ((exp) == false) { \
    printf("[error] %s:%d:%s() %s\n", __FILE__, __LINE__, __func__, msg); \
    exit(EXIT_FAILURE); \
  }

// assert or output an SDL error
#define assertWithSDLErr(exp) \
  if ((exp) == false) { \
    printf("[error][SDL] %s:%d:%s() %s\n", __FILE__, __LINE__, __func__, SDL_GetError()); \
    exit(EXIT_FAILURE); \
  }

// assert a window doesn't outsize the screen dimensions
// [!] buffer (logical) size != window size != screen/display size
#define assertWinSizeFitsScreen(wd, hg) \
  SDL_DisplayMode dm; \
  assertWithSDLErr(SDL_GetCurrentDisplayMode(0, &dm) == 0); \
  if ((wd) > dm.w || (wd) <= 0 || (hg) > dm.h || (hg) <= 0) { \
    char s[92]; \
    snprintf(s, 92, "window size cannot be bigger than the current display resolution (%dx%d) or equal to zero\n", dm.w, dm.h); \
    assertWithMsg(0, s); \
  }

//////////////////////////////////////////////////////////////////////////////
// DYNAMIC ARRAY /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// "Exported" API

// pushes an element to buffer
#define bufPush(b, ...) (bufMustFit(b, 1), b[bufGetHdr(b)->len++] = (__VA_ARGS__))
// return current length
#define bufLen(b) ((b) ? bufGetHdr(b)->len : 0)
// return maximum capacity
#define bufCap(b) ((b) ? bufGetHdr(b)->cap : 0)
// return address right after the last containing element
#define bufEnd(b) ((b) + bufLen(b))
// free memory for buffer if it is not NULL
#define bufFree(b) ((b) ? (free(bufGetHdr(b)), (b) = NULL) : 0)

// "Private" implementation

// dynamic array structure
typedef struct BufHdr {
  size_t len;
  size_t cap;
  char buf[0]; // here can be anything
} BufHdr;

// get "hidden behind" address of the buffer header
#define bufGetHdr(b) ((BufHdr *)((char *)b - offsetof(BufHdr, buf)))
// check if current capacity can fit requested n elements (and what inside already)
#define bufCanFit(b, n) (bufLen(b) + (n) <= bufCap(b))
// check if the buffer can fit new elements otherwise expand
#define bufMustFit(b, n) \
  (bufCanFit(b, n) ? 0 : ((b) = bufNewOrExpand(b, bufLen(b) + n, sizeof(*b))))

void *bufNewOrExpand(const void *buf, size_t new_len, size_t elem_size) {
  // check if we can double the current capacity without number overflow
  assert(bufCap(buf) <= (SIZE_MAX - 1) / 2); // <--> (1 + 2*bufCap < SIZE_MAX)
  // check if the requested length is more than the double size of curr capacity
  size_t new_cap = max(1 + 2 * bufCap(buf), new_len);
  assert(new_len <= new_cap);
  // check if we can calc new_size (in bytes) without number overflow
  assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf)) / elem_size);
  // ^ if it's true == it's not possible to wrap around during the calc
  size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size;
  BufHdr *new_hdr;
  if (buf) {
    new_hdr = (BufHdr *)realloc(bufGetHdr(buf), new_size);
    assertWithMsg(new_hdr != NULL, "failed to allocate additional memory fo dynamic buffer");
  } else {
    new_hdr = (BufHdr *)malloc(new_size);
    assertWithMsg(new_hdr != NULL, "failed to allocate memory for dynamic buffer");
    new_hdr->len = 0;
  }
  new_hdr->cap = new_cap;
  return new_hdr->buf;
}

//////////////////////////////////////////////////////////////////////////////
// PRINT /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// [TODO] implement oldschool bitmap fonts loading
static void port_printSetFont(const char* fontpath) {
  assertWithMsg(fontpath != NULL, "fontpath shouldn't be empty");
  assertWithMsg(win->fontSize > 0, "set print size first");

  // reset previously loaded font if the new one doesn't match
  if (win->font != NULL && strcmp(win->fontPath, fontpath) != 0) {
    TTF_CloseFont(win->font);
    win->font = NULL;
  }

  // init SDL's TTF module if wasn't initalized
  if (!TTF_WasInit()) assertWithMsg(TTF_Init() != -1, TTF_GetError());
  TTF_Font *font = TTF_OpenFont(fontpath, win->fontSize);
  assertWithMsg(font != 0, "cannot find or load the font");
  win->font = font;
  win->fontPath = (char *)fontpath;
}

static void port_printSetFontSize(unsigned int ptsize) {
  win->fontSize = ptsize;
}

static void port_printSetColor(Uint32 rgb, Uint8 alpha) {
  win->fontColor = (SDL_Color){R8(rgb), G8(rgb), B8(rgb), alpha};
}

static void port_print(const char* str, int x, int y) {
  assertWithMsg(win->font != NULL, "specify font, size and color first");

  SDL_Surface *surface = TTF_RenderText_Blended(win->font, str, win->fontColor);
  assertWithSDLErr(surface != NULL);

  // [TODO] upscaling from logical to actual buffer
  // use port_interpolateOnto(surface->pixels, win->buf, surface->w, ...)

  SDL_Texture *texture;
  texture = SDL_CreateTextureFromSurface(win->renderer, surface);
  assertWithSDLErr(texture != NULL);

  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = surface->w;
  rect.h = surface->h;
  SDL_FreeSurface(surface);

  SDL_RenderCopy(win->renderer, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
  SDL_RenderPresent(win->renderer);
}

//////////////////////////////////////////////////////////////////////////////
// DRAWING ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static void port_drawSetColor(Uint32 rgb, Uint8 a) {
  win->drawColor = pxFromRGB_A(rgb, a);
}

// [TODO] buf implementation
static void port_drawLine(int x1, int y1, int x2, int y2) {
  // SDL_RenderDrawLine(win->renderer, x1, y1, x2, y2);
  // SDL_RenderPresent(win->renderer);

}

// #define setPxRaw(win, x, y, px) \
//   // calculate offset (location)
//   unsigned int offs = (win)->vbufw * (y) + (x); \
//   // fill px with drawColor
//   *((Uint32 *)(win)->vbuf + offs) = (px); \

static void port_drawPx(int x, int y) {
  // locate px
  unsigned int offs = win->vbufw * y + x;
  // fill px with drawColor
  *((Uint32 *)win->vbuf + offs) = win->drawColor;
  // render px
  win->update();

  // [TODO] add blend mode to do manual alpha calculations
  // win->buf[offs]     = B8(win->drawColor);
  // win->buf[offs + 1] = G8(win->drawColor);
  // win->buf[offs + 2] = R8(win->drawColor);
  // win->buf[offs + 3] = A8(win->drawColor);
}

static void port_drawPxRaw(int x, int y, Uint32 px) {
  // locate px
  unsigned int offs = win->vbufw * y + x;
  // fill px with drawColor
  *((Uint32 *)win->vbuf + offs) = px;
  // render px
  win->update();
}

//////////////////////////////////////////////////////////////////////////////
// VBUFFER ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// resize src buffer onto destination one applying interpolation
static void port_interpolateOnto(Uint32 *src, Uint32 *dst,
  int srcw, int srch, int dstw, int dsth) {

  float scaleX = (float)dstw/srcw; // how many srcw fits into dstw
  float scaleY = (float)dsth/srch; // the same way

  // nearest-neighbor interpolation algorithm
  for (int y = 0; y < dsth; y++) {   // iterate over each dst row
    for (int x = 0; x < dstw; x++) { // iterate over each dst column
      *(dst + (x + y * dstw)) =
        src[(int)floor(x/scaleX) + (int)floor(y/scaleY) * srcw];
    }
  }
}


//////////////////////////////////////////////////////////////////////////////
// WINDOW OPERATIONS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static void port_open() {
  SDL_ShowWindow(win->window);
  win->isClosed = false;
}

static void port_close() {
  SDL_HideWindow(win->window);
  win->isClosed = true;
}

static void port_minimize() {
  SDL_MinimizeWindow(win->window);
  win->isClosed = true;
}

static void port_restore() {
  SDL_RestoreWindow(win->window);
  win->isClosed = false;
}

static void port_exit() {
  if (win != NULL) {
    SDL_DestroyWindow(win->window);
    win->window = NULL;
    SDL_DestroyTexture(win->texture);
    win->texture = NULL;
    SDL_DestroyRenderer(win->renderer);
    win->renderer = NULL;
    free(win->buf);
    win->buf = NULL;
    free(win);
    win = NULL;
  }
  SDL_Quit(); //destroy SDL_Init's subsystems
}

static void port_center() {
  SDL_SetWindowPosition(win->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  win->isCentered = true;
}

static void port_wait(Uint32 ms) {
  SDL_Delay(ms);
}

// clear buffer with predefined color
static void port_clear() {
  // fill the buffer with clearColor
  memSet32((Uint32 *)(win->vbuf), win->clearColor, win->vbufSize/4);
}

// dynamic window resize
static void port_resize(int w, int h) {
  // assert new window size does not oversize the screen
  assertWinSizeFitsScreen(w, h);

  // if logical size is set, assert new window size is >= the logical size
  if (win->vbuf != win->buf) {
    assertWithMsg(w >= win->vbufw && h >= win->vbufh, "new window size cannot be less than vbuffer logical size. If required, use UnsetLogicalSize() first.");
  }

  // [bug] the line below doesn't work, thus..
  // SDL_SetWindowSize(win->window, w, h);

  // copy old window title
  const char *tmp = SDL_GetWindowTitle(win->window);
  char oldtitle[strlen(tmp)+1];
  strcpy(oldtitle, tmp);

  // copy old window flags
  int oldflags = SDL_GetWindowFlags(win->window);

  // copy old window position
  int oldx, oldy;
  if (win->isCentered) {
    oldx = SDL_WINDOWPOS_CENTERED;
    oldy = SDL_WINDOWPOS_CENTERED;
  } else {
    SDL_GetWindowPosition(win->window, &oldx, &oldy);
  }

  // destroy underlying window structures
  SDL_DestroyTexture(win->texture);   // remove old texture
  SDL_DestroyRenderer(win->renderer); // remove old renderer
  SDL_DestroyWindow(win->window);     // remove old window

  // create new window
  win->window = SDL_CreateWindow(oldtitle, oldx, oldy, w, h, oldflags);
  assertWithSDLErr(win->window != 0);
  win->w = w;
  win->h = h;

  // attach new renderer
  win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
  assertWithSDLErr(win->renderer != 0);

  // create new texture
  SDL_Texture* texture =
      SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, w, h);
  win->texture = texture;

  // save previous vbuffer representation
  Uint32 *oldbuf = (Uint32 *)win->buf;  // save prev buffer content
  int oldw = win->bufw;                 // save prev buffer width
  int oldh = win->bufh;                 // save prev buffer width

  // create new physical vbuffer representation
  win->bufw = w;
  win->bufh = h;
  win->bufSize = (size_t)(w * h * 4);
  win->buf = (char *)malloc(win->bufSize);
  assertWithMsg(win->buf != NULL, "failed to reallocate memory for physical buffer")

  // interpolate old buffer onto a new one
  port_interpolateOnto(oldbuf, (Uint32 *)win->buf,
    oldw, oldh, win->bufw, win->bufh);

  // remove old buffer
  free(oldbuf);

  // present a new one if the window is not closed
  if (win->isClosed) return;
  win->update();
}

// render vbuffer to the screen
static void port_update() {
  // if logical buffer is present, interpolate it onto actual one
  if (win->vbuf != win->buf) {
    port_interpolateOnto((Uint32 *)win->vbuf, (Uint32 *)win->buf,
      win->vbufw, win->vbufh,
      win->bufw, win->bufh);
  }
  // (!) texture and rendering buffer sizes are always the same
  SDL_UpdateTexture(win->texture, NULL, win->buf, win->bufw * 4);
  // deliver vbuffer to the rendering target (through SDL texture)
  SDL_RenderCopy(win->renderer, win->texture, NULL,
    // &(SDL_Rect){0, 0, win->bufw, win->bufh} // already match (redudant)
    NULL
  );

  // flip vbuffer
  SDL_RenderPresent(win->renderer);
}

static void port_setPxRaw(int x, int y, Uint32 px) {
  // locate px
  unsigned int offs = win->vbufw * y + x;
  // fill px with drawColor
  *((Uint32 *)win->vbuf + offs) = px;
}

// set window logical size
static void port_setLogicalSize(int w, int h) {
  assertWithMsg((h <= win->bufh && w <= win->bufw) && (h != 0 && w != 0),
  "logic size cannot be 0 and must be less or equal to the current window size");
  win->vbufw = w;
  win->vbufh = h;
  win->vbufSize = w * h * 4;
  win->vbuf = (char *)calloc(1, win->vbufSize);
  assertWithMsg(win->vbuf != NULL, "failed to allocate memory for logical buffer");

  // [optional] possibly use SDL functionality instead
  // SDL_RenderSetScale(win->renderer, 1.0, 1.0);
  // SDL_RenderSetLogicalSize(win->renderer, win->w, win->h);
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

// set window logical size to match physical dimensions
static void port_UnsetLogicalSize() {
  if (win->vbuf == win->buf) return; // nothing to do

  // destroy logical rendering surface
  free(win->vbuf);
  win->vbufw = 0;
  win->vbufh = 0;
  win->vbufSize = 0;

  // set logical vbuffer to point at physical one
  win->vbuf = win->buf;
  win->vbufw = win->bufw;
  win->vbufh = win->bufh;
  win->vbufSize = win->bufSize;
}


// set clear color for buffer
static void port_setClearColor(Uint32 rgb) {
  win->clearColor = pxFromRGB(rgb);
}

static void port_setOpacity(float opacity) {
  SDL_SetWindowOpacity(win->window, opacity);
}

static void port_setTitle(const char *title) {
  SDL_SetWindowTitle(win->window, title);
}

static void port_setPosition(int x, int y) {
  SDL_SetWindowPosition(win->window, x, y);
  win->isCentered = false;
}

static void port_setResizable(int flag) {
  if (flag == yes) {
    SDL_SetWindowResizable(win->window, SDL_TRUE);
  } else if(flag == no) {
    SDL_SetWindowResizable(win->window, SDL_FALSE);
  } else { // flag == toggle
    Uint32 flags = SDL_GetWindowFlags(win->window);
    SDL_SetWindowResizable(win->window, (SDL_bool)((flags & SDL_WINDOW_RESIZABLE) ? false : true));
  }
}

static void port_setBorder(int flag) {
  if (flag == yes) {
    SDL_SetWindowBordered(win->window, SDL_TRUE);
  } else if(flag == no) {
    SDL_SetWindowBordered(win->window, SDL_FALSE);
  } else { // flag == toggle
    Uint32 flags = SDL_GetWindowFlags(win->window);
    SDL_SetWindowBordered(win->window, (SDL_bool)((flags & SDL_WINDOW_BORDERLESS) ? true : false));
  }
}

// [TODO] set to fullscreen mode
static void port_setFullscreen() {
  SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN_DESKTOP); // then change res
  // SDL_SetWindowFullscreen(win->window, SDL_WINDOW_FULLSCREEN); // then change res
  // win->setDisplayMode(win, w, h)
  // SDL_DisplayMode d = {SDL_GetWindowPixelFormat(win->window), 1440, 810, 0, 0};
  // SDL_SetWindowDisplayMode(win->window, &d);
}

//////////////////////////////////////////////////////////////////////////////
// WINDOW EVENTS /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// wait for a keypress
static void port_waitFor(int flags) {
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    while(SDL_PollEvent(&e)) {
      switch (e.type) {
      case (SDL_KEYDOWN):
        if(  flags == pressANY ||
          !!(flags & pressENTER) && SDLK_RETURN == e.key.keysym.sym ||
          !!(flags & pressRIGHT) && SDLK_RIGHT == e.key.keysym.sym // ...
          ) {
          quit = true;
          break;
        }
      case (SDL_QUIT):
        if(!!(flags & pressCLOSE)) {
          quit = true;
          break;
        }
      }
    }
    SDL_Delay(100);
  }
}

// close window on any keypress or window close
static void port_waitForClose() {
  SDL_Event e;
  while (true) {
    SDL_PollEvent(&e);
    if (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) break;
    SDL_Delay(100);
  }
  win->close();
}

static void port_pollFor(int flags) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case (SDL_QUIT):
        win->isClosed = true;
        return;
      case (SDL_WINDOWEVENT):
        switch(e.window.event) {
          case (SDL_WINDOWEVENT_RESIZED):
            // if (flags & SCREEN_RESIZED) {
              printf("resized to %dx%d\n", e.window.data1, e.window.data2);
              // e.window.data1 // width
              // e.window.data2 // height
            // }
        }
        break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// PORT INIT /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Window *newWindow(int width, int height) {

  win = (Window *)malloc(sizeof(Window));
  assertWithMsg(win != 0, "failed to allocate memory for Window structure");

  // init SDL subsystems
  assertWithSDLErr(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);
  assertWinSizeFitsScreen(width, height);

  // create a hidden centered window
  SDL_Window *window = SDL_CreateWindow("",
                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_HIDDEN);
  assertWithSDLErr(window != 0);
  win->window = window;
  win->w = width;
  win->h = height;
  win->isCentered = true;

  // create renderer for the window
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  assertWithSDLErr(renderer != 0);
  win->renderer = renderer;

  // [future] use SDL's unlocked texture instead of ..

  // (default) window size matches physical and logical buffer sizes
  win->vbufw = win->bufw = width;
  win->vbufh = win->bufh = height;
  win->vbufSize = win->bufSize =
    (size_t)(height * width * 4); // 4 color channels, 1 byte per each
  win->vbuf = win->buf = (char *)calloc(sizeof(char), win->bufSize);
  assertWithMsg(win->buf != 0, "failed to allocate memory for video buffer");

  // (default) size of physical buffer == size of texture
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, win->bufw, win->bufh);
  assertWithSDLErr(texture != 0);
  win->texture = texture;

  // set alpha blending
  assertWithSDLErr(SDL_SetTextureBlendMode(win->texture, SDL_BLENDMODE_BLEND) == 0);

  // intialize methods ..

  // window main commands
  win->open = port_open;
  win->close = port_close;
  win->exit = port_exit;
  win->center = port_center;
  win->update = port_update;
  win->clear = port_clear;

  win->setClearColor = port_setClearColor;
  win->setTitle = port_setTitle;
  win->setOpacity = port_setOpacity;
  win->setResizable = port_setResizable;
  win->setBorder = port_setBorder;
  win->setPosition = port_setPosition;
  win->setPxRaw = port_drawPxRaw;
  win->setLogicalSize = port_setLogicalSize;
  win->UnsetLogicalSize = port_UnsetLogicalSize;

  // windows events
  win->wait = port_wait;
  win->waitFor = port_waitFor;
  win->waitForClose = port_waitForClose;
  win->pollFor = port_pollFor;

  // printing commands
  win->printSetFont = port_printSetFont;
  win->printSetFontSize = port_printSetFontSize;
  win->printSetColor = port_printSetColor;
  win->print = port_print;

  // drawing commands
  win->drawSetColor = port_drawSetColor;
  win->drawLine = port_drawLine;
  win->drawPx = port_drawPx;
  win->drawPxRaw = port_drawPxRaw;

  return win;
}

//////////////////////////////////////////////////////////////////////////////
// BENCHMARKING //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef struct Measure {
  Uint64 start, end, frames;
} Measure;

static Measure *measureStart() { // and Stop()
  Measure *m = (Measure *)calloc(1, sizeof(Measure));
  m->start = SDL_GetPerformanceCounter();
  return m;
}

static void measureEnd(Measure *m) {
  m->end = SDL_GetPerformanceCounter();
  double elapsed = (double)(m->end - m->start) / SDL_GetPerformanceFrequency();
  printf("[info] elapsed %.0fms, total frames %ld, FPS %.0f\n",
    elapsed * 1000,
    m->frames,
    m->frames/(elapsed)
  );
  // [addon] capping to 60 FPS
	// SDL_Delay(floor(16.666f - elapsed));
}

static void port_info() {
  #ifdef linux
    printf("[info] port is started.. (pid %d)\n", getpid());
  #endif
  SDL_RendererInfo info;
  SDL_GetRendererInfo(win->renderer, &info);
  printf("[info][SDL] current video driver: %s\n", SDL_GetCurrentVideoDriver());
  printf("[info][SDL] renderer name: %s\n", info.name);
  printf("[info][SDL] supported texture formats:\n");
  for (uint i = 0; i < info.num_texture_formats; i++) {
      printf(" %s\n", SDL_GetPixelFormatName(info.texture_formats[i]));
  }
}


#endif
