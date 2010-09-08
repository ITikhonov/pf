


enum iprefix { CALL=0<<12, JMP=1<<12,
		JZ=1<<13,
		JN=1<<14,
		JP=1<<15,
		ZR=0,
		BLTIN=1<<11, NUM=3<<10 };

extern uint16_t *prog[];
extern uint64_t prog_numbers[];

