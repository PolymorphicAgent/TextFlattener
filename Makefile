sources := main.cpp Utils.cpp File.cpp CompressionTable.cpp

main: $(sources)
	g++ -o main $(sources) -std=c++20