CC = gcc
CFLAGS = -Wall -Werror
CLIENT = LiteSH
SERVER = LiteSHserver
SERVICE = LiteSH.service

PREFIX = /usr

all: bin/$(CLIENT) bin/$(SERVER) build/libhelp.so

bin/$(CLIENT): build/client.o build/erproc.o build/commands.o
	$(CC) $(CFLAGS) $^ -o $@ -ldl

bin/$(SERVER): build/server.o build/erproc.o build/commands.o
	$(CC) $(CFLAGS) $^ -o $@
	
build/client.o: src/client.c
	$(CC) $(CFLAGS) -c $< -o $@

build/server.o: src/server.c
	$(CC) $(CFLAGS) -c $< -o $@
	
build/erproc.o: src/libs/erproc.c
	$(CC) $(CFLAGS) -c $< -o $@
	
build/commands.o: src/libs/commands.c
	$(CC) $(CFLAGS) -c $< -o $@

build/help.o: src/libs/help.c
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
	
build/libhelp.so: build/help.o
	$(CC) $(CFLAGS) -g -shared -o $@ $<
	
.PHONY: clean install unistall

clean: 
	rm -rf build/*.o
	rm -rf build/*.so
	rm -rf bin/*
	
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f LiteSH $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/LiteSH
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp -f libhelp.so $(DESTDIR)$(PREFIX)/lib
	chmod 644 $(DESTDIR)$(PREFIX)/lib/libhelp.so
	
unistall:
	rm -f $(DESTDIR)$(PREFIX)/bin/LiteSH
	rm -f $(DESTDIR)$(PREFIX)/lib/libhelp.so
	
