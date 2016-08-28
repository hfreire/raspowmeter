# A Raspberry Pi :computer: power meter :zap::electric_plug: solution for your home :house_with_garden:

## Features
* Polls GPIO state through sysfs interface that uses interrupts (edge detection) from the kernel
* Multi-threaded (consumer & producer) makes sure that we don't miss GPIO states

### How to build (cross-compile)
`mkdir -p build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/platforms/linux/toolchain/arm-linux-gnueabihf.cmake .. && make`

### How to start
`RASPOWMETER_GPIO_PIN=7 RASPOWMETER_GPIO_TRIGGER_MODE=falling ./raspowmeter `