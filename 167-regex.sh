gcc -xc - `pkg-config --libs --cflags apophenia` -g -Wall --std=gnu99 -O3 << "----"
#include <apop.h>

int main(int argc, char **argv){
    Apop_stopif(argc == 2, return 1, 0, "Give two arguments: a regex and a string.");
    apop_data *txt;
    int returnval = apop_regex(argv[2], argv[1], &txt);
    Apop_stopif(!returnval, return 1, 0, "No matches found.");
    Apop_stopif(returnval==-1, return 2, 0, "Bad regex.");
    printf("%s", apop_text_paste(txt, .before="∙ [", .between_cols="] [", .between="]\n∙ [", .after="]\n"));
}
----

a.out "([[:alpha:]]+), *([-[:digit:].]+)" "{pants, 23} {plants,-14} {dirigibles, 12.81}"
echo -----
a.out "((<[^<]*>)([^<]*)</[^>]*>)" "<b>pants</b> <em>blazers</em>"
echo -----
a.out "([[:alnum:].])" " hello. "
