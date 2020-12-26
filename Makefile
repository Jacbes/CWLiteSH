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
	
install: bin/$(CLIENT) bin/$(SERVER)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f bin/$(CLIENT) $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/LiteSH
	cp -f bin/$(SERVER) $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/LiteSHserver
	
	
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	cp -f build/libhelp.so $(DESTDIR)$(PREFIX)/lib
	chmod 644 $(DESTDIR)$(PREFIX)/lib/libhelp.so
	
	mkdir -p /etc/systemd/system
	cp -f $(SERVICE) /etc/systemd/system
	chmod 755 $(DESTDIR)$(PREFIX)/lib/LiteSH.service
	
unistall:
	rm -f $(DESTDIR)$(PREFIX)/bin/LiteSH
	rm -f $(DESTDIR)$(PREFIX)/bin/LiteSHserver
	rm -f $(DESTDIR)$(PREFIX)/lib/libhelp.so
	rm -f /etc/systemd/system/LiteSH.service
	
