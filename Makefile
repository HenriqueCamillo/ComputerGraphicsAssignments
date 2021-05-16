all:
	g++ *.cpp -lglfw -lGL -lGLEW -lm -o main

run:
	./main

clean:
	rm ./main

zip: clean
	zip Ex.zip *cpp *.hpp *.vert *.frag Makefile