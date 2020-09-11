/*****************************************************************************
 *****************************************************************************
 * @file   forkexit.c
 * @brief
 * @author TF141
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
#include "keyboard.h"
#include "proto.h"


PRIVATE void cleanup(struct proc * proc);

//相应fork()函数中的系统调用
//成功返回0,否则返回-1
PUBLIC int do_fork()
{
	//在proc_table中找到一个空的槽位
	struct proc* p = proc_table;
	int i;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p++)
		if (p->p_flags == FREE_SLOT)
			break;
   //分配进程表，找到槽位后存放子进程的进程表
	int child_pid = i;
	assert(p == &proc_table[child_pid]);
	assert(child_pid >= NR_TASKS + NR_NATIVE_PROCS);
	if (i == NR_TASKS + NR_PROCS) /* no free slot */
		return -1;
	assert(i < NR_TASKS + NR_PROCS);

	//将父进程的进程表复制给子进程
	int pid = mm_msg.source;
	u16 child_ldt_sel = p->ldt_sel;
	*p = proc_table[pid];
	p->ldt_sel = child_ldt_sel;
	p->p_parent = pid;
	sprintf(p->name, "%s_%d", proc_table[pid].name, child_pid);

	//开始复制进程内容
	struct descriptor * ppd;

	//代码部分
	ppd = &proc_table[pid].ldts[INDEX_LDT_C];
	//基地址
	int caller_T_base  = reassembly(ppd->base_high, 24,
					ppd->base_mid,  16,
					ppd->base_low);
	//代码最大限度
	int caller_T_limit = reassembly(0, 0,
					(ppd->limit_high_attr2 & 0xF), 16,
					ppd->limit_low);
	//代码长度
	int caller_T_size  = ((caller_T_limit + 1) *
			      ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?
			       4096 : 1));

	//数据和堆栈的复制
	ppd = &proc_table[pid].ldts[INDEX_LDT_RW];

	int caller_D_S_base  = reassembly(ppd->base_high, 24,
					  ppd->base_mid,  16,
					  ppd->base_low);
	// 数据和堆栈的限度
	int caller_D_S_limit = reassembly((ppd->limit_high_attr2 & 0xF), 16,
					  0, 0,
					  ppd->limit_low);

	int caller_D_S_size  = ((caller_T_limit + 1) *
				((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?
				 4096 : 1));

	//一起检测
	assert((caller_T_base  == caller_D_S_base ) &&
	       (caller_T_limit == caller_D_S_limit) &&
	       (caller_T_size  == caller_D_S_size ));

    //定义子进程的基址，代码段、数据段和堆栈分享，所以只要分配一次即可
	int child_base = alloc_mem(child_pid, caller_T_size);


	phys_copy((void*)child_base, (void*)caller_T_base, caller_T_size);

	//子进程的LDT更新，由于内存空间不足
	init_desc(&p->ldts[INDEX_LDT_C],
		  child_base,
		  (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT,
		  DA_LIMIT_4K | DA_32 | DA_C | PRIVILEGE_USER << 5);
	init_desc(&p->ldts[INDEX_LDT_RW],
		  child_base,
		  (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT,
		  DA_LIMIT_4K | DA_32 | DA_DRW | PRIVILEGE_USER << 5);

    //告诉FS，父子进程间共享文件
	MESSAGE msg2fs;
	msg2fs.type = FORK;
	msg2fs.PID = child_pid;
	send_recv(BOTH, TASK_FS, &msg2fs);

	//将子进程的PID返回给父进程
	mm_msg.PID = child_pid;

	//给子进程发消息，解除阻塞
	MESSAGE m;
	m.type = SYSCALL_RET;
	m.RETVAL = 0;
	m.PID = 0;
	send_recv(SEND, child_pid, &m);

	return 0;
}


//注意防止僵尸进程的出现
PUBLIC void do_exit(int status)
{
	int i;
	int pid = mm_msg.source;
	int parent_pid = proc_table[pid].p_parent;
	struct proc * p = &proc_table[pid];

	//告诉文件系统此消息
	MESSAGE msg2fs;
	msg2fs.type = EXIT;
	msg2fs.PID = pid;
	send_recv(BOTH, TASK_FS, &msg2fs);
    //释放此进程的内存
	free_mem(pid);
    //将其状态设置为传过来的参数，表示父进程的状态
	p->exit_status = status;
    //如果父进程处在waiting状态，则清楚p的waiting位，给p发送信息解除阻塞，并释放proc_table[]表项
	if (proc_table[parent_pid].p_flags & WAITING) {
		proc_table[parent_pid].p_flags &= ~WAITING;
		cleanup(&proc_table[pid]);
	}
	else { //若父进程不在waiting状态，中将子进程挂起，防止出错
		proc_table[pid].p_flags |= HANGING;
	}

    //如果发现此进程有子进程，则将子进程的父进程改成Init进程，
    //再判断INIT进程是否处于waiting状态和子进程是不是出于挂起状态，若是则清空Init的waiting，并发送非阻塞信息，删除子进程进程表

	for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
		if (proc_table[i].p_parent == pid) {
			proc_table[i].p_parent = INIT;
			if ((proc_table[INIT].p_flags & WAITING) &&
			    (proc_table[i].p_flags & HANGING)) {
				proc_table[INIT].p_flags &= ~WAITING;
				cleanup(&proc_table[i]);
			}
		}
	}
}

//将进程完全清理；
//参数 proc表示将要清理的进程
PRIVATE void cleanup(struct proc * proc)
{
    //将信息发给父进程
	MESSAGE msg2parent;
	msg2parent.type = SYSCALL_RET;
	msg2parent.PID = proc2pid(proc);
	msg2parent.STATUS = proc->exit_status;
	send_recv(SEND, proc->p_parent, &msg2parent);
    //进程的进程表清空标志
	proc->p_flags = FREE_SLOT;
}


PUBLIC void do_wait()
{
	int pid = mm_msg.source;

	int i;
	int children = 0;
	struct proc* p_proc = proc_table;
	//遍历proc_table[]，如果发现A是P的子进程，且正在hanging，则向p发消息接触阻塞，并释放A的进程表项
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) {
		if (p_proc->p_parent == pid) {
			children++;
			if (p_proc->p_flags & HANGING) {
				cleanup(p_proc);
				return;
			}
		}
	}
    //如果没有子进程在hanging，则设置waiting位
	if (children) {
		proc_table[pid].p_flags |= WAITING;
	}
	else {//若没有子进程则向此进程发送消息携带错误信息
		MESSAGE msg;
		msg.type = SYSCALL_RET;
		msg.PID = NO_TASK;
		send_recv(SEND, pid, &msg);
	}
}
