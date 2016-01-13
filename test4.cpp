#include <stdio.h>
#include <values.h>
#include <limits.h>
#include <stdlib.h>

#include "test.h"
#include "nfs.h"
#include "util.hpp"


class Foo {
public:
   XDR xdr;
   Buffer<char> buffer;
   Foo(int size) : buffer(size) {
      xdrmem_create(&xdr, buffer.buffer,
                    buffer.size(), XDR_ENCODE);
   }
   ~Foo() {
      xdr_destroy(&xdr);
   }
   void encode(xdrproc_t xdrArgs, caddr_t args) {
      if (!xdrArgs(&xdr, args)) {
         return;
      }
   }
   int getSize() {
      return xdr_getpos(&xdr);
   }
};

int main()
{
   Foo foo(512);
   FSINFO3args args;
   memset(&args, 0, sizeof args);
   char rootHandle[28];
   args.fsroot.data.data_val = (char*) rootHandle;
   args.fsroot.data.data_len = sizeof(rootHandle);
   foo.encode((xdrproc_t) xdr_FSINFO3args, (caddr_t) &args);
   printf("%d\n", foo.getSize());
   return 0;
}
