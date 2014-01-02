#include <apop.h>

int main(){
    apop_text_to_db("amash_vote_analysis.csv", .tabname="amash");
    apop_data *d = apop_query_to_data("select "
            "case when vote='Aye' then 0 else 1 end as vote, "
            "ideology, log(contribs+10) from amash");

    apop_data_show(apop_data_correlation(d));

    apop_data *eigens = apop_matrix_pca(d->matrix);

    printf("\nEigenvalues:\n");
    apop_vector_print(eigens->vector);

    printf("\nPrincipal eigenvector:\n");
    Apop_col_v(eigens, 0, principal);
    apop_vector_print(principal);
}
