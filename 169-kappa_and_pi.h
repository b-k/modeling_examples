#include <apop.h>

/* Takes in a confusion matrix; returns κ, π, and P_I.

It is up to the user to guarantee that the data in column one represents the same
category as the data in row one, and so on for cat(col 2)==cat(row 2), and so on.

Returns an apop_data set with these rows:

"total count"
"percent agreement"
"Cohen's p_e"
"Scott's p_e"
"κ"
"π"

Use, e.g., apop_data_get(outdata, .rowname="Cohen's p_e") to get one value out of the set.

A blank data set with out->error set is returned if:

--The input is NULL (out->error == 'n')
--The input matrix is NULL (out->error == 'm')
--The input matrix row count != the input matrix column count (out->error == 's')
*/
apop_data *kappa_and_pi(apop_data const *tab_in);
