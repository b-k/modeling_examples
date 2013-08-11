#include <gsl/gsl_matrix.h>
#include <math.h>
#include <apop.h> //just for apop_matrix_print


#define make_a_list(...) (double[]){__VA_ARGS__, NAN}

#define matrix_cross(list1, list2) matrix_cross_base(make_a_list list1, make_a_list list2)

gsl_matrix *matrix_cross_base(double *list1, double *list2){
    int count1 = 0, count2 = 0;
    while(!isnan(list1[count1])) count1++;
    while(!isnan(list2[count2])) count2++;
    if (!count1 || !count2) return NULL;

    gsl_matrix *out = gsl_matrix_alloc(count1, count2);
    for (int i=0; i< count1; i++)
        for (int j=0; j< count2; j++)
            gsl_matrix_set(out, i, j, list1[i]*list2[j]);
    return out;
}

int main(){
    gsl_matrix *m = matrix_cross((1,2,4,8), (5, 11, 15));
    apop_matrix_print(m);

    printf("\n\n");
    apop_matrix_print( matrix_cross((17, 19, 23), (1,2,3,5,7,11,13)) );

    //create and fill a column vector
    printf("\n\n");
    apop_matrix_print( matrix_cross((1,2,3,5,7,11,13), (1)) );
}
