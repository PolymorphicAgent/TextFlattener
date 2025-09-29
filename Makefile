sources := main.cpp Utils.cpp File.cpp

main: $(sources)
	g++ -o main $(sources)