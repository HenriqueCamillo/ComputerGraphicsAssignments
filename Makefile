all:
	g++ *.cpp -lglfw -lGL -lGLEW -lm -o main

debug:
	g++ *.cpp -lglfw -lGL -lGLEW -lm -g -o main

run:
	./main

clean:
	rm ./main

zip: clean
	zip Ex.zip *cpp *.hpp *.vert *.frag Makefile