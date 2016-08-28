# A Raspberry Pi :computer: power meter :zap::electric_plug: solution for your home :house_with_garden:

## Features
* Blazing fast :dizzy: C code :neckbeard::astonished: with low memory footprint :white_check_mark:
* Polls GPIO state through the sysfs interface :scream: that uses interrupts (edge detection) from the kernel :white_check_mark:
* Multi-threaded (consumer & producer) :thumbsup: makes sure that we don't miss GPIO states :white_check_mark:
* Run inside a Docker container :whale: so you don't need to manage the dependencies :raised_hands: :white_check_mark:

### How to build (cross-compile)
`mkdir -p build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/platforms/raspbian/toolchain/arm-linux-gnueabihf.cmake .. && make`

### How to start
`RASPOWMETER_GPIO_PIN=7 RASPOWMETER_GPIO_TRIGGER_MODE=falling ./raspowmeter `

### Build Docker container
`docker build -t registry.exec.sh/raspowmeter -f ./share/docker/Dockerfile .`

### Digital light pulse sensor
![alt text](https://raw.githubusercontent.com/hfreire/raspowmeter/master/share/fritzing/digital-light-pulse-sensor.png "Digital light pulse sensor in a breadboard")
