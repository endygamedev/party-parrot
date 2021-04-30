TARGET = parrot
PREFIX = /usr/local/bin
ASSETS = /usr/local/lib

.PHONY: all clean install uninstall update

all:
	echo $(TARGET)

clean:
	sudo rm -rf $(TARGET) *.o

parrot.o:
	parrot.c gcc -c -o parrot.o parrot.c

$(TARGET):
	parrot.o gcc -o parrot parrot.o

install:
	install $(TARGET) $(PREFIX)
	mkdir $(ASSETS)/parrot
	cp -r ./frames $(ASSETS)/parrot/

uninstall:
	sudo rm -rf $(PREFIX)/$(TARGET)
	sudo rm -rf $(ASSETS)/parrot

update:
	sudo make uninstall
	git pull
	sudo make install
	sudo chmod -R ug+w .;
