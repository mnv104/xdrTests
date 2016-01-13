#include <stdio.h>
#include <stdlib.h>

#include "test.h"

/* Print, write and free request. */
static void write_request (const char *filename, request *);
/* Read, print and free request. */
static void read_request (const char *filename);
/* Print, write and free reply. */
static void write_reply (const char *filename, reply *);
/* Read, print and free reply. */
static void read_reply (const char *filename);

int
main ()
{
  request rq;
  reply rp;

  /* Requests. */

  rq.strings = NULL;

  write_request ("test2-rq1.out", &rq);
  read_request ("test2-rq1.out");

  rq.strings = malloc (sizeof (struct stringentry));
  rq.strings->item = strdup ("string1");
  rq.strings->next = NULL;

  write_request ("test2-rq2.out", &rq);
  read_request ("test2-rq2.out");

  rq.strings = malloc (sizeof (struct stringentry));
  rq.strings->item = strdup ("string1");
  rq.strings->next = malloc (sizeof (struct stringentry));
  rq.strings->next->item = strdup ("string2");
  rq.strings->next->next = NULL;

  write_request ("test2-rq3.out", &rq);
  read_request ("test2-rq3.out");

  rq.strings = malloc (sizeof (struct stringentry));
  rq.strings->item = strdup ("string1");
  rq.strings->next = malloc (sizeof (struct stringentry));
  rq.strings->next->item = strdup ("string2");
  rq.strings->next->next = malloc (sizeof (struct stringentry));
  rq.strings->next->next->item = strdup ("string3");
  rq.strings->next->next->next = NULL;

  write_request ("test2-rq4.out", &rq);
  read_request ("test2-rq4.out");

  /* Replies. */

  rp.ok = TRUE;
  rp.reply_u.strings = NULL;

  write_reply ("test2-rp1.out", &rp);
  read_reply ("test2-rp1.out");

  rp.ok = TRUE;
  rp.reply_u.strings = malloc (sizeof (struct stringentry));
  rp.reply_u.strings->item = strdup ("string1");
  rp.reply_u.strings->next = malloc (sizeof (struct stringentry));
  rp.reply_u.strings->next->item = strdup ("string2");
  rp.reply_u.strings->next->next = malloc (sizeof (struct stringentry));
  rp.reply_u.strings->next->next->item = strdup ("string3");
  rp.reply_u.strings->next->next->next = NULL;

  write_reply ("test2-rp2.out", &rp);
  read_reply ("test2-rp2.out");

  rp.ok = FALSE;
  rp.reply_u.err.code = 0;

  write_reply ("test2-rp3.out", &rp);
  read_reply ("test2-rp3.out");

  rp.ok = FALSE;
  rp.reply_u.err.code = 1;
  rp.reply_u.err.error_u.message = strdup ("errorcode1");

  write_reply ("test2-rp4.out", &rp);
  read_reply ("test2-rp4.out");

  rp.ok = FALSE;
  rp.reply_u.err.code = 2;
  rp.reply_u.err.error_u.error2.subcode = 0x123456789abcdefLL;
  rp.reply_u.err.error_u.error2.message = strdup ("errorcode2");

  write_reply ("test2-rp5.out", &rp);
  read_reply ("test2-rp5.out");

  return 0;
}

static void
print_request (const request *rq)
{
  stringentry *entry;

  printf ("request = [");

  for (entry = rq->strings; entry; entry = entry->next)
    printf (" \"%s\"", entry->item);

  printf (" ]\n");
}

static void
write_request (const char *filename, request *rq)
{
  XDR xdr;
  FILE *fp;

  fp = fopen (filename, "w");
  if (!fp) { perror (filename); exit (1); }
  xdrstdio_create (&xdr, fp, XDR_ENCODE);

  if (!xdr_request (&xdr, rq)) {
    fprintf (stderr, "write_request: could not encode\n");
    exit (1);
  }

  xdr_free ((xdrproc_t) xdr_request, (char *) rq);
  xdr_destroy (&xdr);
  fclose (fp);
}

static void
read_request (const char *filename)
{
  XDR xdr;
  FILE *fp;
  request rq = { 0 };

  fp = fopen (filename, "r");
  if (!fp) { perror (filename); exit (1); }
  xdrstdio_create (&xdr, fp, XDR_DECODE);

  if (!xdr_request (&xdr, &rq)) {
    fprintf (stderr, "read_request: could not decode\n");
    exit (1);
  }

  fclose (fp);

  print_request (&rq);

  xdr_free ((xdrproc_t) xdr_request, (char *) &rq);
  xdr_destroy (&xdr);
}

static void
print_reply (const reply *rp)
{
  printf ("reply =");

  if (rp->ok) {
    stringentry *entry;

    printf (" ok [");
    for (entry = rp->reply_u.strings; entry; entry = entry->next)
      printf (" \"%s\"", entry->item);
    printf (" ]\n");
  } else {
    printf (" error %d", rp->reply_u.err.code);
    switch (rp->reply_u.err.code) {
    case 0:
      break;
    case 1:
      printf (" \"%s\"", rp->reply_u.err.error_u.message);
      break;
    case 2:
      printf (" %lx \"%s\"",
	      rp->reply_u.err.error_u.error2.subcode,
	      rp->reply_u.err.error_u.error2.message);
      break;
    }
    printf ("\n");
  }
}

static void
write_reply (const char *filename, reply *rp)
{
  XDR xdr;
  FILE *fp;

  fp = fopen (filename, "w");
  if (!fp) { perror (filename); exit (1); }
  xdrstdio_create (&xdr, fp, XDR_ENCODE);

  if (!xdr_reply (&xdr, rp)) {
    fprintf (stderr, "write_reply: could not encode\n");
    exit (1);
  }

  xdr_free ((xdrproc_t) xdr_reply, (char *) rp);
  xdr_destroy (&xdr);
  fclose (fp);
}

static void
read_reply (const char *filename)
{
  XDR xdr;
  FILE *fp;
  reply rp = { 0 };

  fp = fopen (filename, "r");
  if (!fp) { perror (filename); exit (1); }
  xdrstdio_create (&xdr, fp, XDR_DECODE);

  if (!xdr_reply (&xdr, &rp)) {
    fprintf (stderr, "read_reply: could not decode\n");
    exit (1);
  }

  fclose (fp);

  print_reply (&rp);

  xdr_free ((xdrproc_t) xdr_reply, (char *) &rp);
  xdr_destroy (&xdr);
}
