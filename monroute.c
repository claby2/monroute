#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xinerama.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

static Display *dpy;
static int mon = 0;
static int dev = 50;
static int rate = 60;

void die(const char *errstr, ...) {
  va_list ap;

  va_start(ap, errstr);
  vfprintf(stderr, errstr, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

void xinit(void) {
  if (!(dpy = XOpenDisplay(NULL)))
    die("Can't open display\n");
}

void xclose(void) {
  XCloseDisplay(dpy);
  exit(EXIT_SUCCESS);
}

XineramaScreenInfo *getscrinfos(int *nn) {
  if (XineramaIsActive(dpy)) {
    return XineramaQueryScreens(dpy, nn);
  }
  die("Xinerma is not active\n");
  return NULL;
}

void listmon(void) {
  int nn;
  XineramaScreenInfo *info = getscrinfos(&nn);
  for (int i = 0; i < nn; i++)
    printf("\tMonitor %d: %dX%d+%d+%d\n", info[i].screen_number, info[i].width,
           info[i].height, info[i].x_org, info[i].y_org);
}

void usage(char *argv0) {
  die("usage: %s [-l] [-m monitor] [-d device] [-r rate]\n", argv0);
}

void v4l2(void) {
  char devpath[256], cmd[256];
  struct stat stats;
  snprintf(devpath, sizeof(devpath), "/dev/video%d", dev);
  if (stat(devpath, &stats) == -1) {
    system("sudo modprobe -r v4l2loopback");
    snprintf(
        cmd, sizeof(cmd),
        "sudo modprobe v4l2loopback video_nr=%d card_label=\"monroutecam\"",
        dev);
    system(cmd);
  }
}

void route(void) {
  int nn;
  char cmd[256];
  XineramaScreenInfo scr = getscrinfos(&nn)[mon];
  snprintf(cmd, sizeof(cmd),
           "ffmpeg -f x11grab -r %d -s %dx%d -i :0+%d,%d -pix_fmt yuv420p -f "
           "v4l2 /dev/video%d",
           rate, scr.width, scr.height, scr.x_org, scr.y_org, dev);
  system(cmd);
}

int main(int argc, char *argv[]) {
  int opt;
  xinit();

  while ((opt = getopt(argc, argv, "m:d:r:l")) != -1) {
    switch (opt) {
    case 'm':
      mon = (int)atol(optarg);
      break;
    case 'd':
      dev = (int)atol(optarg);
      break;
    case 'r':
      rate = (int)atol(optarg);
      break;
    case 'l':
      listmon();
      exit(EXIT_SUCCESS);
    default:
      usage(argv[0]);
    }
  }
  v4l2();
  route();
  xclose();

  return 0;
}
