#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <cmath>
#include <cctype>
#include "window.h"

using namespace std;

const double pi = std::acos(-1);

Xwindow::Xwindow(int width, int height): width{width}, height{height} {

	d = XOpenDisplay(NULL);
	if (d == NULL) {
		cerr << "Cannot open display" << endl;
		exit(1);
	}
	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
			BlackPixel(d, s), WhitePixel(d, s));
	XSelectInput(d, w, ExposureMask | KeyPressMask);
	XMapRaised(d, w);

	Pixmap pix = XCreatePixmap(d,w,width,
			height,DefaultDepth(d,DefaultScreen(d)));
	gc = XCreateGC(d, pix, 0,(XGCValues *)0);

	XFlush(d);
	XFlush(d);


	// Set up colours.
	XColor xcolour;
	Colormap cmap;
	char color_vals[16][20] = {"white",  "black", "red",      "green",
		"blue",   "cyan",  "yellow",   "magenta",
		"orange", "brown", "dimgray", "lightsteelblue", "steelblue", "aliceblue",
		"lightseagreen", "sandybrown"};

	cmap=DefaultColormap(d,DefaultScreen(d));
	for(int i=0; i < 16; ++i) {
		XParseColor(d,cmap,color_vals[i],&xcolour);
		XAllocColor(d,cmap,&xcolour);
		colours[i]=xcolour.pixel;
	}

	XSetForeground(d,gc,colours[Black]);

	// Create stipple patterns for board tiles (8x8).
	// 1-bits use foreground, 0-bits use background (FillOpaqueStippled).
	static const unsigned char light_bits[8] = {
		0x11, 0x00, 0x44, 0x00, 0x11, 0x00, 0x44, 0x00  // subtle speckle
	};
	static const unsigned char dark_bits[8] = {
		0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55  // checker stipple
	};
	tile_stipple_light = XCreateBitmapFromData(d, w, reinterpret_cast<const char*>(light_bits), 8, 8);
	tile_stipple_dark  = XCreateBitmapFromData(d, w, reinterpret_cast<const char*>(dark_bits), 8, 8);

	// Make window non-resizeable.
	XSizeHints hints;
	hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
	hints.height = hints.base_height = hints.min_height = hints.max_height = height;
	hints.width = hints.base_width = hints.min_width = hints.max_width = width;
	XSetNormalHints(d, w, &hints);

	XSynchronize(d, True);

	XSelectInput(d, w, ExposureMask);
	XFlush(d);
	XEvent event;
	XNextEvent(d, &event); // Hang until the window is ready.
	XSelectInput(d, w, 0);

    /* allocate a new GC (graphics context) for drawing in the window. */
    gc2 = create_gc(d, w, 0);
    XSync(d, False);

    char piece_names[12] = {'K','Q','R','B','N','P','k','q','r','b','n','p'};
    string extension = ".xbm";
    string filename;

    bitmap_width = bitmap_height = 80;
    axis_width = 640;
    axis_height = 30;
    for (int i = 0; i < 12; ++i) {
        filename = piece_names[i];
        if (i < 6) {
            filename = directory + filename + "w" + extension;
        } else {
            filename = directory + filename + "b" + extension;
        }

        /* this variable will contain the ID of the newly created pixmap.    */
        Pixmap* bitmap = new Pixmap;

        /* these variables will contain the location of the hotspot of the   */
        /* loaded bitmap.                                                    */
        int hotspot_x, hotspot_y;
        hotspot_x = hotspot_y = 0;

        /* load the bitmap found in the file "icon.bmp", create a pixmap     */
        /* containing its data in the server, and put its ID in the 'bitmap' */
        /* variable.                                                         */
        int rc = XReadBitmapFile(d, w, filename.c_str(),
        &bitmap_width, &bitmap_height, bitmap, &hotspot_x, &hotspot_y);
        imageMap[piece_names[i]] = bitmap;

        switch (rc) {
        case BitmapOpenFailed:
            cerr << "XReadBitmapFile - could not open file: " << filename << endl;
            exit(1);
            break;
        case BitmapFileInvalid:
            cerr << "XReadBitmapFile - file " << filename << " doesn't contain a valid bitmap.\n";
            exit(1);
            break;
        case BitmapNoMemory:
            cerr << "XReadBitmapFile - not enough memory.\n";
            exit(1);
            break;
        }
    }

}

Xwindow::~Xwindow() {
	XFreeGC(d, gc);
	XFreePixmap(d, tile_stipple_light);
	XFreePixmap(d, tile_stipple_dark);
	for (auto &kv : imageMap) {
		if (kv.second) {
			XFreePixmap(d, *(kv.second));
			delete kv.second;
		}
	}
	XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
	XSetForeground(d, gc, colours[colour]);
	XFillRectangle(d, w, gc, x, y, width, height);
	XSetForeground(d, gc, colours[Black]);
}

void Xwindow::fillTile(int x, int y, int width, int height, bool dark) {
	// Softer blue scheme:
	// - Light tile: aliceblue base with lightsteelblue speckles
	// - Dark tile:  lightsteelblue base with steelblue checker texture
	const int fg = dark ? SteelBlue : LightBlue;
	const int bg = dark ? LightBlue : AliceBlue;
	Pixmap stipple = dark ? tile_stipple_dark : tile_stipple_light;

	XSetForeground(d, gc, colours[fg]);
	XSetBackground(d, gc, colours[bg]);
	XSetStipple(d, gc, stipple);
	XSetFillStyle(d, gc, FillOpaqueStippled);
	XSetTSOrigin(d, gc, x, y);
	XFillRectangle(d, w, gc, x, y, width, height);
	XSetFillStyle(d, gc, FillSolid);
	XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawHighlightBorder(int x, int y, int width, int height, int colour, int thickness) {
	if (thickness < 1) thickness = 1;
	if (thickness > width / 2) thickness = width / 2;
	if (thickness > height / 2) thickness = height / 2;

	// Top
	fillRectangle(x, y, width, thickness, colour);
	// Bottom
	fillRectangle(x, y + height - thickness, width, thickness, colour);
	// Left
	fillRectangle(x, y, thickness, height, colour);
	// Right
	fillRectangle(x + width - thickness, y, thickness, height, colour);
}

void Xwindow::drawString(int x, int y, string msg, int colour) {
  XFontStruct * f = XLoadQueryFont(d, "6x13");
	
	printMessage(x, y, msg, colour, *f); 

	delete f;
}

void Xwindow::drawStringFont(int x, int y, string msg, string font, int colour) {
  XFontStruct * f = XLoadQueryFont(d, font.c_str());
  
	if ( f == nullptr ){
		f = XLoadQueryFont(d, "6x13");
	}

	printMessage(x, y, msg, colour, *f);
	delete f;
}

void Xwindow::drawBigString(int x, int y, string msg, int colour) {
  // Font f = XLoadFont(d, "-*-helvetica-bold-r-normal--*-240-*-*-*-*-*");
  ostringstream name;
  name << "-*-helvetica-bold-r-*-*-*-240-" << width/10 << "-" << height/10 << "-*-*-*-*";

	drawStringFont(x, y, msg, name.str(), colour);
}

void Xwindow::printMessage(int x, int y, const string& msg, int colour, XFontStruct& f){
  XSetForeground(d, gc, colours[colour]);
  XTextItem ti;
  ti.chars = const_cast<char*>(msg.c_str());
  ti.nchars = msg.length();
  ti.delta = 0;
  ti.font = f.fid;
  XDrawText(d, w, gc, x, y, &ti, 1);
  XSetForeground(d, gc, colours[Black]);
  XFlush(d);
}

void Xwindow::showAvailableFonts() {
  int count;
  char** fnts = XListFonts(d, "*", 10000, &count);

  for (int i = 0; i < count; ++i) cout << fnts[i] << endl;
}

GC Xwindow::create_gc(Display* d, Window w, int reverse_video) {
	GC gc; /* handle of newly created GC.  */
	unsigned long valuemask = 0; /* which values in 'values' to  */
	/* check when creating the GC.  */
	XGCValues values; /* initial values for the GC.   */
	unsigned int line_width = 2; /* line width for the GC.       */
	int line_style = LineSolid; /* style for lines drawing and  */
	int cap_style = CapButt; /* style of the line's edje and */
	int join_style = JoinBevel; /*  joined lines.		*/
	int screen_num = DefaultScreen(d);

	gc = XCreateGC(d, w, valuemask, &values);
	if (!gc) {
		cerr << "XCreateGC:" << endl;
	}

	/* allocate foreground and background colors for this GC. */
	if (reverse_video) {
		XSetForeground(d, gc, WhitePixel(d, screen_num));
		XSetBackground(d, gc, BlackPixel(d, screen_num));
	} else {
		XSetForeground(d, gc, BlackPixel(d, screen_num));
		XSetBackground(d, gc, WhitePixel(d, screen_num));
	}

	/* define the style of lines that will be drawn using this GC. */
	XSetLineAttributes(d, gc, line_width, line_style, cap_style,
			join_style);

	/* define the fill style for the GC. to be 'solid filling'. */
	XSetFillStyle(d, gc, FillSolid);

	return gc;
}

void Xwindow::drawPiece(char piece, int x, int y, int width, int height, bool darkTile) {
	// Center the bitmap inside the tile.
	const int draw_x = x + (width / 2) - static_cast<int>(bitmap_width / 2);
	const int draw_y = y + (height / 2) - static_cast<int>(bitmap_height / 2);

	(void)darkTile;

	try {
		// Goal:
		// - 0-bits stay transparent (tile shows through)
		// - black pieces: draw as black silhouette
		// - white pieces: fill white (using filled mask) and overlay black outline
		const bool is_white_piece = ('A' <= piece && piece <= 'Z');
		const char fillMaskChar = static_cast<char>(std::tolower(static_cast<unsigned char>(piece)));    // *b.xbm
		const char outlineMaskChar = static_cast<char>(std::toupper(static_cast<unsigned char>(piece))); // *w.xbm

		if (is_white_piece) {
			// Fill pass (white) using the filled mask
			{
				Pixmap fillMask = *(imageMap.at(fillMaskChar));
				XSetClipMask(d, gc2, fillMask);
				XSetClipOrigin(d, gc2, draw_x, draw_y);
				XSetForeground(d, gc2, colours[White]);
				XFillRectangle(d, w, gc2, draw_x, draw_y, bitmap_width, bitmap_height);
				XSetClipMask(d, gc2, None);
			}
			// Outline pass (black) using the outline mask
			{
				Pixmap outlineMask = *(imageMap.at(outlineMaskChar));
				XSetClipMask(d, gc2, outlineMask);
				XSetClipOrigin(d, gc2, draw_x, draw_y);
				XSetForeground(d, gc2, colours[Black]);
				XFillRectangle(d, w, gc2, draw_x, draw_y, bitmap_width, bitmap_height);
				XSetClipMask(d, gc2, None);
			}
		} else {
			// Black piece: draw silhouette as black using its own (filled) mask.
			Pixmap mask = *(imageMap.at(piece));
			XSetClipMask(d, gc2, mask);
			XSetClipOrigin(d, gc2, draw_x, draw_y);
			XSetForeground(d, gc2, colours[Black]);
			XFillRectangle(d, w, gc2, draw_x, draw_y, bitmap_width, bitmap_height);
			XSetClipMask(d, gc2, None);
		}

		XSync(d, false);
	} catch (const out_of_range& except) {
		cerr << "out of range error: " << except.what() << endl;
	}
}

