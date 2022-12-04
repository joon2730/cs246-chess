#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include <map>

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  GC gc2;
  unsigned long colours[10];
	unsigned int bitmap_width, bitmap_height;
	std::map<char, Pixmap*> imageMap; //pointers to loaded images (mapped by char, eg K,k,Q)
  unsigned int axis_width, axis_height;

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  // Available colours.
  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, AliceBlue, LightBlue};

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a string
  void drawString(int x, int y, std::string msg);

  // Creates a graphic context gc
  GC create_gc(Display* display, Window win, int reverse_video);

  // Draws a chess piece
  void drawPiece(char piece, int x, int y, int width, int height);

};

#endif
