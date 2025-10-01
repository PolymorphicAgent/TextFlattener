sources := main.cpp Utils.cpp File.cpp CompressionTable.cpp
headers := Utils.h File.h CompressionTable.h

main.exe: $(sources) $(headers)
	g++ -g -o main $(sources) -std=c++20