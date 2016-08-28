/*
 * Copyright (c) 2016, Hugo Freire <hugo@exec.sh>.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RASPOWMETER_GPIO_H
#define RASPOWMETER_GPIO_H

#ifdef __arm__
  #define SYS_GPIO_FS "/sys/class"
#else
  #define SYS_GPIO_FS "/tmp"
#endif

#define SYS_GPIO_UNEXPORT_FS   SYS_GPIO_FS "/gpio/unexport"
#define SYS_GPIO_EXPORT_FS     SYS_GPIO_FS "/gpio/export"
#define SYS_GPIO_DIRECTION_FS  SYS_GPIO_FS "/gpio/gpio%s/direction"
#define SYS_GPIO_EDGE_FS       SYS_GPIO_FS "/gpio/gpio%s/edge"
#define SYS_GPIO_VALUE_FS      SYS_GPIO_FS "/gpio/gpio%s/value"

int unexport_gpio_pin(const char *gpio_pin);

int export_gpio_pin(const char *gpio_pin);

int set_gpio_direction(const char *gpio_pin);

int set_gpio_trigger_mode(const char *gpio_pin, const char *gpio_trigger_mode);

int setup_gpio(const char *gpio_pin, const char *gpio_trigger_mode);

int open_gpio_value(const char *gpio_pin);

void read_gpio_value(int gpio_value_fd);

#endif //RASPOWMETER_GPIO_H
