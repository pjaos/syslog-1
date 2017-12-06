#pragma once
//#include "Syslog.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void mgos_syslog_logError(const char* appName, const char* fmt, ...);
    void mgos_syslog_logWarn(const char* appName, const char* fmt, ...);
    void mgos_syslog_logInfo(const char* appName, const char* fmt, ...);
    void mgos_syslog_logNotice(const char* appName, const char* fmt, ...);
    void mgos_syslog_logDebug(const char* appName, const char* fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */
