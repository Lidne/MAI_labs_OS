#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void to_uppercase(char* str) {
   for (int i = 0; str[i]; i++) {
      str[i] = toupper(str[i]);
   }
}

int main() {
   char buffer[1024];

   while (fgets(buffer, 1024, stdin)) {
      to_uppercase(buffer);
      printf("%s", buffer);
      fflush(stdout);
   }

   return 0;
}