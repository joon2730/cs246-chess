#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <cmath>
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
	char color_vals[11][11] = {"white",  "black", "red",      "green",
		"blue",   "cyan",  "yellow",   "magenta",
		"orange", "brown", "dimgray"};

	cmap=DefaultColormap(d,DefaultScreen(d));
	for(int i=0; i < 11; ++i) {
		XParseColor(d,cmap,color_vals[i],&xcolour);
		XAllocColor(d,cmap,&xcolour);
		colours[i]=xcolour.pixel;
	}

	XSetForeground(d,gc,colours[Black]);

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
	XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
	XSetForeground(d, gc, colours[colour]);
	XFillRectangle(d, w, gc, x, y, width, height);
	XSetForeground(d, gc, colours[Black]);
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

void Xwindow::drawPiece(char piece, int x, int y, int width, int height) { 
	x = x + (width / 2) - (bitmap_width / 2);
	y = y + (height / 2) - (bitmap_height / 2);
	try {
		XCopyPlane(d, *(imageMap.at(piece)), w, gc2, 0, 0, bitmap_width,
		   bitmap_height, x, y, 1);
		XSync(d, false);
	} catch (const out_of_range& except) {
		cerr << "out of range error: " << except.what() << endl;
	}
}

