require("apop.jl")

db_open("test.db")
text_to_db("173-data.txt", "dta")
q = query_to_data("select out, input from dta")
data = data_as_array(q)
db_close()


oest = apop_estimate(q, "apop_ols")
out1 = data_as_vector(oest.parameters)

lest = apop_estimate(q, "apop_logit")
out2 = data_as_array(lest.parameters)
