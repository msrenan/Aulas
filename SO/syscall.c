#include <stdio.h>
#include <stdbool.h>

int main() {

  printf("Hello, World!\n");  
  
  int x = 10;
  bool flag = false;

  {
  	int x = 15;

    if (x > 10) {
        flag = true;
    }
	
  }

  printf("%d\n", x);
  printf("Flag: %d\n", flag);

}
