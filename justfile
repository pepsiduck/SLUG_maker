run :
    clear
    make -C build -j16
    mv build/SLUG_maker SLUG_maker
setup:
    mkdir -p build/
    cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
clear:
    rm -rf build
    rm SLUG_maker
