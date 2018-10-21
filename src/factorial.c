
#include "mathx/core/factorial.h"

/*@
 * \fn void nbr_factorial(mx_int_t *fn, const mx_int_t *n)
 * \brief Computes the factorial of a number.
 * \param variable to receive the result
 * \param input
 *
 * If \c n is negative, this function assigns zero to \c fn.
 */
void nbr_factorial(mx_int_t *fn, const mx_int_t *n)
{
  mx_int_t i;
  mx_int_t temp;

  if (n->size < 0)
  {
    nbr_assign_zero(fn);
    return;
  }

  nbr_copy_init(&i, n);
  nbr_init(&temp);

  nbr_limb_assign(fn, 1);

  while (i.size != 0)
  {
    nbr_mul(&temp, fn, &i);

    nbr_swap(fn, &temp);

    nbr_limb_abs_decr(&i, 1);
  }

  nbr_clear(&temp);
  nbr_clear(&i);
}