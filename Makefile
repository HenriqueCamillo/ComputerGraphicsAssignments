all:
	g++ *.cpp -lglfw -lGL -lGLEW -lm -lSDL2_image -lSDL2 -o main

debug:
	g++ *.cpp -lglfw -lGL -lGLEW -lm -lSDL2_image -lSDL2 -g -o main

run:
	./main

clean:
	rm ./main

zip: 
	zip Ex.zip *cpp *.hpp Makefile *.md shaders objects skybox -r 