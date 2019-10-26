#pragma once

#ifdef INCLUDE_AMALGAMATION
#define AMALGAMATION(file) "notes.hpp"
#else
#define AMALGAMATION(file) #file
#endif
