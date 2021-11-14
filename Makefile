all:
	gcc -c alsa_play.c -o alsa_play.o -fno-stack-protector
	ld alsa_play.o -o alsa_play
clean:
	rm -f alsa_play.o alsa_play
