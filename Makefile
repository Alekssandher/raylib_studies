RAYLIB_PATH = /home/alek/raylib/src

build:
	gcc $(FILE) -o $(basename $(FILE)) -I$(RAYLIB_PATH) \
	    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11