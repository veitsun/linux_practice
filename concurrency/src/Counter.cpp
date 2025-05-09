#include "Counter.h"

int Counter::operator()() { return ++count; }
int Counter::get() const { return count; }