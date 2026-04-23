# Makefile for Cosmic Dance - Multi Scene Animation
# Compile all chapters plus new accurate solar system

CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
LDFLAGS = -lopengl32 -lglu32 -lfreeglut

# Output executables
TARGET1 = chapter1_chaos.exe
TARGET2 = chapter2_bigbang.exe
TARGET3 = nebula_genesis.exe
TARGET4 = cosmic_observer.exe
TARGET5 = galaxy_milkyway.exe
TARGET6 = chapter3_accurate_solar.exe
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
INTEGRATED = cosmic_dance_integrated.exe
INTEGRATED_COMPLETE = cosmic_dance_complete.exe

# Source files
SOURCES1 = chaos_darkness.cpp
SOURCES2 = chapter2_bigbang.cpp
SOURCES3 = nebula_genesis.cpp
SOURCES4 = cosmic_observer.cpp
SOURCES5 = galaxy_milkyway.cpp
SOURCES6 = chapter3_accurate_solar.cpp
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
SOURCES_INTEGRATED = integrated_cosmic_dance.cpp
SOURCES_INTEGRATED_COMPLETE = cosmic_dance_complete.cpp

OBJECTS17 = $(SOURCES17:.cpp=.o)
OBJECTS_INTEGRATED = $(SOURCES_INTEGRATED:.cpp=.o)
OBJECTS_INTEGRATED_COMPLETE = $(SOURCES_INTEGRATED_COMPLETE:.cpp=.o)
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

# Default target - build all
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET17) $(INTEGRATED) $(INTEGRATED_COMPLETE)

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

$(INTEGRATED): $(OBJECTS_INTEGRATED)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(INTEGRATED_COMPLETE): $(OBJECTS_INTEGRATED_COMPLETE)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Chapter 1
run1: $(TARGET1)
	./$(TARGET1)

# Run Chapter 2: The Big Bang — Birth of the Universe
run2: $(TARGET2)
	./$(TARGET2)

# Run Chapter 3: Stellar Nurseries — Nebula Genesis
run3: $(TARGET3)
	./$(TARGET3)

# Run Chapter 4: The Cosmic Observer
run4: $(TARGET4)
	./$(TARGET4)

# Run Chapter 5: The Milky Way Galaxy
run5: $(TARGET5)
	./$(TARGET5)

# Run Chapter 6: Formation of the Solar System
run6: $(TARGET6)
	./$(TARGET6)

# Run Chapter 7: Precambrian Era — Emergence of Life
run7: $(TARGET7)
	./$(TARGET7)

# Run Chapter 8: Precambrian — The First Cells
run8: $(TARGET8)
	./$(TARGET8)

# Run Chapter 9: Precambrian — Multicellular Life
run9: $(TARGET9)
	./$(TARGET9)

# Run Chapter 10: Cambrian Period — Explosion of Life
run10: $(TARGET10)
	./$(TARGET10)

# Run Chapter 11: Ordovician–Silurian — Complex Marine Life
run11: $(TARGET11)
	./$(TARGET11)

# Run Chapter 12: Devonian Period — Life Moves to Land
run12: $(TARGET12)
	./$(TARGET12)

# Run Chapter 13: Carboniferous Period — Forests and Amphibians
run13: $(TARGET13)
	./$(TARGET13)

# Run Chapter 14: Jurassic Period — Age of Dinosaurs
run14: $(TARGET14)
	./$(TARGET14)

# Run Chapter 15: Cretaceous Period — Rise of Mammals
run15: $(TARGET15)
	./$(TARGET15)

# Run Chapter 17: Humanity Observes the Universe
run17: $(TARGET17)
	./$(TARGET17)

# Run Integrated Cosmic Dance (5:30 minutes)
run_integrated: $(INTEGRATED)
	./$(INTEGRATED)

# Run Complete Cosmic Dance (16 chapters, 5:20 minutes)
run_complete: $(INTEGRATED_COMPLETE)
	./$(INTEGRATED_COMPLETE)

# Clean build files
clean:
	del $(OBJECTS1) $(OBJECTS2) $(OBJECTS3) $(OBJECTS4) $(OBJECTS5) $(OBJECTS6) $(OBJECTS7) $(OBJECTS8) $(OBJECTS9) $(OBJECTS10) $(OBJECTS11) $(OBJECTS12) $(OBJECTS13) $(OBJECTS14) $(OBJECTS15) $(OBJECTS17) $(OBJECTS_INTEGRATED) $(OBJECTS_INTEGRATED_COMPLETE) $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10) $(TARGET11) $(TARGET12) $(TARGET13) $(TARGET14) $(TARGET15) $(TARGET17) $(INTEGRATED) $(INTEGRATED_COMPLETE) 2>nul
# Phony targets
.PHONY: all run1 run2 run3 run4 run5 run6 run7 run8 run9 run10 run11 run12 run13 run14 run15 run17 run_integrated run_complete clean
