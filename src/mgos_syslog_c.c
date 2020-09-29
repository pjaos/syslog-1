#include <mgos.h>

struct mgos_syslog {
  const char *syslog;
  const char *hostname;
};

static struct mgos_syslog *s_syslog = NULL;
static const char *SYSLOG_NILVALUE = "-";
static uint32_t s_msgId = 1;
static bool s_init = false;

enum mgos_syslog_severity {
  SYSLOG_PRIO_EMERG,   /* system is unusable */
  SYSLOG_PRIO_ALERT,   /* action must be taken immediately */
  SYSLOG_PRIO_CRIT,    /* critical conditions */
  SYSLOG_PRIO_ERR,     /* error conditions */
  SYSLOG_PRIO_WARNING, /* warning conditions */
  SYSLOG_PRIO_NOTICE,  /* normal but significant condition */
  SYSLOG_PRIO_INFO,    /* informational */
  SYSLOG_PRIO_DEBUG,   /* debug-level messages */
};

enum mgos_syslog_facility {
  SYSLOG_FAC_KERN = 0,          /* kernel messages */
  SYSLOG_FAC_USER = 1,          /* random user-level messages */
  SYSLOG_FAC_MAIL = 2,          /* mail system */
  SYSLOG_FAC_DAEMON = 3,        /* system daemons */
  SYSLOG_FAC_AUTH = 4,          /* security/authorization messages */
  SYSLOG_FAC_SYSLOG = 5,        /* messages generated internally by syslogd */
  SYSLOG_FAC_LPR = 6,           /* line printer subsystem */
  SYSLOG_FAC_NEWS = 7,          /* network news subsystem */
  SYSLOG_FAC_UUCP = 8,          /* UUCP subsystem */
  SYSLOG_FAC_CRON = 9,          /* clock daemon */
  SYSLOG_FAC_AUTHPRIV = 10,     /* security/authorization messages (private) */
  SYSLOG_FAC_FTP = 11,          /* ftp daemon */
  SYSLOG_FAC_NTP = 12,          /* ftp daemon */
  SYSLOG_FAC_LOG_AUDIT = 13,    /* ftp daemon */
  SYSLOG_FAC_LOG_ALERT = 14,    /* ftp daemon */
  SYSLOG_FAC_CLOCK_DAEMON = 15, /* ftp daemon */
  SYSLOG_FAC_LOCAL0 = 16,       /* reserved for local use */
  SYSLOG_FAC_LOCAL1 = 17,       /* reserved for local use */
  SYSLOG_FAC_LOCAL2 = 18,       /* reserved for local use */
  SYSLOG_FAC_LOCAL3 = 19,       /* reserved for local use */
  SYSLOG_FAC_LOCAL4 = 20,       /* reserved for local use */
  SYSLOG_FAC_LOCAL5 = 21,       /* reserved for local use */
  SYSLOG_FAC_LOCAL6 = 22,       /* reserved for local use */
  SYSLOG_FAC_LOCAL7 = 23,       /* reserved for local use */
};

static bool init() {
  if (!s_init) {
    if (mgos_sys_config_is_initialized()) {
      s_syslog = (struct mgos_syslog *) calloc(1, sizeof(struct mgos_syslog));
      s_syslog->syslog = mgos_sys_config_get_syslog_url();
      if (NULL != s_syslog->syslog && (0 != strlen(s_syslog->syslog))) {
        s_syslog->hostname = mgos_sys_config_get_syslog_hostname();
        if (NULL != s_syslog->hostname && (0 != strlen(s_syslog->hostname))) {
          s_init = true;
        }
      }
    }
  }

  return s_init;
}

static void mgos_syslog_log(enum mgos_syslog_facility fac,
                            enum mgos_syslog_severity sev, const char *app_name,
                            const char *fmt, va_list arg) {
  //<34>1 2003-10-11T22:14:15.003Z mymachine.example.com
  // APP-NAME=su
  // PROCID=-
  // MSGID=ID47
  // STRUCTURED-DATA=-
  // BOM
  // MSG='su root' failed for lonvick on /dev/pts/8

  //<165>1 2003-08-24T05:14:15.000003-07:00 192.0.2.1
  // APP-NAME=myproc
  // PROCID=8710
  // MSGID=-
  // STRUCTURED-DATA=-
  // MSG=%% It's time to make the do-nuts.
  // syslog: <14>1 2017-03-17T08:54:03Z hF�? publishTemps - -
  // {"timestamp":1489740840,"temps":[{"id":"28ff95
  // syslog: <14>1 2017-03-17T08:59:03Z 192.168.0.31 publishTemps - -
  // {"timestamp":1489741140,"temps":[{"id":
  // header
  if (!init()) {
    return;
  }
  // time_t now = time(0);
  // struct tm* timeinfo = localtime(&now);
  // char timestamp[24];
  // strftime(timestamp, sizeof (timestamp), "%FT%TZ", timeinfo);
  time_t t = time(0);
  struct tm *timeinfo = localtime(&t);
  static char timestamp[24];
  strftime(timestamp, sizeof(timestamp), "%F %T", timeinfo);

  struct mg_connection *udp =
      mg_connect(mgos_get_mgr(), s_syslog->syslog, NULL, NULL);
  mg_printf(udp, "<%d> %s %s %s %s %lu %s ", (fac << 3) + sev, timestamp,
            s_syslog->hostname, app_name, SYSLOG_NILVALUE,
            (unsigned long) s_msgId, SYSLOG_NILVALUE);
  s_msgId++;

  mg_vprintf(udp, fmt, arg);
  mg_send(udp, "", 0);
  udp->flags |= MG_F_SEND_AND_CLOSE;
}

void mgos_syslog_log_error(const char *app_name, const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  mgos_syslog_log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_ERR, app_name, fmt, arg);
  va_end(arg);
}

void mgos_syslog_log_warn(const char *app_name, const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  mgos_syslog_log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_WARNING, app_name, fmt, arg);
  va_end(arg);
}

void mgos_syslog_log_info(const char *app_name, const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  mgos_syslog_log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_INFO, app_name, fmt, arg);
  va_end(arg);
}

void mgos_syslog_log_notice(const char *app_name, const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  mgos_syslog_log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_NOTICE, app_name, fmt, arg);
  va_end(arg);
}

void mgos_syslog_log_debug(const char *app_name, const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  mgos_syslog_log(SYSLOG_FAC_LOCAL0, SYSLOG_PRIO_DEBUG, app_name, fmt, arg);
  va_end(arg);
}

bool mgos_syslog_init(void) {
  return true;
}
