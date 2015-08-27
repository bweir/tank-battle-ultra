all:
	cd src; g++ -I ../include display.cpp audio.cpp level.cpp object.cpp bullet.cpp input.cpp main.cpp `allegro-config --libs` -L ../lib -o ../tankbattle

clean:
	rm tankbattle
