

#ifndef	_STAYHOMEOS_PROTO_H_
#define	_STAYHOMEOS_PROTO_H_

#include "const.h"

/* kliba.asm */
PUBLIC void	out_byte(u16 port, u8 value);
PUBLIC u8	in_byte(u16 port);
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);
PUBLIC void	init_prot();
PUBLIC void	init_8259A();

/* klib.c */
PUBLIC void disp_int(int input);

#endif
/* _STAYHOMEOS_PROTO_H_ */