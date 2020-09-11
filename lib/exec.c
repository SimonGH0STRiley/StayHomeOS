/*****************************************************************************
 *****************************************************************************
 * @file   lib/exec.c
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

/*****************************************************************************
 *                                exec
 *****************************************************************************/
/**
 * Executes the program pointed by path.
 * 
 * @param path  The full path of the file to be executed.
 * 
 * @return  Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int exec(const char * path)
{
	MESSAGE msg;
	msg.type	= EXEC;
	msg.PATHNAME	= (void*)path;
	msg.NAME_LEN	= strlen(path);
	msg.BUF		= 0;
	msg.BUF_LEN	= 0;

	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}

//execl()最终调用execv()来完成自己的执行
PUBLIC int execl(const char *path, const char *arg, ...)
{
	va_list parg = (va_list)(&arg);
	char **p = (char**)parg;
	return execv(path, p);
}

//任务即向MM提供最终供调用exec的进程使用的堆栈，argv相当于main函数中的输入参数
PUBLIC int execv(const char *path, char * argv[])
{
	char **p = argv;
	//准备好一块内存arg_stack[]
	char arg_stack[PROC_ORIGIN_STACK];
	int stack_len = 0;
    //遍历调用者传递给自己的参数，数参数个数
	while(*p++) {
		assert(stack_len + 2 * sizeof(char*) < PROC_ORIGIN_STACK);
		stack_len += sizeof(char*);
	}
    //将指针数组的末尾赋零
	*((int*)(&arg_stack[stack_len])) = 0;
	stack_len += sizeof(char*);

	char ** q = (char**)arg_stack;
	for (p = argv; *p != 0; p++) {
	    //将每个字符串的地址写入指针数组的正确位置
		*q++ = &arg_stack[stack_len];

		assert(stack_len + strlen(*p) + 1 < PROC_ORIGIN_STACK);
		//将字符串复制到arg_stack[]中
		strcpy(&arg_stack[stack_len], *p);
		stack_len += strlen(*p);
		arg_stack[stack_len] = 0;
		stack_len++;
	}
    //将arg_stack[]的首地址以及其中有效内容的长度等发送给MM，这样MM就可以实际的进行exec了
	MESSAGE msg;
	msg.type	= EXEC;
	msg.PATHNAME	= (void*)path;
	msg.NAME_LEN	= strlen(path);
	msg.BUF		= (void*)arg_stack;
	msg.BUF_LEN	= stack_len;

	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}

