// [[Rcpp::depends(ListOf)]]

#include <Rcpp.h>
#include <ListOf.h>

using namespace Rcpp;

typedef ListOf<NumericVector> NVList;

// [[Rcpp::export]]
NVList test_identity(NVList x) {
  return x;
}

template <typename T>
double sum_(const T& x) {
  return sum(x);
}

// [[Rcpp::export]]
List test_lapply_sum(NVList x) {
  return lapply(x, sum_<NumericVector>);
}

// [[Rcpp::export]]
NumericVector test_sapply_sum(NVList x) {
  return sapply(x, sum_<NumericVector>);
}

// [[Rcpp::export]]
NVList test_assign(NVList x, NumericVector y) {
  x[1] = y;
  return x;
}

// [[Rcpp::export]]
NVList test_assign_names(NVList x) {
  x["a"] = x["b"];
  return x;
}

// [[Rcpp::export]]
NumericVector test_add(NVList x) {
  return x[0] + x[1] + x[2];
}

// [[Rcpp::export]]
NVList test_add2(NVList x) {
  x[0] += x[1];
  return x;
}

// [[Rcpp::export]]
NumericVector test_add_subtract(NVList x) {
  return x[0] + x[1] - x[2];
}

// [[Rcpp::export]]
NumericVector test_mult(NVList x) {
  return x[0] * x[1] * x[2];
}

typedef ListOf<CharacterVector> CVList;
// [[Rcpp::export]]
CVList test_char(CVList x) {
  x[0] = "apple";
  return x;
}

// [[Rcpp::export]]
void test_validate(NVList x) {
  x.validate();
}

typedef ListOf<NumericMatrix> NMList;

// [[Rcpp::export]]
NVList test_matrix_sum(NVList x) {
  return lapply(x, sum_<NumericVector>);
}

// [[Rcpp::export]]
double test_sum_all(NVList x) {
  return sum( sapply(x, sum_<NumericVector>) );
}
// [[Rcpp::export]]
NVList test_as_wrap(NVList x) {
  List y = as<List>(x);
  NVList z = as<NVList>(y);
  NumericVector k = x[0];
  x[0] = z[1];
  return z;
}

ListOf<List> test_lol( ListOf<List> x ) {
  return x[0];
}

/*** R
x <- list( c(1, 5), c(2, 6), c(3, 7) )
test_identity( setNames(x, c('a', 'b', 'c')) )
test_lapply_sum(x)
test_sapply_sum(x)
test_assign(x, 100)
x
x <- setNames(list(1, 2, 3), c('a', 'b', 'c'))
test_assign_names(x)
x
test_add(list(1, 2, 3))
test_add2(list(1, 2, 3))
test_add_subtract(list(1, 2, 3))
test_mult( list(1, 2, 3) )
test_char( list("banana") )
test_validate( list(1, 2, 3) )
tryCatch(test_validate( list(1, 2, '4') ),
  error=function(e) {
    print(e)
  }
)
tryCatch(test_assign_names(
  list(alpha=1, beta=2, gamma=3)
  ), error=function(e) {
    print(e)
  }
)
test_sum_all( list(1, c(2, 3), c(4, 5, 6)) )
test_as_wrap( list(1, 5, 10) )

library(microbenchmark)
m <- replicate(100, rnorm(1E5), simplify=FALSE)
microbenchmark(
  sapply(m, sum, USE.NAMES=FALSE),
  test_sapply_sum(m)
)

m <- replicate(100, matrix(rnorm(1E6), nrow=1E2), simplify=FALSE)
microbenchmark( times=5, 
  lapply(m, sum),
  test_matrix_sum(m)
)
*/
