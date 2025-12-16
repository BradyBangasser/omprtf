git submodule update --init
rm -rf build 2>/dev/null 1>&2
mkdir build && cd build && cmake .. && make
