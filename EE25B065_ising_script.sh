#!/bin/bash

# Simple Ising Model Plot Script - All J values in one graph
# Usage: ./simple_plot.sh

EXECUTABLE="./ising"
OUTPUT_DIR="results"

# Parameters
L=32
T_START=0.5
T_END=4.0
T_INCREMENT=0.01
N=5000
DM=0.0001

# J values to test
J_VALUES=(0.25 0.5 1.0 1.25 1.5)

echo "Running Ising simulations for different J values..."

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Please compile your code first: gcc -o ising ising.c -lm"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Run simulations for each J value
for J in "${J_VALUES[@]}"; do
    echo "Running J = $J..."
    $EXECUTABLE $L $J $T_START $T_END $T_INCREMENT $N $DM > /dev/null 2>&1
    
    if [ -f "Mavg_vs_T.txt" ]; then
        mv "Mavg_vs_T.txt" "${OUTPUT_DIR}/magnetization_J${J}.txt"
        echo "  Done"
    else
        echo "  Failed"
    fi
done

echo "Creating plot..."

# Create simple gnuplot script
cat > "plot.gp" << 'EOF'
set terminal png size 1000,600 font "Arial,12"
set output "magnetization_comparison.png"

set title "2D Ising Model: Magnetization vs Temperature"
set xlabel "Temperature"
set ylabel "|Magnetization|"

set grid
set key top right

plot "results/magnetization_J0.25.txt" using 1:(abs($2)) with linespoints lw 2 title "J = 0.25", \
     "results/magnetization_J0.5.txt" using 1:(abs($2)) with linespoints lw 2 title "J = 0.5", \
     "results/magnetization_J1.0.txt" using 1:(abs($2)) with linespoints lw 2 title "J = 1.0", \
     "results/magnetization_J1.25.txt" using 1:(abs($2)) with linespoints lw 2 title "J = 1.25", \
     "results/magnetization_J1.5.txt" using 1:(abs($2)) with linespoints lw 2 title "J = 1.5"
EOF

# Run gnuplot
gnuplot plot.gp

if [ -f "magnetization_comparison.png" ]; then
    echo "Plot created: magnetization_comparison.png"
else
    echo "Plot failed"
fi

# Clean up
rm plot.gp
