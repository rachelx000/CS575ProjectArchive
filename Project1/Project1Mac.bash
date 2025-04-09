for t in 1
do
    echo "NUMT = $t"
    cmake -S . -B build -DNUMT=$t
    cmake --build build
    ./build/Project1
    rm -rf build
done