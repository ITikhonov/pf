#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "prefix.h"

static uint64_t stack[32], tos=0, *dp=stack;
static struct rp { uint16_t w,p; } rstack[32]={{-1}}, ip={0,0}, *sp=rstack+1;
static int flag=0;

static void drop() { tos=*(--dp); }
static void ret() { ip=*(--sp); }
static void cmp() { flag=dp[-1]-tos; }
static void dot() { printf("%li ",tos); fflush(stdout); }
static void istack() { printf("<"); uint64_t *p=stack; for(;p<dp;p++) { printf("%li ",*p); } printf("%li>",tos); fflush(stdout); }
static void fetch() { tos=*(uint64_t*)tos; }
static void store() { *(uint64_t*)tos=dp[-1]; }
static void add() { tos=*(--dp)+tos; }
static void sub() { tos=*(--dp)-tos; }
static void choose() { ip.p+=tos; }
static void nip() { --dp; }

static void builtin(int w) {
	if(w&0x400) {
		*dp++=tos;
		tos=prog_numbers[w&0x3ff];
		return;
	}
	w&=0x7ff;
	switch(w) {
	case 1: putchar(tos); break;
	case 2: drop(); break;
	case 3: cmp(); break;
	case 4: dot(); break;
	case 5: istack(); break;
	case 6: fetch(); break;
	case 7: store(); break;
	case 8: add(); break;
	case 9: sub(); break;
	case 10: choose(); break;
	case 11: nip(); break;
	}
}

static void call(int w) {
	if(w&BLTIN) { builtin(w); return; }
	*sp++=ip; ip.w=w; ip.p=0;
}

static void jmp(int w) {
	if(w&BLTIN) { ret(); builtin(w); return; }
	ip.w=w; ip.p=0;
}

static void address(int w) {
	if(w&0x400) {
		*dp++=tos;
		tos=(uint64_t)&prog_numbers[w&0x3ff];
		return;
	}
}

void run() {
	for(;ip.w!=0xffff;) {
		uint16_t i=prog[ip.w][ip.p];
		ip.p++;
		int w=i&0xfff;

		if(i>>13) {
			int cond=((i&JZ)&&flag==0) || ((i&JN)&&flag<0) || ((i&JP)&&flag>0);
			if(cond) {
				switch(i&JMP) {
				case CALL: call(w); break;
				case JMP: jmp(w); break;
				default: printf("wtf %x\n",i&JMP);
				}
			}
		}  else {
			address(w);
		}
	}
}

int main() {
	for(;;) {
		int c=getchar();
		if(c==-1) break;
		tos=c; dp=stack; ip.w=0; ip.p=0; sp=rstack+1;
		run();
	}
	return 0;
}

