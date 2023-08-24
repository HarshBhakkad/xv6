#include "user.h"

#include "stat.h"

#include "types.h"

int 
main(void)
{
	static char buf[20000];
	printf(1,"messi system call return %d\n",messi((void*) buf,20000));
	printf(1,"%s",buf);
	exit();
}