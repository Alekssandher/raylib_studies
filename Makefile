RAYLIB_PATH = /home/alek/raylib/src

build:
	mkdir -p $(dir $(FILE))bin

	gcc $(FILE) \
	    -o $(dir $(FILE))bin/$(basename $(notdir $(FILE))) \
	    -I$(RAYLIB_PATH) \
	    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
		
clean:
	find . -type f -name "*.o" -delete
	find . -type d -name "bin" -exec rm -rf {} +