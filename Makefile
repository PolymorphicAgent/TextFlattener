# Makefile for the project

# Define Sources
sources := main.cpp Utils.cpp File.cpp CompressionTable.cpp

# Define Headers
headers := Utils.h File.h CompressionTable.h

# Build Target
main.exe: $(sources) $(headers)
	g++ -g -o main $(sources) -std=c++20