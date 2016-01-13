#include <stdio.h>
#include <values.h>
#include <limits.h>
#include <stdlib.h>

#include "test.h"
int main()
{
   XDR xdr;
   int i = 42;
   char b = 'a';
   int c[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   char buffer[80];
   xdrmem_create(&xdr, buffer, sizeof(buffer), XDR_ENCODE);
   xdr_int(&xdr, &i);
   xdr_char(&xdr, &b);
   unsigned int d = sizeof(c)/sizeof(c[0]);
   xdr_vector(&xdr, (char *) c, d, sizeof(int), (xdrproc_t) xdr_int);
   printf("%d\n", xdr_getpos(&xdr));
   xdr_destroy(&xdr);

   xdrmem_create(&xdr, buffer, sizeof(buffer), XDR_DECODE);
   int j;
   xdr_int(&xdr, &j);
   printf("i = %d, j = %d\n", i, j);
   char k;
   xdr_char(&xdr, &k);
   printf("b = %c, k = %c\n", b, k);
   int l[10];
   unsigned int m = sizeof(l)/sizeof(l[0]);
   xdr_vector(&xdr, (char *) l, m, sizeof(int), (xdrproc_t) xdr_int);
   printf("%d\n", xdr_getpos(&xdr));
   xdr_destroy(&xdr);
   return 0;
}
