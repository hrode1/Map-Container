all: minimal.cpp morseex.cpp test.cpp test-kec.cpp test-scaling.cpp;
	g++ -o minimal minimal.cpp;
	g++ -o morseex morseex.cpp;
	g++ -o test test.cpp;
	g++ -o test-kec test-kec.cpp;
	g++ -o test-scaling test-scaling.cpp;
clean:
	rm minimal;
	rm morseex;
	rm test;
	rm test-scaling;
	rm test-kec;
