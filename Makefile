# Makefile for Cosmic Dance - Multi Scene Animation
# Compile all chapters plus new accurate solar system

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
LDFLAGS = -lopengl32 -lglu32 -lfreeglut

# Output executables
TARGET1 = chapter1_chaos.exe
TARGET2 = chapter2_bigbang.exe
TARGET3 = chapter3_solar-system.exe
TARGET4 = nebula_genesis.exe
TARGET5 = galaxy_milkyway.exe
TARGET6 = cosmic_observer.exe
TARGET7 = first_life_microscopic.exe
TARGET8 = emergence_of_life.exe
TARGET9 = first_cells.exe

# Source files
SOURCES1 = chaos_darkness.cpp
SOURCES2 = chapter2_bigbang.cpp
SOURCES3 = chapter3_solar-system.cpp
SOURCES4 = nebula_genesis.cpp
SOURCES5 = galaxy_milkyway.cpp
SOURCES6 = cosmic_observer.cpp
SOURCES7 = first_life_microscopic.cpp
SOURCES8 = emergence_of_life.cpp
SOURCES9 = first_cells.cpp

OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)
OBJECTS3 = $(SOURCES3:.cpp=.o)
OBJECTS4 = $(SOURCES4:.cpp=.o)
OBJECTS5 = $(SOURCES5:.cpp=.o)
OBJECTS6 = $(SOURCES6:.cpp=.o)
OBJECTS7 = $(SOURCES7:.cpp=.o)
OBJECTS8 = $(SOURCES8:.cpp=.o)
OBJECTS9 = $(SOURCES9:.cpp=.o)

# Default target - build all
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9)

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

$(TARGET7): $(OBJECTS7)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET8): $(OBJECTS8)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET9): $(OBJECTS9)
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

# Run First Life Microscopic
run7: $(TARGET7)
	./$(TARGET7)

# Run Emergence of Life
run8: $(TARGET8)
	./$(TARGET8)

# Run First Cells Mitosis
run9: $(TARGET9)
	./$(TARGET9)

# Clean build files
clean:
	del $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) $(OBJECTS8) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) 2>nul

# Phony targets
.PHONY: all run1 run2 run3 run4 run5 run6 run7 run8 clean
