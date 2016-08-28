/*
 * Copyright (c) 2016, Hugo Freire <hugo@exec.sh>.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <err.h>

#include "../include/gpio.h"

const gchar *GPIO_PIN;
const gchar *GPIO_TRIGGER_MODE;

GThreadPool *thread_pool;
GThread *poll_thread;

void shutdown_gracefully(const int status, const char *reason) {
  unexport_gpio_pin(GPIO_PIN);

  if (reason != NULL) {
    fprintf(status ? stdout : stderr, "Shutting down because %s\n", reason);
  }

  g_thread_pool_free(thread_pool, false, false);

  exit(status);
}

void signal_handler(int signal) {
  shutdown_gracefully(0, "received signal");
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void *poll_gpio() {
  int gpio_value_fd;
  struct pollfd poll_fd;
  unsigned long long *epoch;
  struct timeval tv;

  if (setup_gpio(GPIO_PIN, GPIO_TRIGGER_MODE) < 0) {
    shutdown_gracefully(1, "setup_gpio(GPIO_PIN, GPIO_TRIGGER_MODE) failed");
  }

  if ((gpio_value_fd = open_gpio_value(GPIO_PIN)) < 0) {
    shutdown_gracefully(1, "open_gpio_value(GPIO_PIN) failed");
  }

  poll_fd.fd = gpio_value_fd;
  poll_fd.events = POLLPRI; // poll() will return only if there is urgent data to read

  read_gpio_value(gpio_value_fd);

#ifndef __arm__
  int skip = false;
#endif

  while (true) {
    if ((epoch = malloc(sizeof(unsigned long long))) == NULL) {
      shutdown_gracefully(1, "malloc() failed");
    }

    lseek(gpio_value_fd, 0, SEEK_SET);

    if (poll(&poll_fd, 1, -1) < 0) { // poll forever
      shutdown_gracefully(1, "poll() failed");
    }

#ifndef __arm__
    if (skip == false) {
      skip = true;
    }
    else {
      skip = false;
      continue;
    } // TODO need to work fifo files in darwin
#endif

    if (gettimeofday(&tv, NULL) < 0) {
      shutdown_gracefully(1, "gettimeofday() failed");
    }

    *epoch = (unsigned long long) (tv.tv_sec) * 1000 + (unsigned long long) (tv.tv_usec) / 1000;

    read_gpio_value(gpio_value_fd);
    read_gpio_value(gpio_value_fd);

    g_thread_pool_push (thread_pool, &epoch, NULL);
  }
}
#pragma clang diagnostic pop

void *submit_data(unsigned long long *epoch) {
  printf("[thread-%p] Detected %s edge in GPIO %s at %llu\n", g_thread_self(), GPIO_TRIGGER_MODE, GPIO_PIN, *epoch);

  return NULL;
}

int main() {
  gchar **environ = g_get_environ();

  GPIO_PIN = g_environ_getenv(environ, "RASPOWMETER_GPIO_PIN");
  GPIO_TRIGGER_MODE = g_environ_getenv(environ, "RASPOWMETER_GPIO_TRIGGER_MODE");

  if (GPIO_PIN == NULL || *GPIO_PIN == '\0') {
    errx(-1, "environment variable RASPOWMETER_GPIO_PIN is not set");
  }

  if (GPIO_TRIGGER_MODE == NULL || *GPIO_TRIGGER_MODE == '\0') {
    errx(-1, "environment variable RASPOWMETER_GPIO_TRIGGER_MODE is not set");
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  thread_pool = g_thread_pool_new((GFunc) submit_data, NULL, 5, true, NULL);

  poll_thread = g_thread_new("poll_thread", (void *) &poll_gpio, NULL);
  g_thread_join(poll_thread);

  return 0;
}