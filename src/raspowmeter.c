/*
 * Copyright (c) 2016, Hugo Freire <hugo@exec.sh>.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <sys/time.h>

#include "../include/gpio.h"

GThreadPool *thread_pool;
pthread_t poll_thread;
int status;

void shutdown_gracefully(const int reason_status, const char *reason) {
  unexport_gpio_pin();

  if (reason != NULL) {
    fprintf(status ? stdout : stderr, "Shutting down because %s\n", reason);
  }

  status = reason_status;

  pthread_cancel(poll_thread);
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

  if (setup_gpio() < 0) {
    shutdown_gracefully(1, "setup_gpio() failed");
  }

  if ((gpio_value_fd = open(SYS_GPIO_VALUE_FS, O_RDONLY)) < 0) {
    shutdown_gracefully(1, "open(SYS_GPIO_VALUE) failed");
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
  fflush(stdout);

  return NULL;
}

int main() {
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  thread_pool = g_thread_pool_new((GFunc) submit_data, NULL, 5, true, NULL);

  if (pthread_create(&poll_thread, NULL, (void *) &poll_gpio, NULL) != 0) {
    shutdown_gracefully(1, "phtread_create() failed");
  }

  if (pthread_join(poll_thread, NULL) != 0) {
    shutdown_gracefully(1, "pthread_join() failed");
  }

  g_thread_pool_free(thread_pool, false, false);

  return status;
}