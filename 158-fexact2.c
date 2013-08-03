#include <apop.h>

int main(){
    int readin_status = apop_text_to_db("amash_vote_analysis.csv", .tabname="amash");
    Apop_stopif( readin_status== -1, exit(1), 0, "Trouble reading in the data. "
                     "Have you downloaded it to this directory?");

    double median_contrib = apop_query_to_float("select max(contribs) from "
                            " (select contribs from amash order by contribs "
                            "  limit (select count(*)/2 from amash))");
    apop_query("create table pv_xtab as "
               " select vote, (contribs+0.0 > %g) as hi_money, count(*) as ct "
               " from amash "
               " group by vote, hi_money ", median_contrib);

    apop_data *xtab = apop_db_to_crosstab("pv_xtab", "hi_money", "vote", "ct");
    apop_data_show(xtab);
    apop_data_show(apop_test_fisher_exact(xtab));
}
