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
	GC gc, gc2;
	unsigned long colours[14];
	int width, height;
	Pixmap tile_stipple_light;
	Pixmap tile_stipple_dark;
	unsigned int bitmap_width, bitmap_height;
	std::map<char, Pixmap*> imageMap; //pointers to loaded images (mapped by char, eg K,k,Q)
  	unsigned int axis_width, axis_height;
    std::string directory = "../img/";

  public:
	Xwindow(int width=800, int height=800);  // Constructor; displays the window.
	~Xwindow();                              // Destructor; destroys the window.

	enum { White = 0, Black, Red, Green, Blue, Cyan, Yellow, Magenta,
		Orange, Brown, DimGray, LightBlue, SteelBlue, AliceBlue }; // Available colours.

	// Draws a string
	void drawString(int x, int y, std::string msg, int colour = Black);

	// Draws a bigger
	void drawBigString(int x, int y, std::string msg, int colour = Black);

	// Draws a rectangle
	void fillRectangle(int x, int y, int width, int height, int colour=Black);

	// Draws a patterned chessboard tile (dark/light).
	void fillTile(int x, int y, int width, int height, bool dark);

	// Draws a chess piece
	void drawPiece(char piece, int x, int y, int width, int height, bool darkTile);

  private:
	// Creates a graphic context gc
	GC create_gc(Display* display, Window win, int reverse_video);

	// if you use this function, make sure font exists on the undergrad environment
	void drawStringFont(int x, int y, std::string msg, std::string font, int colour = Black);

  	void showAvailableFonts();

	void printMessage(int x, int y, const std::string& msg, int colour, XFontStruct& f);
};

#endif

