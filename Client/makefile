CFLAGS= -Wall -pedantic -std=gnu99


all: p

c : client.c
	gcc $(CFLAGS) `pkg-config --cflags glib-2.0 gtk+-2.0` client.c -o c `pkg-config --cflags --libs libnotify`

p : pro_server.o server_functions.o
	gcc $(CFLAGS) `pkg-config --cflags glib-2.0 gtk+-2.0`   pro_server.o server_functions.o -o build/p `pkg-config --cflags --libs libnotify`

pro_server.o : pro_server.c
	gcc $(CFLAGS) `pkg-config --cflags glib-2.0 gtk+-2.0` pro_server.c -o pro_server.o -c `pkg-config --cflags --libs libnotify`

server_functions.o : server_functions.c
	gcc  server_functions.c -o server_functions.o -c

clean:
	rm c
