#include <mgos.h>

#include "Syslog.h"

Syslog sysLog;

const char* Syslog::SYSLOG_NILVALUE = "-";
uint32_t Syslog::_msgId = 1;
bool Syslog::_init = false;

void Syslog::log(Facility fac, Severity sev, const char* appName, const char* fmt, va_list arg)
{
    //<34>1 2003-10-11T22:14:15.003Z mymachine.example.com
    //APP-NAME=su
    //PROCID=-
    //MSGID=ID47
    //STRUCTURED-DATA=-
    //BOM
    //MSG='su root' failed for lonvick on /dev/pts/8

    //<165>1 2003-08-24T05:14:15.000003-07:00 192.0.2.1
    //APP-NAME=myproc
    //PROCID=8710
    //MSGID=-
    //STRUCTURED-DATA=-
    //MSG=%% It's time to make the do-nuts.
    //syslog: <14>1 2017-03-17T08:54:03Z hF�? publishTemps - - {"timestamp":1489740840,"temps":[{"id":"28ff95
    //syslog: <14>1 2017-03-17T08:59:03Z 192.168.0.31 publishTemps - - {"timestamp":1489741140,"temps":[{"id":
    //header
    if (!Init()) {
        return;
    }
    //time_t now = time(0);
    //struct tm* timeinfo = localtime(&now);
    //char timestamp[24];
    //strftime(timestamp, sizeof (timestamp), "%FT%TZ", timeinfo);
    time_t t = time(0);
    struct tm* timeinfo = localtime(&t);
    static char timestamp[24];
    strftime(timestamp, sizeof (timestamp), "%FT%TZ", timeinfo);

    struct mg_connection* udp = mg_connect(mgos_get_mgr(), _syslog, NULL, NULL);
    mg_printf(udp, "<%d> %s %s %s %s %d %s ", (fac << 3) + sev, timestamp, _hostname, appName, SYSLOG_NILVALUE, _msgId, SYSLOG_NILVALUE);
    _msgId++;

    mg_vprintf(udp, fmt, arg);
    mg_send(udp, "", 0);
    udp->flags |= MG_F_SEND_AND_CLOSE;
}

void Syslog::logError(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_ERR, appName, fmt, arg);
    va_end(arg);
}

void Syslog::logWarn(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_WARNING, appName, fmt, arg);
    va_end(arg);
}

void Syslog::logInfo(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_INFO, appName, fmt, arg);
    va_end(arg);
}

void Syslog::logNotice(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_NOTICE, appName, fmt, arg);
    va_end(arg);
}

void Syslog::logDebug(const char* appName, const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_DEBUG, appName, fmt, arg);
    va_end(arg);
}

bool Syslog::Init()
{
    if (!_init) {
        if (mgos_sys_config_is_initialized()) {
            _syslog = mgos_sys_config_get_syslog_url();
            if (NULL != _syslog && (0 != strlen(_syslog))) {
                _hostname = mgos_sys_config_get_syslog_hostname();
                if (NULL != _hostname && (0 != strlen(_hostname))) {
                    _init = true;
                }
            }
        }
    }

    return _init;
}
