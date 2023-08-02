/* BTK Akademi - C Programlama Dili Kursu */
/**
 * C programlama dilinde değişkenlere değer atama
*/

#include <stdio.h>

int main() {
  int x = 5, y = 6, z = 50;
  printf("x = %d\n", x);  
  printf("y = %d\n", y);
  printf("z = %d\n", z);    
  printf("x = %d\t y = %d\t z = %d\n", x, y, z);    
  printf("%d", x + y + z);
  printf("\n...\n");
  
  x = y = z = 50;
  printf("x = %d\n", x);  
  printf("y = %d\n", y);
  printf("z = %d\n", z);    
  printf("%d", x + y + z);
  
  return 0;
}