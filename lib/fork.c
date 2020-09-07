/*************************************************************************//**
 *****************************************************************************
 * @file   fork.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue May  6 14:22:13 2008
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


//创建子进程，调用fork()时说明MM收到一个FORK消息
//如果成功了，则在父进程中返回子进程的PID，在子进程中返回0
//如果失败了，返回-1，不创建进程
PUBLIC int fork()
{
	MESSAGE msg;
	msg.type = FORK;

	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);
	assert(msg.RETVAL == 0);

	return msg.PID;
}
