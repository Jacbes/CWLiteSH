CC = gcc
CFLAGS = -Wall -Werror
CLIENT = LiteSH
SERVER = LiteSHserver
SERVICE = LiteSH.service

all: bin/$(CLIENT) bin/$(SERVER)

bin/$(CLIENT): build/client.o build/erproc.o build/commands.o
	$(CC) $(CFLAGS) $^ -o $@

bin/$(SERVER): build/server.o build/erproc.o build/commands.o
	$(CC) $(CFLAGS) $^ -o $@
	
build/client.o: src/client.c
	$(CC) $(CFLAGS) -c $^ -o $@

build/server.o: src/server.c
	$(CC) $(CFLAGS) -c $^ -o $@
	
build/erproc.o: src/libs/erproc.c
	$(CC) $(CFLAGS) -c $< -o $@
	
build/commands.o: src/libs/commands.c
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY: clean install unistall

clean: 
	rm -rf build/*.o
	
install:
	sudo cp bin/$(CLIENT) /usr/bin/$(CLIENT)
	sudo cp bin/$(SERVER) /usr/bin/$(SERVER)
	sudo cp $(SERVICE) /etc/systemd/system/$(SERVICE)
	
unistall:
	sudo rm /usr/bin/$(CLIENT) /usr/bin/$(SERVER) /etc/systemd/system/$(SERVICE)