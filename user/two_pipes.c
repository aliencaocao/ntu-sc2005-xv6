#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  char buf[6];
  int n;
  int pid;

  if(pipe(p1) < 0 || pipe(p2) < 0){
    printf("two_pipes: pipe failed\n");
    exit(1);
  }

  pid = fork();
  if(pid < 0){
    printf("two_pipes: fork failed\n");
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    exit(1);
  }

  if(pid == 0){
    // Child: reads from p1, writes to p2
    close(p1[1]);
    close(p2[0]);

    n = read(p1[0], buf, sizeof(buf)-1);
    if(n != 4){
      printf("%d: child read failed\n", getpid());
      close(p1[0]);
      close(p2[1]);
      exit(1);
    }
    if(buf[0] == 'p' && buf[1] == 'i' && buf[2] == 'n' && buf[3] == 'g'){
      printf("%d: received ping and replies with pong\n", getpid());
    } else {
      printf("%d: unexpected message before pong\n", getpid());
      close(p1[0]);
      close(p2[1]);
      exit(1);
    }

    if(write(p2[1], "pong", 4) != 4){
      printf("%d: child write failed\n", getpid());
      close(p1[0]);
      close(p2[1]);
      exit(1);
    }

    close(p1[0]);
    close(p2[1]);
    exit(0);

  } else {
    // Parent: writes to p1, reads from p2
    close(p1[0]);
    close(p2[1]);

    if(write(p1[1], "ping", 4) != 4){
      printf("%d: parent write failed\n", getpid());
      close(p1[1]);
      close(p2[0]);
      wait(0);
      exit(1);
    }

    n = read(p2[0], buf, sizeof(buf)-1);
    if(n != 4){
      printf("%d: parent read failed\n", getpid());
      close(p1[1]);
      close(p2[0]);
      wait(0);
      exit(1);
    }
    if(buf[0] == 'p' && buf[1] == 'o' && buf[2] == 'n' && buf[3] == 'g'){
      printf("%d: received pong\n", getpid());
    } else {
      printf("%d: unexpected response\n", getpid());
      close(p1[1]);
      close(p2[0]);
      wait(0);
      exit(1);
    }

    close(p1[1]);
    close(p2[0]);
    if(wait(0) < 0){
      exit(1);
    }
    exit(0);
  }
}
