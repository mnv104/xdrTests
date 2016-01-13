#include <stdio.h>
#include <values.h>
#include <limits.h>
#include <stdlib.h>

#include "test.h"
#include "nfs.h"

int main()
{
   XDR xdr;
   char buffer[512];
   FSINFO3args args;
   FSINFO3res  res;
   memset(&args, 0, sizeof args);
   memset(&res, 0, sizeof res);
   char rootHandle[28];
   args.fsroot.data.data_val = (char*) rootHandle;
   args.fsroot.data.data_len = sizeof(rootHandle);
   xdrmem_create(&xdr, buffer, sizeof(buffer), XDR_ENCODE);
   xdr_FSINFO3args(&xdr, &args);
   printf("%d\n", xdr_getpos(&xdr));
   xdr_destroy(&xdr);
   return 0;
}
