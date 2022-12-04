#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

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
  char color_vals[11][11]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "aliceblue", "lightblue"};

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

  XSynchronize(d,True);

  usleep(1000);

  // Make sure we don't race against the Window being shown
  XEvent ev;
  while(1) {
    XNextEvent(d, &ev);
    if(ev.type == Expose) break;
  }

  /* allocate a new GC (graphics context) for drawing in the window. */
  gc2 = create_gc(d, w, 0);
  XSync(d, False);

  char piece_names[12] = {'K','Q','R','B','N','P','k','q','r','b','n','p'};
  string directory = "../img/";
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

void Xwindow::drawString(int x, int y, string msg) {
  XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
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
