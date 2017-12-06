#include <mgos.h>
#include "mgos_Syslog.h"
#include "Syslog.h"

void mgos_syslog_logError(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    sysLog.log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_ERR, appName, fmt, arg);
    va_end(arg);
}

void mgos_syslog_logWarn(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    sysLog.log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_WARNING, appName, fmt, arg);
    va_end(arg);
}

void mgos_syslog_logInfo(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    sysLog.log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_INFO, appName, fmt, arg);
    va_end(arg);
}

void mgos_syslog_logNotice(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    sysLog.log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_NOTICE, appName, fmt, arg);
    va_end(arg);
}

void mgos_syslog_logDebug(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    sysLog.log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_DEBUG, appName, fmt, arg);
    va_end(arg);
}
