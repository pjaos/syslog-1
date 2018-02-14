#pragma once

#ifndef NULL
#define NULL 0
#endif

enum Severity
{
    SYSLOG_PRIO_EMERG, /* system is unusable */
    SYSLOG_PRIO_ALERT, /* action must be taken immediately */
    SYSLOG_PRIO_CRIT, /* critical conditions */
    SYSLOG_PRIO_ERR, /* error conditions */
    SYSLOG_PRIO_WARNING, /* warning conditions */
    SYSLOG_PRIO_NOTICE, /* normal but significant condition */
    SYSLOG_PRIO_INFO, /* informational */
    SYSLOG_PRIO_DEBUG, /* debug-level messages */
};

enum Facility
{
    SYSLOG_FAC_KERN = 0, /* kernel messages */
    SYSLOG_FAC_USER = 1, /* random user-level messages */
    SYSLOG_FAC_MAIL = 2, /* mail system */
    SYSLOG_FAC_DAEMON = 3, /* system daemons */
    SYSLOG_FAC_AUTH = 4, /* security/authorization messages */
    SYSLOG_FAC_SYSLOG = 5, /* messages generated internally by syslogd */
    SYSLOG_FAC_LPR = 6, /* line printer subsystem */
    SYSLOG_FAC_NEWS = 7, /* network news subsystem */
    SYSLOG_FAC_UUCP = 8, /* UUCP subsystem */
    SYSLOG_FAC_CRON = 9, /* clock daemon */
    SYSLOG_FAC_AUTHPRIV = 10, /* security/authorization messages (private) */
    SYSLOG_FAC_FTP = 11, /* ftp daemon */
    SYSLOG_FAC_NTP = 12, /* ftp daemon */
    SYSLOG_FAC_LOG_AUDIT = 13, /* ftp daemon */
    SYSLOG_FAC_LOG_ALERT = 14, /* ftp daemon */
    SYSLOG_FAC_CLOCK_DAEMON = 15, /* ftp daemon */
    SYSLOG_FAC_LOCAL0 = 16, /* reserved for local use */
    SYSLOG_FAC_LOCAL1 = 17, /* reserved for local use */
    SYSLOG_FAC_LOCAL2 = 18, /* reserved for local use */
    SYSLOG_FAC_LOCAL3 = 19, /* reserved for local use */
    SYSLOG_FAC_LOCAL4 = 20, /* reserved for local use */
    SYSLOG_FAC_LOCAL5 = 21, /* reserved for local use */
    SYSLOG_FAC_LOCAL6 = 22, /* reserved for local use */
    SYSLOG_FAC_LOCAL7 = 23, /* reserved for local use */
};

class Syslog
{
public:

    Syslog()
    : _syslog(NULL)
    , _hostname(NULL)
    {
    }

    ~Syslog()
    {
    }
    void logError(const char* appName, const char* fmt, ...);
    void logWarn(const char* appName, const char* fmt, ...);
    void logInfo(const char* appName, const char* fmt, ...);
    void logNotice(const char* appName, const char* fmt, ...);
    void logDebug(const char* appName, const char* fmt, ...);

    void log(Facility fac, Severity sev, const char* appName, const char* fmt, va_list arg);
private:

    static const char* SYSLOG_NILVALUE; // "-"
    static uint32_t _msgId;

    const char* _syslog;
    const char* _hostname;
    static bool _init;

    bool Init();
};

extern Syslog sysLog;
