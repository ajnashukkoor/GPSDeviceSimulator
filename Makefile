all:
	rm -f simulator
	gcc simulator.c -o simulator
	gcc client.c -o client -lpaho-mqtt3c
