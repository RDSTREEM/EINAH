#ifndef NATIVE_H
#define NATIVE_H

#include <memory>
#include "runtime/environment.h"

void registerNativeFunctions(std::shared_ptr<Environment> env);

#endif // NATIVE_H
