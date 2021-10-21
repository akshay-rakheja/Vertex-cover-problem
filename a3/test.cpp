#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
   fork();
   fork();
   fork();
   printf("ECE 650\n");
   return 0;
}