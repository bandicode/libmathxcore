
#include "test-framework.h"
#include "test-framework-float.h"

#include "mathx/core/float/fsqrt.h"
#include "mathx/core/float/pi.h"

#include <stdio.h>
#include <string.h>

Test(FloatFunctions, sqrt2)
{
  DecimalRepresentation expected;
  expected.digits = 
    "1414213562373095048801688724209698078569671875376"
    "9480731766797379907324784621070388503875343276415"
    "7273501384623091229702492483605585073721264412149"
    "7099935831413222665927505592755799950501152782060"
    "5714701095599716059702745345968620147285174186408"
    "8919860955232923048430871432145083976260362799525"
    "1407989687253396546331808829640620615258352395054"
    "7457502877599617298355752203375318570113543746034"
    "0849884716038689997069900481503054402779031645424"
    "7823068492936918621580578463111596668713013015618"
    "5689872372";
  expected.exp = -499;

  mx_float_t two;
  float_init(&two);
  float_assign_limb(&two, 2);

  mx_float_t sqrt_two;
  float_init_prec(&sqrt_two, 128 / sizeof(mx_limb_t));

  float_sqrt(&sqrt_two, &two);

  //printf_float(&sqrt_two);
  //printf_float_dec(&sqrt_two);

  Assert(compare_to_decrepr(&sqrt_two, expected));

  float_clear(&two);
  float_clear(&sqrt_two);
}


Test(FloatFunctions, sqrt3)
{
  DecimalRepresentation expected;
  expected.digits =
    "17320508075688772935274463415058723669428052538103"
    "80628055806979451933016908800037081146186757248575"
    "67562614141540670302996994509499895247881165551209"
    "43736485280932319023055820679748201010846749232650"
    "15312343266903322886650672254668921837971227047131"
    "66036786158801904998653737985938946765034750657605"
    "07566183481296061009476021871903250831458295239598"
    "32997789824508288714463832917347224163984587855397"
    "66795806381835366611084317378089437831610208830552"
    "49016700235207111442886959909563657970871684980729";
  expected.exp = -499;

  mx_float_t three;
  float_init(&three);
  float_assign_limb(&three, 3);

  mx_float_t sqrt_three;
  float_init_prec(&sqrt_three, 128 / sizeof(mx_limb_t));

  float_sqrt(&sqrt_three, &three);

  //printf_float(&sqrt_two);
  //printf_float_dec(&sqrt_three);

  Assert(compare_to_decrepr(&sqrt_three, expected));

  float_clear(&three);
  float_clear(&sqrt_three);
}

Test(FloatFunctions, pi)
{
  DecimalRepresentation expected;
  expected.digits =
    "31415926535897932384626433832795028841971693993751"
    "05820974944592307816406286208998628034825342117067"
    "98214808651328230664709384460955058223172535940812"
    "84811174502841027019385211055596446229489549303819"
    "64428810975665933446128475648233786783165271201909"
    "14564856692346034861045432664821339360726024914127"
    "37245870066063155881748815209209628292540917153643"
    "67892590360011330530548820466521384146951941511609"
    "43305727036575959195309218611738193261179310511854"
    "80744623799627495673518857527248912279381830119491";
  expected.exp = -499;

  mx_float_t pi;
  float_init_prec(&pi, 128 / sizeof(mx_limb_t));

  float_assign_pi(&pi);

  //printf_float(&pi);
  //printf_float_dec(&pi);

  Assert(compare_to_decrepr(&pi, expected));

  float_clear(&pi);
}

TestSuite(FloatFunctions, sqrt2, sqrt3, pi);


void register_floats_functions_test_suite()
{
  register_test(&FloatFunctions);
}
