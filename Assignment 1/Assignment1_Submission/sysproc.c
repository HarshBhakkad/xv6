#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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

int sys_draw(void)
{

  char *buffer;
  int size;

  char *draw="            |.---.|\n\
                          ||___||\n\
                          |+  .'|\n\
                          | _ _ |\n\
    	                  |_____/\n\n";

  // Fetch the 1st 32 bit call argument and assign it to the variable size i.e.the max-size of the buffer
  // Return -1 if an invalid address is accessed
  if (argint(1, &size) == -1)
  {
    return -1;
  }

  // Fetch the 0th word-sized system call argument as a pointer
  // to a block of memory of size bytes.Check that the pointer
  // lies within the process address space if it does not then return -1.
  if (argptr(0, (char **)&buffer, size) == -1)
  {
    return -1;
  }

  //Find the size of the  picture;
  int drawsize = 0;
  while (draw[drawsize] != '\0')
  {
    drawsize++;
  }

  if (drawsize > size)
  {
    //If the size of picture is greater than max size of the buffer return -1
    return -1;
  }

  //copy the picture to the buffer
  for (int i = 0; i < drawsize; i++)
  {
    buffer[i] = draw[i];
  }
  //return the size of  pictue
  return drawsize;
}

int sys_thread_create(void) {
	void (*fcn)(void*), *arg, *stack;
	argptr(0, (void*) &fcn, sizeof(void (*)(void *)));
	argptr(1, (void*) &arg, sizeof(void *));
	argptr(2, (void*) &stack, sizeof(void *));
	return thread_create(fcn, arg, stack);
}

// sys_join
int sys_thread_join(void) {
	return thread_join();
}

int sys_thread_exit(void) {
	return thread_exit();
}