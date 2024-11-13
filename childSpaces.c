#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_extra_spaces(char* str) {
   int i, x = 0;
   for (i = 0; str[i]; i++) {
      if (!(str[i] == ' ' && str[i + 1] == ' ')) {
         str[x++] = str[i];
      }
   }
   str[x] = '\0';
}

int main() {
   char buffer[1024];

   if (fgets(buffer, sizeof(buffer), stdin)) {
      remove_extra_spaces(buffer);
      printf("%s", buffer);
      fflush(stdout);
   }

   return 0;
}