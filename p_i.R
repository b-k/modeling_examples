p_i <- function(dataset, col1=1, col2=2){

    entropy <- function(inlist){
        -sum(sapply(inlist, function(x){log2(x)*x}), na.rm=TRUE)
    }

    information_in_agreement <- function(diag, margin1, margin2){
        sum <- 0
        for (i in 1:length(diag))
            if (diag[i] != 0)
                sum <- sum + diag[i]*log2(diag[i]/(margin1[i]*margin2[i]))
        return (sum)
    }

    dataset <- as.data.frame(dataset) #in case user provided a matrix.
    crosstab <- table(as.data.frame(cbind(dataset[,col1],dataset[,col2])))
    d1tab <- table(dataset[,col1])
    d2tab <- table(dataset[,col2])
    d1tab <- d1tab/sum(d1tab)
    d2tab <- d2tab/sum(d2tab)
    crosstab <- crosstab/sum(crosstab)

    entropy_1 <- entropy(d1tab)
    entropy_2 <- entropy(d2tab)
    ia <- information_in_agreement(diag(crosstab), d1tab, d2tab)
    return (2*ia/(entropy_1+entropy_2))
}

test_p_i <- function(){
    fullagreement <- matrix(
                    c(1,1,1,1,2,2,2,2,3,3,
                      1,1,1,1,2,2,2,2,3,3),
                    ncol=2, byrow=FALSE
                )

    stopifnot(p_i(fullagreement)==1)

    noagreement <- matrix(
                    c(1,2,1,2,1,2,3,1,3,2,
                      2,1,3,1,2,3,2,2,1,3),
                    ncol=2, byrow=FALSE
                )

    stopifnot(p_i(noagreement)==0)

    constant <- matrix(
                    c(1,1,1,1,1,1,
                      1,1,2,2,2,3),
                    ncol=2, byrow=FALSE
                )

    stopifnot(p_i(constant)==0)

    neg_corr <- matrix(
                    c(1,1,1,1,1,2,2,2,2,2,
                      1,2,2,2,2,1,1,1,1,2),
                    ncol=2, byrow=FALSE
                )

    stopifnot(abs(p_i(neg_corr)- -.2643856) < 1e-6)

    rare_agreement <- matrix(
                    c(1,1,1,2,1,2,2,2,3,3,
                      1,1,1,1,2,2,2,2,3,3),
                    ncol=2, byrow=FALSE
                )

    stopifnot(abs(p_i(rare_agreement)- .6626594) < 1e-6)

    common_agreement <- matrix(
                    c(1,1,1,1,2,2,2,3,2,3,
                      1,1,1,1,2,2,2,2,3,3),
                    ncol=2, byrow=FALSE
                )

    stopifnot(abs(p_i(common_agreement)- 0.6130587) < 1e-6)
}
