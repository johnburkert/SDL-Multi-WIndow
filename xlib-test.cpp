#include <iostream>
#include <X11/Xlib.h>

// https://tronche.com/gui/x/xlib/
// https://gist.github.com/kui/2622504

void print_window_attributes(Display* display, Window window)
{
  XWindowAttributes attributes;

  XGetWindowAttributes(display, window, &attributes);

  std::cout << "Window[" << window << "] ( " << attributes.width << ", " << attributes.height << ")" << std::endl;
}

Bool xerror = False;

int handle_error(Display* display, XErrorEvent* error)
{
  printf("ERROR: X11 error\n");
  xerror = True;
  return 1;
}

int main(int argc, char* argv[])
{
  Display* display = XOpenDisplay(NULL);

  XSetErrorHandler(handle_error);

  int count = XScreenCount(display);

  std::cout << "Screen Count: " << count << std::endl;

  for (int i = 0; i < count; i++)
  {
    Screen* screen = XScreenOfDisplay(display, i);
    int width = XWidthOfScreen(screen);
    int height = XHeightOfScreen(screen);

    std::cout << "Screen[" << i << "] " << screen << " (" << width << ", " << height << ")" << std::endl;

    Window window = XRootWindowOfScreen(screen);
    Window parent = window;
    Window root = None;
    Window *children;
    unsigned int children_count;

    print_window_attributes(display, parent);

    while (parent != root)
    {
      Window win = parent;
      Status status = XQueryTree(display, win, &root, &parent, &children, &children_count);
      print_window_attributes(display, parent);

      if (status)
        XFree(children);

        if(xerror)
        {
          printf("fail\n");
          exit(1);
        }
    }
  }

  XCloseDisplay(display);

  return 0;
}
