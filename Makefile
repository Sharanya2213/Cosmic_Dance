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
TARGET7 = emergence_of_life.exe
TARGET8 = first_cells.exe
TARGET9 = multicellular_organism.exe
TARGET10 = chapter10_early_organism.exe
TARGET11 = chapter11_complex_organism.exe
TARGET12 = chapter12_ocean_to_land.exe
TARGET13 = chapter13_land_volcanos.exe
TARGET14 = chapter14_dinosaur_era.exe
TARGET15 = chapter15_mammalian_rise.exe
TARGET17 = chapter17_humanity_observes.exe

# Source files
SOURCES1 = chaos_darkness.cpp
SOURCES2 = chapter2_bigbang.cpp
SOURCES3 = chapter3_accurate_solar.cpp
SOURCES4 = nebula_genesis.cpp
SOURCES5 = galaxy_milkyway.cpp
SOURCES6 = cosmic_observer.cpp
SOURCES7 = emergence_of_life.cpp
SOURCES8 = first_cells.cpp
SOURCES9 = multicellular_organism.cpp
SOURCES10 = chapter10_early_organism.cpp
SOURCES11 = chapter11_complex_organism.cpp
SOURCES12 = chapter12_ocean_to_land.cpp
SOURCES13 = chapter13_land_volcanos.cpp
SOURCES14 = chapter14_dinosaur_era.cpp
SOURCES15 = chapter15_mammalian_rise.cpp
SOURCES17 = chapter17_humanity_observes.cpp

OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)
OBJECTS3 = $(SOURCES3:.cpp=.o)
OBJECTS4 = $(SOURCES4:.cpp=.o)
OBJECTS5 = $(SOURCES5:.cpp=.o)
OBJECTS6 = $(SOURCES6:.cpp=.o)
OBJECTS7 = $(SOURCES7:.cpp=.o)
OBJECTS8 = $(SOURCES8:.cpp=.o)
OBJECTS9 = $(SOURCES9:.cpp=.o)
OBJECTS10 = $(SOURCES10:.cpp=.o)
OBJECTS11 = $(SOURCES11:.cpp=.o)
OBJECTS12 = $(SOURCES12:.cpp=.o)
OBJECTS13 = $(SOURCES13:.cpp=.o)
OBJECTS14 = $(SOURCES14:.cpp=.o)
OBJECTS15 = $(SOURCES15:.cpp=.o)
OBJECTS17 = $(SOURCES17:.cpp=.o)

# Default target - build all
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET17)

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

$(TARGET10): $(OBJECTS10)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET11): $(OBJECTS11)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET12): $(OBJECTS12)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET13): $(OBJECTS13)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET14): $(OBJECTS14)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET15): $(OBJECTS15)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET17): $(OBJECTS17)
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

# Run Chapter 13: Volcanic Island
run13: $(TARGET13)
	./$(TARGET13)

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
run7: $(TARGET7)
	./$(TARGET7)

# Run First Cells
run8: $(TARGET8)
	./$(TARGET8)

# Run Multicellular Organism
run9: $(TARGET9)
	./$(TARGET9)

# Run Early Organism Movement
run10: $(TARGET10)
	./$(TARGET10)

# Run Early Complex Organism Formation
run11: $(TARGET11)
	./$(TARGET11)

# Run Life Moves from Ocean to Land
run12: $(TARGET12)
	./$(TARGET12)

# Run Land with Volcanoes & Gymnosperms
run13: $(TARGET13)
	./$(TARGET13)

# Run Age of Dinosaurs
run14: $(TARGET14)
	./$(TARGET14)

# Run Mammalian Rise
run15: $(TARGET15)
	./$(TARGET15)

# Run Humanity Observes the Beginning
run17: $(TARGET17)
	./$(TARGET17)

# Clean build files
clean:
	del $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) $(OBJECTS8) $(OBJECTS9) $(OBJECTS10) $(OBJECTS11) $(OBJECTS12) $(OBJECTS13) $(OBJECTS14) $(OBJECTS15) $(OBJECTS17) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET17) 2>nul
# Phony targets
.PHONY: all run1 run2 run3 run4 run5 run6 run7 run8 run9 run10 run11 run12 run13 run14 run15 run17 clean
