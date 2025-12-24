#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open X display\n");
        exit(1);
    }

    int screen = DefaultScreen(dpy);

    Window win = XCreateSimpleWindow(
        dpy,
        RootWindow(dpy, screen),
        200, 200,
        500, 300,
        1,
        BlackPixel(dpy, screen),
        WhitePixel(dpy, screen)
    );

    XStoreName(dpy, win, "X11 Event Logger (ASCII)");

    XSelectInput(dpy, win,
        KeyPressMask |
        KeyReleaseMask |
        ButtonPressMask |
        ButtonReleaseMask |
        PointerMotionMask
    );

    XMapWindow(dpy, win);
    XFlush(dpy);

    printf("Click inside the window and type keys\n\n");

    XEvent ev;

    while (1) {
        XNextEvent(dpy, &ev);

        switch (ev.type) {

        case KeyPress: {
            char buf[32] = {0};
            KeySym keysym;
            int len;

            len = XLookupString(
                &ev.xkey,
                buf,
                sizeof(buf),
                &keysym,
                NULL
            );

            printf("[KEY PRESS] keycode=%u keysym=0x%lx",
                   ev.xkey.keycode, keysym);

            if (len > 0) {
                printf(" ascii='%c' (0x%02x)",
                       buf[0], (unsigned char)buf[0]);
            } else {
                printf(" ascii=<non-printable>");
            }

            printf("\n");
            break;
        }

        case KeyRelease:
            printf("[KEY RELEASE] keycode=%u\n",
                   ev.xkey.keycode);
            break;

        case ButtonPress:
            printf("[MOUSE PRESS] button=%u x=%d y=%d\n",
                   ev.xbutton.button,
                   ev.xbutton.x, ev.xbutton.y);
            break;

        case ButtonRelease:
            printf("[MOUSE RELEASE] button=%u x=%d y=%d\n",
                   ev.xbutton.button,
                   ev.xbutton.x, ev.xbutton.y);
            break;

        case MotionNotify:
            printf("[MOUSE MOVE] x=%d y=%d\n",
                   ev.xmotion.x, ev.xmotion.y);
            break;
        }

        fflush(stdout);
    }
}

