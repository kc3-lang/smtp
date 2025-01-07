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

bool kc3_smtp_address_add (u_ptr_w *smtp, const s_sym * const *type,
                           const s_str *email, const s_str *name)
{
  enum smtp_address_type address_type;
  sw r;
  assert(smtp);
  assert(smtp->p);
  if (*type == sym_1("from"))
    address_type = SMTP_ADDRESS_FROM;
  else if (*type == sym_1("to"))
    address_type = SMTP_ADDRESS_TO;
  else if (*type == sym_1("cc"))
    address_type = SMTP_ADDRESS_CC;
  else if (*type == sym_1("bcc"))
    address_type = SMTP_ADDRESS_BCC;
  else {
    err_puts("kc3_smtp_address_add: invalid address type");
    assert(! "kc3_smtp_address_add: invalid address type");
    return false;
  }
  if ((r = smtp_address_add(smtp->p, address_type, email->ptr.pchar,
                            name->ptr.pchar))) {
    err_write_1("kc3_smtp_address_add: smtp_address_add: ");
    err_inspect_sw_decimal(&r);
    err_write_1("\n");
    assert("kc3_smtp_address_add: smtp_address_add");
    return false;
  }
  return true;
}

bool kc3_smtp_close (u_ptr_w *smtp)
{
  sw r;
  assert(smtp);
  assert(smtp->p);
  if ((r = smtp_close(smtp->p))) {
    err_write_1("kc3_smtp_close: smtp_close: ");
    err_inspect_sw_decimal(&r);
    err_write_1("\n");
    assert("kc3_smtp_close: smtp_close");
    return false;
  }
  return true;
}

bool kc3_smtp_header_add (u_ptr_w *smtp, const s_str *name,
                          const s_str *value)
{
  sw r;
  assert(smtp);
  assert(smtp->p);
  if ((r = smtp_header_add(smtp->p, name->ptr.pchar,
                           value->ptr.pchar))) {
    err_puts("kc3_smtp_header_add: smtp_header_add");
    assert(! "kc3_smtp_header_add: smtp_header_add");
    return false;
  }
  return true;
}

bool kc3_smtp_mail (u_ptr_w *smtp, const s_str *body)
{
  sw r;
  if ((r = smtp_mail(smtp->p, body->ptr.pchar))) {
    err_puts("kc3_smtp_mail: smtp_mail");
    assert(! "kc3_smtp_mail: smtp_mail");
    return false;
  }
  return true;
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
