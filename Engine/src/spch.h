#pragma once
#include "sixcore.h"
#include <chrono>
#include <vector>
#include <thread>
#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <utility>

#if _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#endif