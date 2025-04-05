for t in 1 4
do
    echo "NUMT = $t"
    cmake -S . -B build -DNUMT=$t
    cmake --build build
    ./build/Project0
    rm -rf build
done