// service.c
//
// Time-stamp: <29/09/02 16:35:37 keuchel@rainer-keuchel.de>

#ifdef _SERVICE

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include "service.h"

static HANDLE thread_handle;
static SERVICE_STATUS MyServiceStatus;
static SERVICE_STATUS_HANDLE MyServiceStatusHandle;

int
main(int argc, char **argv)
{
  SERVICE_TABLE_ENTRY DispatchTable[] =
  {
    {SERVICE_NAME, MyServiceStart},
    {NULL, NULL}
  };

  char username[126];
  DWORD dwLen = sizeof(username);
    
  GetUserName(username, &dwLen);

  /* child */
  if(getenv("NETSOCK") != NULL)
    {
      main_thread(NULL);
    }
  /* foreground flag */
  else if(argv[1] && strcmp(argv[1], "-f") == 0)
    {
      main_thread(NULL);
    }
  else if(argv[1] && strcmp(argv[1], "-s") == 0)
    {
      install_service();
    }
  else if(argv[1] && strcmp(argv[1], "-r") == 0)
    {
      remove_service();
    }
  else if(stricmp(username, "system") != 0)
    {
      main_thread(NULL);
    }
  else
    /* service */
    {
      if(!StartServiceCtrlDispatcher(DispatchTable))
	{
	  SvcDebugPrintf("%s: StartServiceCtrlDispatcher: Error %d\n",
			 SERVICE_NAME, GetLastError());
	}
    }

  return 0;
}

VOID WINAPI
MyServiceStart(DWORD argc, LPTSTR *argv)
{
  DWORD status;
  DWORD specificError;

  MyServiceStatus.dwServiceType = SERVICE_WIN32;
  MyServiceStatus.dwCurrentState = SERVICE_START_PENDING;
  MyServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
  MyServiceStatus.dwWin32ExitCode = 0;
  MyServiceStatus.dwServiceSpecificExitCode = 0;
  MyServiceStatus.dwCheckPoint = 0;
  MyServiceStatus.dwWaitHint = 0;

  MyServiceStatusHandle = RegisterServiceCtrlHandler
    (SERVICE_NAME, MyServiceCtrlHandler);
  if(MyServiceStatusHandle == (SERVICE_STATUS_HANDLE) 0)
    {
      SvcDebugPrintf("%s: RegisterServiceCtrlHandler: Error %d\n",
		     SERVICE_NAME, GetLastError());
      return;
    }

  // initialization...
  status = MyServiceInit(argc, argv, &specificError);

  if(status != NO_ERROR)
    {
      MyServiceStatus.dwCurrentState = SERVICE_STOPPED;
      MyServiceStatus.dwWin32ExitCode = status;
      MyServiceStatus.dwServiceSpecificExitCode = specificError;
      MyServiceStatus.dwCheckPoint = 0;
      MyServiceStatus.dwWaitHint = 0;

      SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus);
      return;
    }

  MyServiceStatus.dwCurrentState = SERVICE_RUNNING;
  MyServiceStatus.dwCheckPoint = 0;
  MyServiceStatus.dwWaitHint = 0;

  if(!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
    {
      status = GetLastError();
      SvcDebugPrintf("%s: SetServiceStatus: Error %d\n",
		     SERVICE_NAME, status);
    }

  WaitForSingleObject(thread_handle, INFINITE);
}

VOID WINAPI
MyServiceCtrlHandler(DWORD opcode)
{
  DWORD status;

  switch(opcode)
    {
    case SERVICE_CONTROL_STOP:
      TerminateThread(thread_handle, 0);

      MyServiceStatus.dwCurrentState = SERVICE_STOPPED;
      MyServiceStatus.dwWin32ExitCode = 0;
      MyServiceStatus.dwCheckPoint = 0;
      MyServiceStatus.dwWaitHint = 0;

      if(!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
	{
	  status = GetLastError();
	  SvcDebugPrintf("%s: SetServiceStatus: Error %d\n",
			 SERVICE_NAME, status);
	}
      syslog(LOG_NOTICE, "Service stopped");
      return;
    default:
      SvcDebugPrintf("%s: Unknown opcode: %d\n", SERVICE_NAME, opcode);
    }

  if(!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
    {
      status = GetLastError();
      SvcDebugPrintf("%s: SetServiceStatus: Error %d\n",
		     SERVICE_NAME, status);
    }
  return;
}

DWORD
MyServiceInit(DWORD argc, LPTSTR *argv, DWORD *error)
{
  DWORD dwId;
  char dirname[126];

  thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main_thread, 
			       NULL, 0, &dwId);
  if(thread_handle == NULL)
    {
      syslog(LOG_ERR, "Cannot create thread");
      return GetLastError();
    }

#if 0
  /* Services are started in the %SYSTEM% directory, 
     we changed in main_thread */
  GetCurrentDirectory(sizeof(dirname), dirname);
  syslog(LOG_NOTICE, "Service started in %s", dirname);
#else
  syslog(LOG_NOTICE, "Service started");
#endif

  return 0;
}

VOID
SvcDebugPrintf(const char *fmt, ...)
{
  va_list ap;
  char buf[1024];

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  syslog(LOG_ERR, "%s", buf);
}

#endif // SERVICE
