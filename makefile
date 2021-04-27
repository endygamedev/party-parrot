TARGET = parrot
PREFIX = /usr/local/bin
ASSETS = /usr/local/lib

.PHONY: all clean install uninstall

all:
	echo $(TARGET)

clean:
	rm -rf $(TARGET) *.o

parrot.o:
	parrot.c gcc -c -o parrot.o parrot.c

$(TARGET):
	parrot.o gcc -o parrot parrot.o

install:
	install $(TARGET) $(PREFIX)
	mkdir $(ASSETS)/parrot
	cp -r ./frames $(ASSETS)/parrot/

uninstall:
	rm -rf $(PREFIX)/$(TARGET)
	rm -rf $(ASSETS)/parrot
