#include "Counter.h"

void Counter::operator()() { ++count; }
int Counter::get() const { return count; }