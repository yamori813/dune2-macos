// duned.h

#ifndef DUNED_H
#define DUNED_H 1

#define MAXCMDLEN   1024
#define DEFAULTPORT 2124
#define LOGNAME "/var/log/duned.log"

int duned_init();
int duned_shutdown();
int duned_handle_command(char *s);

#endif
