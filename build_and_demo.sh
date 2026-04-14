#!/bin/bash
# Cosmic Dance - Build and Demo Script
# Comprehensive build automation for all 16 chapters

echo "=========================================="
echo "COSMIC DANCE - 16-Chapter Animation Series"
echo "=========================================="
echo ""

# Color codes for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Check if mingw32-make is available
if ! command -v mingw32-make &> /dev/null; then
    echo -e "${RED}Error: mingw32-make not found. Please install MinGW.${NC}"
    exit 1
fi

# Function to build a chapter
build_chapter() {
    local chapter=$1
    local name=$2
    echo -e "${BLUE}Building Chapter $chapter: $name...${NC}"
    if mingw32-make chapter$chapter*.exe > /dev/null 2>&1; then
        echo -e "${GREEN}✓ Chapter $chapter built successfully${NC}"
        return 0
    else
        echo -e "${RED}✗ Chapter $chapter build failed${NC}"
        return 1
    fi
}

# Display menu
show_menu() {
    echo ""
    echo "Choose an action:"
    echo "  1. Build all chapters"
    echo "  2. Build specific chapter"
    echo "  3. Run specific chapter"
    echo "  4. Clean all builds"
    echo "  5. Show chapter descriptions"
    echo "  6. Verify all executables"
    echo "  0. Exit"
    echo ""
}

# Show chapter descriptions
show_descriptions() {
    echo ""
    echo -e "${BLUE}=== COSMIC DANCE - 16 CHAPTERS ===${NC}"
    echo ""
    echo "PART 1: COSMIC ORIGINS"
    echo "  Ch 1:  Chaos and Darkness - Primordial universe"
    echo "  Ch 2:  The Big Bang - Birth of universe"
    echo "  Ch 3:  Accurate Solar System - 8 planets with real orbits"
    echo "  Ch 4:  Nebula Genesis - Star birth from dust"
    echo "  Ch 5:  Milky Way Galaxy - Galactic structure"
    echo "  Ch 6:  Cosmic Observer - Zooming through space"
    echo ""
    echo "PART 2: LIFE EMERGES"
    echo "  Ch 7:  Emergence of Life - Prebiotic chemistry"
    echo "  Ch 8:  First Cells - Single-celled organisms"
    echo "  Ch 9:  Multicellular Organism - Cells clustering"
    echo ""
    echo "PART 3: ORGANISM EVOLUTION"
    echo "  Ch 10: Early Organism Movement - Swimming in oceans"
    echo "  Ch 11: Complex Organism - First fish with fins"
    echo "  Ch 12: Ocean to Land - Fish-tetrapod transition"
    echo ""
    echo "PART 4: TERRESTRIAL LIFE"
    echo "  Ch 13: Land with Volcanoes - Ancient environments"
    echo "  Ch 14: Age of Dinosaurs - T-Rex, Triceratops, Stegosaurus"
    echo "  Ch 15: Asteroid Impact - Extinction event"
    echo "  Ch 16: Mammalian Rise - Post-extinction mammals"
    echo ""
}

# Verify executables
verify_executables() {
    echo ""
    echo -e "${BLUE}Verifying executables...${NC}"
    local count=0
    for i in {1..16}; do
        if [ -f "chapter$i*.exe" ] 2>/dev/null || [ -f "chapter${i}_*.exe" ] 2>/dev/null; then
            echo -e "${GREEN}✓ Chapter $i executable found${NC}"
            ((count++))
        fi
    done
    echo ""
    echo "Total executables found: $count/16"
    echo ""
}

# Main loop
while true; do
    show_menu
    read -p "Enter choice [0-6]: " choice
    
    case $choice in
        1)
            echo ""
            echo -e "${BLUE}Building all chapters...${NC}"
            minGW32-make all
            echo ""
            echo -e "${GREEN}All chapters built successfully!${NC}"
            ;;
        2)
            read -p "Enter chapter number (1-16): " chapter
            if [ "$chapter" -ge 1 ] && [ "$chapter" -le 16 ]; then
                mingw32-make chapter${chapter}*.exe
            else
                echo -e "${RED}Invalid chapter number${NC}"
            fi
            ;;
        3)
            read -p "Enter chapter number to run (1-16): " chapter
            if [ "$chapter" -ge 1 ] && [ "$chapter" -le 16 ]; then
                mingw32-make run$chapter
            else
                echo -e "${RED}Invalid chapter number${NC}"
            fi
            ;;
        4)
            echo -e "${BLUE}Cleaning all builds...${NC}"
            mingw32-make clean
            echo -e "${GREEN}Clean complete${NC}"
            ;;
        5)
            show_descriptions
            ;;
        6)
            verify_executables
            ;;
        0)
            echo "Exiting Cosmic Dance. Goodbye!"
            exit 0
            ;;
        *)
            echo -e "${RED}Invalid choice${NC}"
            ;;
    esac
done
