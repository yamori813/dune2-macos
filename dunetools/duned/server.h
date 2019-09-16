#ifndef SERVER_H
#define SERVER_H 1

#define PIDNAME "/tmp/duned.pid"

char *server_getline(char *s, size_t size);
int server_reply(const char *fmt, ...);
void server_dprintf(const char *fmt, ...);

#endif

