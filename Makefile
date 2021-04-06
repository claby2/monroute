include config.mk

SRC = monroute.c
OBJ = ${SRC:.c=.o}

all: monroute

.c.o:
	${CC} -c ${CFLAGS} $<

monroute: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f monroute ${OBJ}

install: monroute
	mkdir -p ${PREFIX}/bin
	cp -f monroute ${PREFIX}/bin
	chmod 755 ${PREFIX}/bin/monroute
	mkdir -p ${MANPREIFX}/man1
	cp -f monroute.1 ${MANPREFIX}/man1/monroute.1
	chmod 644 ${MANPREFIX}/man1/monroute.1

uninstall:
	rm -f ${PREFIX}/bin/monroute \
		${MANPREFIX}/man1/monroute.1

.PHONY: all monroute clean install uninstall
