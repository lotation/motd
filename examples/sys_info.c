#include <stdio.h>
#include <sys/utsname.h>

int main(void) {
  struct utsname info;
  uname(&info);

  printf("%s\n", info.sysname);
  printf("%s\n", info.nodename);
  printf("%s\n", info.release);
  printf("%s\n", info.version);
  printf("%s\n", info.machine);

  return 0;
}
