#include <apop.h>

int main(){
    apop_data *d = apop_data_alloc(2,2);
    apop_data_fill(d,
            11, 14,
            24, 35);

    apop_data_show(apop_test_fisher_exact(d));

    apop_data_fill(d,
            1100, 1400,
            2400, 3500);

    printf("\n Scaled 100X:\n");
    apop_data_show(apop_test_fisher_exact(d));
}
