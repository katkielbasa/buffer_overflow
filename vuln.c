#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  char buffer[500];
  strcpy(buffer, argv[1]);

  printf("The first cmd-line argument is: %s\n", buffer);
  return 0;
}
