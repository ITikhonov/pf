#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "prefix.h"

uint32_t stack[32], tos=0, *dp=stack;
struct rp { uint16_t w,p; } rstack[32]={{-1}}, ip={0,0}, *sp=rstack+1;

void drop() { tos=*(--dp); }
void ret() { ip=*(--sp); }
void cmp() { *dp++=tos; tos=dp[-2]-tos; }
void dot() { printf("%i ",tos); }
void istack() {
	printf("<");
	uint32_t *p=stack;
	for(;p<dp;p++) { printf("%i ",*p); }
	printf("%i>",tos);
}

void builtin(int w) {
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
	}
}

void call(int w) {
	if(w&BLTIN) { builtin(w); return; }
	*sp++=ip; ip.w=w; ip.p=0;
}

void jmp(int w) {
	if(w&BLTIN) { ret(); builtin(w); return; }
	ip.w=w; ip.p=0;
}

void run() {
	for(;ip.w!=0xffff;) {
		uint16_t i=prog[ip.w][ip.p];
		ip.p++;
		int w=i&0xfff;

		if(i>>13) {
			int cond=((i&JZ)&&tos==0) || ((i&JN)&&(int32_t)tos<0) || ((i&JP)&&(int32_t)tos>0);
			if(cond) {
				switch(i&JMP) {
				case CALL: call(w); break;
				case JMP: jmp(w); break;
				default: printf("wtf %x\n",i&JMP);
				}
			}
		}  else {
			ret();
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

