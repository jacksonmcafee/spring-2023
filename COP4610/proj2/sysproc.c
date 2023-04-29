#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "uproc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// do I need to redefine ptable? it's defined in proc.c already...
struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;
 
int sys_getprocs(void) {
    // define variable max to store max entries & struct uproc
    int max;
    struct uproc *uproc;

    // if argint runs into an error, return -1
    // pass val of max to function
    if (argint(0,&max) < 0) { return -1; }

    // if argptr runs into an error, return -2
    // cast to char 2x ptr because argptr yelled at me to
    // pass ptr to uproc into function
    if (argptr(1, (char**)&uproc, max*sizeof(struct uproc)) < 0) { return -2; }

    // get ptable proc
    struct proc *proc = ptable.proc;

    // define counter i and set to zero
    int i = 0;

    // iterate through every process in ptable
    // check if they are unused, ignore if so
    // if being used, save those to uproc megastruct 
    // save pid, ppid, and name
    for (; proc < &ptable.proc[NPROC]; proc++) {
        if (proc->state == UNUSED) {
            // skip this process entry because it is unused
            continue;
        } else {
            // this process is being used, write pid and ppid to uproc
            uproc[i].pid = proc->pid;
            uproc[i].ppid = proc->parent->pid;
            
            // use strncpy to copy the name over b/c .name is ptr
            strncpy(uproc[i].name, proc->name, 16);
            
            // increment i to add to the process count
            i++;
        }
    }
    // return the number of processes
    return i;
}

