/*****************************************************************************
 *****************************************************************************
 * @file   stat.c
 * @brief
 * @author TF 141
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"


//根据路径返回文件信息
PUBLIC int stat(const char *path, struct stat *buf)
{
	MESSAGE msg;

	msg.type	= STAT;

	msg.PATHNAME	= (void*)path;
	msg.BUF		= (void*)buf;
	msg.NAME_LEN	= strlen(path);

	send_recv(BOTH, TASK_FS, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}
