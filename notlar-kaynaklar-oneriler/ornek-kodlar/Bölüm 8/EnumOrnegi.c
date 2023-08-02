/* BTK Akademi - C Programlama Dili Kursu */
/**
 * C programlama dilinde enumerated type'lar
 * yani numaralandırılmış tipler
*/

#include <stdio.h>

enum Seviyeler {
  DUSUK,
  ORTA,
  YUKSEK
};
  
int main() {
  // numaralandırılmış tipte bir değişken tanımla
  // ve değer ata
  enum Seviyeler OdaSicakligi = DUSUK;
 
  // numaralandırılmış tipteki değişkeni yazdır
  printf("%d", OdaSicakligi);
  
  return 0;
}