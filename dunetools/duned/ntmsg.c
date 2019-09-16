// ntmsg.c
//
// Time-stamp: <22/02/03 12:20:19 keuchel@rainer-keuchel.de>

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/time.h>
#include "duned.h"

extern const char *lpServiceName;

#define KEYNAME \
"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application"

/* install message source for event loggging */

int
InstallMsgSource()
{
  char lpszModuleName[512];
  char lpszKeyName[512];
  HKEY hk;
  DWORD dwData;

  if(GetModuleFileName(NULL, lpszModuleName, 512) == 0)
    return -1;

  sprintf(lpszKeyName, "%s\\%s", KEYNAME, lpServiceName);

  /* Add your source name as a subkey under the Application 
     key in the EventLog service portion of the registry.  */  

  if(RegCreateKey(HKEY_LOCAL_MACHINE, lpszKeyName, &hk)) 
    return -1;

  /* Set the Event ID message-file name. */  

  /* Add the Event ID message-file name to the subkey. */  
  if (RegSetValueEx(hk,                       /* subkey handle         */ 
		    "EventMessageFile",       /* value name            */ 
		    0,                        /* must be zero          */ 
		    REG_EXPAND_SZ,            /* value type            */ 
		    (LPBYTE) lpszModuleName,  /* address of value data */ 
		    strlen(lpszModuleName) + 1)) /* length of value data  */ 
    return -1;

  /* Set the supported types flags. */  
  dwData = 
    EVENTLOG_ERROR_TYPE | 
    EVENTLOG_WARNING_TYPE | 
    EVENTLOG_INFORMATION_TYPE;  

  if (RegSetValueEx(hk,                /* subkey handle                */ 
		    "TypesSupported",  /* value name                   */ 
		    0,                 /* must be zero                 */ 
		    REG_DWORD,         /* value type                   */ 
		    (LPBYTE) &dwData,  /* address of value data        */ 
		    sizeof(DWORD)))    /* length of value data         */ 
    return -1;

  RegCloseKey(hk);  

  return 0;
}

int
LogMessage(int logtype, char *buf)
{
  HANDLE hLog;
  char *ary[1];

  if((hLog = OpenEventLog(NULL, lpServiceName)) == NULL)
    return GetLastError();

  ary[0] = buf;

  if(ReportEvent(hLog, (short) logtype, 0, logtype, NULL, 1, 0, ary,
		 NULL) == 0)
    return GetLastError();

  CloseEventLog(hLog);
  return 0;
}

void
openlog(const char *s, int a, int b)
{
}

void
syslog(int type, const char *fmt, ...)
{
  int len;
  char buf[1024];
  va_list ap;
  FILE *logf;

  switch(type)
    {
    case LOG_NOTICE:
      type = EVENTLOG_INFORMATION_TYPE;
      break;
    case LOG_ERR:
      type = EVENTLOG_ERROR_TYPE;
      break;
    case LOG_WARNING:
      type = EVENTLOG_WARNING_TYPE;
      break;
    default:
      type = EVENTLOG_INFORMATION_TYPE;
      break;
    }

  va_start(ap, fmt);
  len = vsprintf(buf, fmt, ap);
  va_end(ap);

  LogMessage(type, buf);

  if((logf = fopen(LOGNAME, "a")) != NULL)
    {
      time_t t = time(NULL);

      fprintf(logf, "%.24s: %s\n", ctime(&t), buf);
      fclose(logf);
    }
  return;
}
