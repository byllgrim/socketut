BIN = server client
SRC = ${BIN:=.c}

all: ${BIN}

.c:
	@${CC} -o $@ $<

clean:
	rm -f ${BIN}
