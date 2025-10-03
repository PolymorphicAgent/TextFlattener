# Makefile for the project

# # Define Sources
# sources := main.cpp Utils.cpp File.cpp CompressionTable.cpp Ctxt/ctxt.cpp

# # Define Headers
# headers := Utils.h File.h CompressionTable.h Ctxt/ctxt.h

# # Build Target
# main.exe: $(sources) $(headers)
# 	g++ -g -o main $(sources) -std=c++20

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
SRCS := main.cpp Utils.cpp File.cpp CompressionTable.cpp Ctxt/ctxt.cpp
HEADERS := Utils.h File.h CompressionTable.h Ctxt/ctxt.h

# Directory to store object files; keeps build artifacts separate from sources
OBJDIR := build

# Object files live in $(OBJDIR) with the same base names as sources
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
TARGET := main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile .cpp -> .o into the build directory
$(OBJDIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)