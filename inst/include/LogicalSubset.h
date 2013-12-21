#ifndef LogicalSubset_h_
#define LogicalSubset_h_

#include <Rcpp.h>

template <typename T>
T subset(const T& lhs, const LogicalVector& rhs) {
  if (lhs.size() != rhs.size()) {
    throw Rcpp::exception("Unequal sizes");
  }
  int n = sum(rhs);
  IntegerVector which = no_init(n);
  int count = 0;
  for (int i=0; i < rhs.size(); ++i) {
    if (rhs[i]) {
      which[count] = i;
      ++count;
  }
  T output = no_init(n);
  for (int i=0; i < n; ++i) {
    output[i] = lhs[ which[i] ];
  }
  return output;
}

#endif
