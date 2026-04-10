# Makefile for Cosmic Dance - Multi Scene Animation
# Compile all chapters plus new accurate solar system

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
LDFLAGS = -lopengl32 -lglu32 -lfreeglut

# Output executables
TARGET1 = chapter1_chaos.exe
TARGET2 = chapter2_bigbang.exe
TARGET3 = chapter3_accurate_solar.exe
TARGET4 = nebula_genesis.exe
TARGET5 = galaxy_milkyway.exe
TARGET6 = cosmic_observer.exe
TARGET8 = emergence_of_life.exe

# Source files
SOURCES1 = chaos_darkness.cpp
SOURCES2 = chapter2_bigbang.cpp
SOURCES3 = chapter3_accurate_solar.cpp
SOURCES4 = nebula_genesis.cpp
SOURCES5 = galaxy_milkyway.cpp
SOURCES6 = cosmic_observer.cpp
SOURCES8 = emergence_of_life.cpp

OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)
OBJECTS3 = $(SOURCES3:.cpp=.o)
OBJECTS4 = $(SOURCES4:.cpp=.o)
OBJECTS5 = $(SOURCES5:.cpp=.o)
OBJECTS6 = $(SOURCES6:.cpp=.o)
OBJECTS8 = $(SOURCES8:.cpp=.o)

# Default target - build all
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET8)

# Linking chapters
$(TARGET1): $(OBJECTS1)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET2): $(OBJECTS2)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET3): $(OBJECTS3)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET4): $(OBJECTS4)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET5): $(OBJECTS5)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET6): $(OBJECTS6)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET8): $(OBJECTS8)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Chapter 1
run1: $(TARGET1)
	./$(TARGET1)

# Run Chapter 2
run2: $(TARGET2)
	./$(TARGET2)

# Run Accurate Solar System
run3: $(TARGET3)
	./$(TARGET3)

# Run Nebula Genesis
run4: $(TARGET4)
	./$(TARGET4)

# Run Milky Way Galaxy
run5: $(TARGET5)
	./$(TARGET5)

# Run Cosmic Observer
run6: $(TARGET6)
	./$(TARGET6)

# Run Emergence of Life
run8: $(TARGET8)
	./$(TARGET8)

# Clean build files
clean:
	del $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS8) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET8) 2>nul

# Phony targets
.PHONY: all run1 run2 run3 run4 run5 run6 run8 clean
