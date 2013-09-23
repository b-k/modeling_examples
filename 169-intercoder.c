#include "169-kappa_and_pi.h"

int main(){
    int readin_status = apop_text_to_db("amash_vote_analysis.csv", .tabname="amash");
    Apop_stopif(readin_status== -1, exit(1), 0, "Trouble reading in the data. "
                     "Have you downloaded it to this directory?");

    apop_query("create table summed as select vote, party, count(*) as ct from amash group by vote, party");
    apop_data *confusion = apop_db_to_crosstab("summed", "vote", "party", "ct");
    apop_data_show(confusion);
    apop_data_show(kappa_and_pi(confusion));

//Find the contribution level that matches the count of democrats;
//use that cutoff to produce a binary small-contrib|big-contrib categorization.
//There are 194 dems. The contribution cutoff is thus $13,000. There are four
    printf("\n\n");
    double dem_count = apop_query_to_float("select count(*) from amash where party='Democrat'");
    apop_query("create table contrib_sums as select vote, "
            "case when contribs+0.0> (select max(contribs) from "
                "(select contribs from amash order by contribs limit %g)) "
                "then 'more $' else 'less $' end "
                "as big_dollars, "
            "count(*) as ct from amash group by vote, big_dollars", dem_count);

    confusion = apop_db_to_crosstab("contrib_sums", "vote", "big_dollars", "ct");
    apop_data_show(confusion);
    apop_data_show(kappa_and_pi(confusion));
}
