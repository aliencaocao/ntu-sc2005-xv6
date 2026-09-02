#include "kernel/types.h"
#include "user/user.h"

// Every xv6 user program starts execution in main().
int
main(void)
{
  // This calls the user-space syscall stub generated from user/usys.pl.
  // The stub enters the kernel, where sys_getproccount() does the counting.
  int count = getproccount();

  // printf() runs in user space and displays the value returned by the kernel.
  printf("Number of processes: %d\n", count);

  // An xv6 program must call exit() when it has finished.
  exit(0);
}
