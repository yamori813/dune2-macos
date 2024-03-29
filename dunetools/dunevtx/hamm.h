#ifndef HAMM_H
#define HAMM_H

typedef unsigned char u8;

int hamm8(u8 *p, int *err);
int hamm16(u8 *p, int *err);
int hamm24(u8 *p, int *err);
int chk_parity(u8 *p, int n);

#endif
