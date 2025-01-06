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
#ifndef KC3_SMTP_H
#define KC3_SMTP_H

#include "types.h"

/* Heap-allocation functions, call kc3_smtp_close after use. */
sw        kc3_smtp_close (u_ptr_w *smtp);
u_ptr_w * kc3_smtp_open (const s_str *server, const s_str *port,
                         const s_sym * const *security,
                         const s_sym * const *flags,
                         const s_str *cafile, u_ptr_w *dest);

/* Operators. */

#endif /* SMTP_H */
