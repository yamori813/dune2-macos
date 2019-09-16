// service.h
//
// Time-stamp: <22/02/03 13:50:40 keuchel@rainer-keuchel.de on mediadev>

#ifndef SERVICE_H
#define SERVICE_H 1

#define SERVICE_NAME        "dunesrv"
#define SERVICE_DISPLAYNAME "Dune Server";

void main_thread(void *);

/* service.c */
VOID WINAPI MyServiceStart(DWORD argc, LPTSTR *argv);
VOID WINAPI MyServiceCtrlHandler(DWORD opcode);
DWORD MyServiceInit(DWORD argc, LPTSTR *argv, DWORD *error);
VOID SvcDebugPrintf(const char *fmt, ...);

/* install-service.c */
void create_service();
void delete_service();
void start_service();
void stop_service();

/* ntmsg.c */
int InstallMsgSource();

#endif

