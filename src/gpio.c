/*
 * Copyright (c) 2016, Hugo Freire <hugo@exec.sh>.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include "../include/gpio.h"

int unexport_gpio_pin(const char *gpio_pin) {
  int fd;
  char fn[32];

  if (gpio_pin == NULL || *gpio_pin == '\0') {
    return -1;
  }

  memset(fn, 0x00, 32);

  snprintf(fn, 32 - 1, SYS_GPIO_UNEXPORT_FS);

  if ((fd = open(fn, O_WRONLY)) < 0) {
    warnx("open(SYS_GPIO_UNEXPORT) failed");

    return -1;
  }

  if (write(fd, gpio_pin, strlen(gpio_pin)) < 0) {
    warnx("write(SYS_GPIO_UNEXPORT) failed");

    return -1;
  }

  if (close(fd) < 0) {
    warnx("close(SYS_GPIO_UNEXPORT) failed");

    return -1;
  }

  return 0;
}

int export_gpio_pin(const char *gpio_pin) {
  int fd;
  char fn[32];

  if (gpio_pin == NULL || *gpio_pin == '\0') {
    return -1;
  }

  memset(fn, 0x00, 32);

  snprintf(fn, 32 - 1, SYS_GPIO_EXPORT_FS);

  if ((fd = open(fn, O_WRONLY)) < 0) {
    warnx("open(SYS_GPIO_EXPORT) failed");

    return -1;
  }

  if (write(fd, gpio_pin, strlen(gpio_pin)) < 0) {
    warnx("write(SYS_GPIO_EXPORT) failed");

    return -1;
  }

  if (close(fd) < 0) {
    warnx("close(SYS_GPIO_EXPORT) failed");

    return -1;
  }

  return 0;
}

int set_gpio_direction(const char *gpio_pin) {
  int fd;
  char fn[32];

  if (gpio_pin == NULL || *gpio_pin == '\0') {
    return -1;
  }

  memset(fn, 0x00, 32);

  snprintf(fn, 32 - 1, SYS_GPIO_DIRECTION_FS, gpio_pin);

  if ((fd = open(fn, O_WRONLY)) < 0) {
    warnx("open(SYS_GPIO_DIRECTION) failed");

    return -1;
  }

  if (write(fd, "in", strlen("in")) < 0) {
    warnx("write(SYS_GPIO_DIRECTION) failed");

    return -1;
  }

  if (close(fd) < 0) {
    warnx("close(SYS_GPIO_DIRECTION) failed");

    return -1;
  }
  
  return 0;
}

int set_gpio_trigger_mode(const char *gpio_pin, const char *gpio_trigger_mode) {
  int fd;
  char fn[32];

  if (gpio_pin == NULL || *gpio_pin == '\0') {
    return -1;
  }

  if (gpio_trigger_mode == NULL || *gpio_trigger_mode == '\0') {
    return -1;
  }

  memset(fn, 0x00, 32);

  snprintf(fn, 32 - 1, SYS_GPIO_EDGE_FS, gpio_pin);

  if ((fd = open(fn, O_WRONLY)) < 0) {
    warnx("open(SYS_GPIO_EDGE) failed");
    
    return -1;
  }

  if (write(fd, gpio_trigger_mode, strlen(gpio_trigger_mode)) < 0) {
    warnx("write(SYS_GPIO_DIRECTION) failed");

    return -1;
  }

  if (close(fd) < 0) {
    warnx("close(SYS_GPIO_DIRECTION) failed");

    return -1;
  }
  
  return 0;
}

int open_gpio_value(const char *gpio_pin) {
  int fd;
  char fn[32];

  if (gpio_pin == NULL || *gpio_pin == '\0') {
    return -1;
  }
  memset(fn, 0x00, 32);

  snprintf(fn, 32 - 1, SYS_GPIO_VALUE_FS, gpio_pin);

  printf("HERE %s\n", fn);

  if ((fd = open(fn, O_RDONLY)) < 0) {
    warnx("open(SYS_GPIO_EDGE) failed");

    return -1;
  }

  return fd;
}

int setup_gpio(const char *gpio_pin, const char *gpio_trigger_mode) {
  if (export_gpio_pin(gpio_pin) < 0) { // echo ${GPIO_PIN} > /sys/class/gpio${GPIO_PIN}/export
    warnx("export_gpio_pin() failed");

    return -1;
  }

  if (set_gpio_direction(gpio_pin) < 0) { // echo in > /sys/class/gpio${$GPIO_PIN}/direction
    warnx("set_gpio_direction() failed");

    return -1;
  }

  if (set_gpio_trigger_mode(gpio_pin, gpio_trigger_mode)
      < 0) { // echo ${GPIO_TRIGGER_MODE} > /sys/class/gpio/gpio${$GPIO_PIN}/edge
    warnx("set_gpio_trigger_mode() failed");

    return -1;
  }

  return 0;
}

void read_gpio_value(int gpio_value_fd) {
  char buf;

  if (read(gpio_value_fd, &buf, 1) < 0) {
    warnx("read() failed");
  }
}

