mkdir -p build && cd build && cmake ..


cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/platforms/linux/toolchain/arm-linux-gnueabihf.cmake ..