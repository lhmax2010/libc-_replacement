#pragma once

extern "C" {
typedef void (*r14_observer_fn)(const char *event);
typedef void (*r14_throw_fn)(void);
}

struct CrossError {
  int origin;
};
