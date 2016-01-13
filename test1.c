#include <stdio.h>
#include <stdlib.h>

#include "test.h"

int
main ()
{
  XDR xdr;
  stringlist strings;
  stringentry *entry;
  FILE *fp;

  fp = fopen ("test1.out", "w");
  xdrstdio_create (&xdr, fp, XDR_ENCODE);

  strings = malloc (sizeof (struct stringentry));
  strings->item = strdup ("hello");
  strings->next = malloc (sizeof (struct stringentry));
  strings->next->item = strdup ("goodbye");
  strings->next->next = NULL;

  if (!xdr_stringlist (&xdr, &strings)) {
    fprintf (stderr, "test1: could not encode\n");
    exit (1);
  }

  xdr_free ((xdrproc_t) xdr_stringlist, (char *) &strings);
  xdr_destroy (&xdr);
  fclose (fp);

  fp = fopen ("test1.out", "r");
  xdrstdio_create (&xdr, fp, XDR_DECODE);

  strings = NULL;
  if (!xdr_stringlist (&xdr, &strings)) {
    fprintf (stderr, "test1: could not decode\n");
    exit (1);
  }

  fclose (fp);

  for (entry = strings; entry; entry = entry->next)
    printf ("entry->item = %s\n", entry->item);

  xdr_free ((xdrproc_t) xdr_stringlist, (char *) &strings);
  xdr_destroy (&xdr);

  exit (0);
}
