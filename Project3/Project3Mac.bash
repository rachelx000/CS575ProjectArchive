for n in 2 3 4 5 10 15 20 30 40 50
do
    echo "NUMCAPITALS = $n"
    cmake -S . -B build -DNUMCAPITALS=$n
    cmake --build build
    ./build/Project3
    rm -rf build
done