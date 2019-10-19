#pragma once

#if __cpp_exceptions || __EXCEPTIONS
#define EXCEPTIONS_ENABLED
#endif

#ifdef EXCEPTIONS_ENABLED
#define THROW(exception) throw exception
#else
#define THROW(exception) exit(1)
#endif