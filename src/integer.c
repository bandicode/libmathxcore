
#include "mathx/core/integer.h"

#include "mathx/core/memory.h"
#include "mathx/core/print.h"

#include <stdlib.h> // abs
#include <string.h>

/*@
 * \class mx_integer_struct
 * \brief Represents an arbitrary-precision signed integer.
 *
 * \endclass
 */


/*@
 * \fn void nbr_init(mx_int_t *x)
 * \brief Zero-initialize an integer.
 * \param the integer to initialize
 */
void nbr_init(mx_int_t *x)
{
  x->size = 0;
  x->alloc = 0;
  x->limbs = NULL;
}

/*@
 * \fn void nbr_copy_init(mx_int_t *x, const mx_int_t *src)
 * \brief Performs copy-initialization of an integer.
 * \param the integer to initialize
 * \param integer to copy
 */
void nbr_copy_init(mx_int_t *x, const mx_int_t *src)
{
  x->size = src->size;
  x->alloc = src->alloc;

  if (x->alloc)
  {
    x->limbs = mx_malloc(x->alloc, NULL);
    memcpy(x->limbs, src->limbs, x->alloc * sizeof(mx_limb_t));
  }
  else
  {
    x->limbs = NULL;
  }
}

/*@
 * \fn void nbr_limb_init(mx_int_t *x, const mx_limb_t value)
 * \brief Initializes an integer from a single limb.
 * \param the integer to initialize
 * \param the limb's value
 */
void nbr_limb_init(mx_int_t *x, const mx_limb_t value)
{
  if (value == 0)
  {
    x->alloc = 0;
    x->size = 0;
    x->limbs = NULL;
  }
  else
  {
    x->size = 1;
    x->limbs = mx_malloc(1, &(x->alloc));
    x->limbs[0] = value;
    memset(x->limbs + 1, 0, (x->alloc - 1) * sizeof(mx_limb_t));
  }
}

/*@
 * \fn void nbr_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc)
 * \brief Initializes an integer given all its internal data members.
 * \param the integer to initialize
 * \param the integer signed size
 * \param pointer to the first limb
 * \param number of allocated limbs
 *
 * This function assumes that allocated limbs that are not part of the integer are 
 * set to zero, i.e. limbs[i] is zero for all i in [abs(size), alloc).
 */
void nbr_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc)
{
  x->alloc = alloc;
  x->size = size;
  x->limbs = limbs;
}

/*@
 * \fn void nbr_string_init(mx_int_t *x, const char *str)
 * \brief Initializes an integer from a string.
 * \param the integer to initialize
 * \param null-terminated char string
 *
 */
void nbr_string_init(mx_int_t *x, const char *str)
{
#if LIBMATHXCORE_LIMB_SIZE == 8
#define DECIMAL_BASE 100
#define DECIMAL_SHIFT 2
#elif LIBMATHXCORE_LIMB_SIZE == 16
#define DECIMAL_BASE 10000
#define DECIMAL_SHIFT 4
#else
#define DECIMAL_BASE 1000000000
#define DECIMAL_SHIFT 9
#endif

  nbr_init(x);

  mx_int_t base;
  nbr_limb_init(&base, DECIMAL_BASE);

  mx_int_t buffer;
  nbr_init(&buffer);

  mx_ssize_t sign = 1;
  if (str[0] == '-')
  {
    sign = -1;
    str++;
  }
  else if (str[0] == '+')
  {
    str++;
  }

  while (*str != '\0')
  {
    mx_limb_t limb = 0;
    base.limbs[0] = 1;
    while (*str != '\0' && base.limbs[0] < DECIMAL_BASE)
    {
      limb = limb * 10 + (*str - '0');
      ++str;
      base.limbs[0] *= 10;
    }

    nbr_mul(&buffer, x, &base);
    nbr_swap(&buffer, x);
    nbr_limb_abs_incr(x, limb);
  }

  x->size *= sign;

  nbr_clear(&base);
  nbr_clear(&buffer);

#undef DECIMAL_BASE
#undef DECIMAL_SHIFT
}

/*@
 * \fn void nbr_clear(mx_int_t *x)
 * \brief Releases any memory used by an arbitrary-precision integer.
 */
void nbr_clear(mx_int_t *x)
{
  mx_free(x->limbs);
  x->size = 0;
  x->alloc = 0;
}

/*@
 * \fn int nbr_is_normalized(mx_int_t *x)
 * \brief Returns whether the input is in its normalized form.
 */
int nbr_is_normalized(const mx_int_t *x)
{
  const mx_size_t s = abs(x->size);
  mx_size_t i = s;

  for (; i < x->alloc; ++i)
  {
    if (x->limbs[i] != 0)
      return 0;
  }

  if (s != 0 && x->limbs[s - 1] == 0)
    return 0;

  return 1;
}

/*@
 * \fn void nbr_normalize(mx_int_t *x)
 * \brief Normalizes its input.
 */
void nbr_normalize(mx_int_t *x)
{
  mx_size_t s = abs(x->size);
  memset(x->limbs + s, 0, (x->alloc - s) * sizeof(mx_limb_t));
  while (s > 0 && x->limbs[s - 1] == 0) --s;
  x->size = s * (x->size < 0 ? -1 : 1);
}

/*@
 * \fn void nbr_ensure_alloc(mx_int_t *x, mx_size_t s)
 * \brief Ensures enough limbs are allocated.
 * \param pointer to integer object
 * \param number of limbs that needs to be allocated
 *
 * Ensures \c x can be manipulated as if there were exactly \c s limbs allocated.
 * In other words, this function will allocate new limbs if not enough are already 
 * allocated, and/or will memset to zero limbs with index \c{>= s}.
 *
 * The size field of \c x is left unchanged, so calling this function might 
 * result in \c x being left in a non-normalized form.
 *
 * This function is mainly for internal purpose.
 */
void nbr_ensure_alloc(mx_int_t *x, mx_size_t s)
{
  if (x->alloc < s)
  {
    mx_free(x->limbs);
    x->limbs = mx_malloc(s, &(x->alloc));
    memset(x->limbs + s, 0, (x->alloc - s) * sizeof(mx_limb_t));
  }
  else
  {
    if (s < (mx_size_t) abs(x->size))
    {
      memset(x->limbs + s, 0, (abs(x->size) - s) * sizeof(mx_limb_t));
    }
  }
}

/*@
 * \fn void nbr_ensure_alloc_zero(mx_int_t *x, mx_size_t s)
 * \brief Ensures enough limbs are allocated and set to zero.
 * \param pointer to integer object
 * \param number of limbs that needs to be allocated
 *
 * Ensures \c x can be manipulated as if there were exactly \c s zero-initialized limbs allocated.
 *
 * The size field of \c x is left unchanged, so calling this function might
 * result in \c x being left in a non-normalized form.
 *
 * This function is mainly for internal purpose.
 */
void nbr_ensure_alloc_zero(mx_int_t *x, mx_size_t s)
{
  if (x->alloc < s)
  {
    mx_free(x->limbs);
    x->limbs = mx_malloc_zero(s, &(x->alloc));
  }
  else
  {
    memset(x->limbs, 0, abs(x->size) * sizeof(mx_limb_t));
  }
}


/*@
 * \fn void nbr_assign(mx_int_t *dest, const mx_int_t *src)
 * \brief Performs integer assignment.
 * \param receiver
 * \param value to assign
 */
void nbr_assign(mx_int_t *dest, const mx_int_t *src)
{
  const mx_size_t srcsize = abs(src->size);

  if (dest->alloc < srcsize)
  {
    mx_free(dest->limbs);
    dest->limbs = mx_malloc(srcsize, &(dest->alloc));
    memset(dest->limbs + srcsize, 0, (dest->alloc - srcsize) * sizeof(mx_limb_t));
  }
  else if(srcsize < (mx_size_t) abs(dest->size))
  {
    memset(dest->limbs + srcsize, 0, (abs(dest->size) - srcsize) * sizeof(mx_limb_t));
  }

  dest->size = src->size;
  memcpy(dest->limbs, src->limbs, srcsize * sizeof(mx_limb_t));
}

/*@
 * \fn void nbr_assign_zero(mx_int_t *dest)
 * \brief Assigns the value 0 to an integer
 * \param receiver
 */
void nbr_assign_zero(mx_int_t *dest)
{
  memset(dest->limbs, 0, abs(dest->size) * sizeof(mx_limb_t));
  dest->size = 0;
}

void nbr_limb_assign(mx_int_t *dest, const mx_limb_t limb)
{
  if (limb == 0)
  {
    nbr_assign_zero(dest);
  }
  else
  {
    nbr_ensure_alloc(dest, 1);
    dest->limbs[0] = limb;
    dest->size = 1;
  }
}

/*@
 * \fn void nbr_swap(mx_int_t *a, mx_int_t *b)
 * \brief Swaps two integers
 * \param first value
 * \param second value
 */
void nbr_swap(mx_int_t *a, mx_int_t *b)
{
  const mx_int_t c = *b;

  *b = *a;
  *a = c;
}

/*@
 * \fn int nbr_sign(const mx_int_t *x)
 * \brief Returns the sign of an integer.
 * \param input value
 * \returns -1, 0 or 1
 */
int nbr_sign(const mx_int_t *x)
{
  return x->size == 0 ? 0 : (x->size < 0 ? -1 : 1);
}


/*@
 * \fn mx_size_t nbr_print(char *out, mx_size_t s, const mx_int_t *x)
 * \brief Writes the decimal representation of an integer
 * \param pointer to output
 * \param size of the output buffer
 * \param integer to print
 * \returns number of char written (excluding null-terminator)
 *
 * This function starts by computing an upper bound of the number of char 
 * that will be written. If the buffer is smaller than this upper bound, 
 * no char are written and this function returns zero. 
 */
mx_size_t nbr_print(char *out, mx_size_t s, const mx_int_t *x)
{
  if (s < 2)
    return 0;

  if (x->size == 0)
  {
    out[0] = '0';
    out[1] = '\0';
    return 1;
  }

  // upper_bound = sign + #digits + null-terminator
  const mx_size_t upper_bound = (x->size < 0 ? 1 : 0) + unbr_print_size(x->limbs, abs(x->size)) + 1;
  if (s < upper_bound)
    return 0;
  
  mx_size_t written = 0;

  if (x->size < 0)
  {
    *(out++) = '-';
    written += 1;
  }

  written += unbr_print(x->limbs, abs(x->size), out);

  return written;
}