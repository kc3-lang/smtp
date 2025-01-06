/* kc3
 * Copyright 2022,2023,2024 kmx.io <contact@kmx.io>
 *
 * Permission is hereby granted to use this software granted the above
 * copyright notice and this permission paragraph are included in all
 * copies and substantial portions of this software.
 *
 * THIS SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY GUARANTEE OF
 * PURPOSE AND PERFORMANCE. IN NO EVENT WHATSOEVER SHALL THE
 * AUTHOR BE CONSIDERED LIABLE FOR THE USE AND PERFORMANCE OF
 * THIS SOFTWARE.
 */
#include <libkc3/kc3.h>
#include "kc3_smtp.h"
#include "smtp.h"

sw kc3_smtp_close (u_ptr_w *smtp)
{
  return smtp_close(smtp->p);
}

u_ptr_w * kc3_smtp_open (const s_str *server, const s_str *port,
                         const s_sym * const *security,
                         const s_sym * const *flags,
                         const s_str *cafile, u_ptr_w *dest)
{
  const char *ca = NULL;
  enum smtp_flag f = 0;
  sw r;
  enum smtp_connection_security sec = 0;
  u_ptr_w tmp;
  if (*security == sym_1("none"))
    sec = SMTP_SECURITY_NONE;
#ifdef SMTP_OPENSSL
  else if (*security == sym_1("starttls"))
    sec = SMTP_SECURITY_STARTTLS;
  else if (*security == sym_1("tls"))
    sec = SMTP_SECURITY_TLS;
#endif
  else {
    err_puts("kc3_smtp_open: invalid argument: security");
    assert(! "kc3_smtp_open: invalid argument: security");
    return NULL;
  }
  if (*flags == sym_1("none"))
    f = 0;
  else if (*flags == sym_1("debug"))
    f = SMTP_DEBUG;
  else {
    err_puts("kc3_smtp_open: invalid flag");
    assert(! "kc3_smtp_open: invalid flag");
    return NULL;
  }
  if (cafile->size)
    ca = cafile->ptr.pchar;
  r = smtp_open(server->ptr.pchar, port->ptr.pchar, sec, f, ca,
                (struct smtp **) &tmp.p);
  switch (r) {
  case SMTP_STATUS_OK:
    *dest = tmp;
    return dest;
  default:
    break;
  }
  err_puts("kc3_smtp_open: smtp_open");
  assert(! "kc3_smtp_open: smtp_open");
  return NULL;
}
