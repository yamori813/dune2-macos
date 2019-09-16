// serviceinst.c
//
// Time-stamp: <29/09/02 18:54:24 keuchel@rainer-keuchel.de>

#include <windows.h>
#include <stdio.h>
#include "service.h"

LPCTSTR lpServiceName = SERVICE_NAME;
LPCTSTR lpDisplayName = SERVICE_DISPLAYNAME;

static LPCTSTR lpServiceStartName = "LocalSystem";
static LPCTSTR lpPassword = NULL;

static DWORD dwDesiredAccess = SERVICE_ALL_ACCESS;
static DWORD dwServiceType   = SERVICE_WIN32_OWN_PROCESS;
static DWORD dwStartType     = SERVICE_AUTO_START;
static DWORD dwErrorControl  = SERVICE_ERROR_NORMAL;

static LPCTSTR lpLoadOrderGroup = NULL;
static LPDWORD lpdwTagId        = NULL; // must be NULL for install!
static LPCTSTR lpDependencies   = NULL;

static SC_HANDLE hSC          = NULL;
static SC_HANDLE hSCManager   = NULL;

static char lpMachineName[126];
static char lpBinaryPathName[126];

static LPCTSTR lpDatabaseName = SERVICES_ACTIVE_DATABASE;

int
install_service()
{
  DWORD dwSize = sizeof(lpMachineName);

  fprintf(stderr, "Installing service %s...\n", lpDisplayName);

  GetComputerName(lpMachineName, &dwSize);
  GetModuleFileName(NULL, lpBinaryPathName, sizeof(lpBinaryPathName));

  hSCManager = OpenSCManager(lpMachineName, lpDatabaseName, 
			     SC_MANAGER_ALL_ACCESS);
  if(hSCManager == NULL)
    {
      fprintf(stderr, "OpenSCManager: Error %d\n", GetLastError());
      return 1;
    }

  fprintf(stderr, "Creating service %s...\n", lpDisplayName);
  create_service();
  fprintf(stderr, "Starting service %s...\n", lpDisplayName);
  start_service();

  CloseServiceHandle(hSCManager);

  if(InstallMsgSource() < 0)
    fprintf(stderr, "Cannot install service message file\n");

  return 0;
}

int
remove_service()
{
  DWORD dwSize = sizeof(lpMachineName);

  fprintf(stderr, "Removing service %s...\n", lpDisplayName);

  GetComputerName(lpMachineName, &dwSize);
  GetModuleFileName(NULL, lpBinaryPathName, sizeof(lpBinaryPathName));

  hSCManager = OpenSCManager(lpMachineName, lpDatabaseName, 
			     SC_MANAGER_ALL_ACCESS);
  if(hSCManager == NULL)
    {
      fprintf(stderr, "OpenSCManager: Error %d\n", GetLastError());
      return 1;
    }

  fprintf(stderr, "Stopping service %s...\n", lpDisplayName);
  stop_service();
  fprintf(stderr, "Deleting service %s...\n", lpDisplayName);
  delete_service();

  CloseServiceHandle(hSCManager);
  return 0;
}

void
create_service()
{
  hSC = CreateService(hSCManager, lpServiceName, lpDisplayName,
		      dwDesiredAccess, dwServiceType, dwStartType,
		      dwErrorControl, lpBinaryPathName, lpLoadOrderGroup,
		      lpdwTagId, lpDependencies, lpServiceStartName,
		      lpPassword);
  if(hSC == NULL)
    {
      DWORD dwErr = GetLastError();

      if(dwErr != ERROR_SERVICE_EXISTS)
	fprintf(stderr, "CreateService: Error %d\n", dwErr);
      return;
    }

  CloseServiceHandle(hSC);
}

void
delete_service()
{
  hSC = OpenService(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);
  if(hSC == NULL)
    {
      DWORD dwErr = GetLastError();

      if(dwErr != ERROR_SERVICE_DOES_NOT_EXIST)
	fprintf(stderr, "OpenService: Error %d\n", GetLastError());
      return;
    }
  if(!DeleteService(hSC))
    {
      fprintf(stderr, "DeleteService: Error %d\n", GetLastError());
    }
  CloseServiceHandle(hSC);
}

void
start_service()
{
  hSC = OpenService(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);
  if(hSC == NULL)
    {
      fprintf(stderr, "OpenService: Error %d\n", GetLastError());
      return;
    }
  if(!StartService(hSC, 0, NULL))
    {
      DWORD dwErr = GetLastError();

      if(dwErr != ERROR_SERVICE_ALREADY_RUNNING)
	fprintf(stderr, "StartService: Error %d\n", GetLastError());
    }
  CloseServiceHandle(hSC);
}

void
stop_service()
{
  SERVICE_STATUS status;

  hSC = OpenService(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);
  if(hSC == NULL)
    {
      DWORD dwErr = GetLastError();

      if(dwErr != ERROR_SERVICE_DOES_NOT_EXIST)
	fprintf(stderr, "OpenService: Error %d\n", GetLastError());
      return;
    }
  if(!ControlService(hSC, SERVICE_CONTROL_STOP, &status))
    {
      fprintf(stderr, "ControlService: Error %d\n", GetLastError());
    }
  CloseServiceHandle(hSC);
}

