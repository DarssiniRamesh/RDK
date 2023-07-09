#include <stdarg.h>
#include <syslog.h>
#include "Wraps.h"
#include <mntent.h>

extern "C" int __wrap_system(const char* command)
{
    return Wraps::getInstance().system(command);
}

extern "C" FILE* __wrap_popen(const char* command, const char* type)
{
    return Wraps::getInstance().popen(command, type);
}

//This function will be called for syslog() in the code (added -Wl,-wrap,syslog)
extern  "C" void __wrap_syslog(int pri, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Wraps::getInstance().syslog(pri, fmt, args);
    vsyslog(pri, fmt, args);
    va_end(args);
}

extern "C" FILE* __wrap_setmntent(const char* command, const char* type)
{
    return Wraps::getInstance().setmntent(command, type);
}

extern "C" struct mntent* __wrap_getmntent(FILE* pipe)
{
    return Wraps::getInstance().getmntent(pipe);
}