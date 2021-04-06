X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

XINERAMALIBS = -lXinerama
XINERAMAFLAGS = -DXINERAMA

INCS = -I{X11INC}
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS}

CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} -D_XOPEN_SOURCE=600 ${XINERAMAFLAGS}
LDFLAGS = ${LIBS}

CC = cc
