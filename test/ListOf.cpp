#ifdef DEBUG
#define debug(x) Rprintf(x)
#define debug2(x, y) Rprintf(x, y)
#define debug3(x, y, z) Rprintf(x, y, z)
#else
#define debug(x)
#define debug2(x, y)
#define debug3(x, y, z)
#endif

#include <Rcpp.h>

namespace Rcpp {
  
  template <typename T>
  class ListOf: public List {
    
  public:
  
    // Proxy class for distinguishing [] read/write
    class Proxy {
    public:
    
      Proxy(ListOf& list_, int index_): list(list_), index(index_) {
        debug("Proxy(ListOf& list_, int index_): list(list_), index(index_)\n");
      }
      
      ~Proxy() {
        debug("~Proxy()\n");
      }
      
      // assignment operators
      inline Proxy& operator=(const Proxy& rhs) {
        debug("Proxy& operator=(const Proxy& rhs)\n");
        static_cast<List>(list)[index] = static_cast<List>(rhs.list)[rhs.index];
        return *this;
      }
      
      inline Proxy& operator=(T vector) {
        debug("Proxy& operator=(T vector)\n");
        static_cast<List>(list)[index] = as<T>(vector);
        return *this;
      }
      
      // addition operators
      inline T operator+(const Proxy& rhs) {
        debug("T operator+(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) + as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      inline friend T operator+(const T& lhs, const Proxy& rhs) {
        debug("friend T operator+(const T& lhs, const Proxy& rhs)\n");
        return lhs + as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      inline T operator+(const U& rhs) {
        debug("T operator+(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) + rhs;
      }
      
      template <typename U>
      inline Proxy& operator+=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) + rhs;
        return *this;
      }
      
      // subtraction operators
      inline T operator-(const Proxy& rhs) {
        debug("T operator-(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) - as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      inline friend T operator-(const T& lhs, const Proxy& rhs) {
        debug("friend T operator-(const T& lhs, const Proxy& rhs)\n");
        return lhs - as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      inline T operator-(const U& rhs) {
        debug("T operator-(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) - rhs;
      }
      
      template <typename U>
      inline Proxy& operator-=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) - rhs;
        return *this;
      }
      
      // multiplication operators
      inline T operator*(const Proxy& rhs) {
        debug("T operator*(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) * as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      inline friend T operator*(const T& lhs, const Proxy& rhs) {
        debug("friend T operator*(const T& lhs, const Proxy& rhs)\n");
        return lhs * as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      inline T operator*(const U& rhs) {
        debug("T operator*(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) * rhs;
      }
      
      template <typename U>
      inline Proxy& operator*=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) * rhs;
        return *this;
      }
      
      // division operators
      inline T operator/(const Proxy& rhs) {
        debug("T operator/(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) / as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      inline friend T operator/(const T& lhs, const Proxy& rhs) {
        debug("friend T operator/(const T& lhs, const Proxy& rhs)\n");
        return lhs / as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      inline T operator/(const U& rhs) {
        debug("T operator/(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) / rhs;
      }
      
      template <typename U>
      inline Proxy& operator/=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) / rhs;
        return *this;
      }
      
      // read
      inline operator T() const {
        debug("operator T() const\n");
        return as<T>(static_cast<List>(list)[index]);
      }
      
      // TODO: reference operator
      
    private:
      ListOf& list;
      int index;
    };
    
    friend class Proxy;
  
    ListOf() {}
    ListOf(const List& list_): List(list_) {}
    
    template <typename U>
    ListOf(const U& data_): List(data_) {}
    
    // for wrap
    operator SEXP() const {
      debug("operator SEXP() const\n");
      return wrap( static_cast<const List&>(*this) );
    }

    inline Proxy operator[](int i) {
      return Proxy(*this, i);
    }
    
    inline const Proxy operator[](int i) const {
      return Proxy(const_cast< ListOf<T>& >(*this), i);
    }
    
    inline Proxy operator[](std::string str) {
      std::vector<std::string> names = as< std::vector<std::string> >(this->attr("names"));
      for (int i=0; i < this->size(); ++i) {
        if (names[i] == str) {
          return Proxy(*this, i);
        }
      }
      Rf_error("No name '%s' in the names attribute of the list supplied", str.c_str());
    }
    
    inline const Proxy operator[](std::string str) const {
      std::vector<std::string> names = as< std::vector<std::string> >(this->attr("names"));
      for (int i=0; i < this->size(); ++i) {
        if (names[i] == str) {
          return Proxy(const_cast< ListOf<T>& >(*this), i);
        }
      }
      Rf_error("No name '%s' in the names attribute of the list supplied", str.c_str());
    }
    
    void validate() {
      for (int i=0; i < this->size(); ++i) {
        if (!is<T>( static_cast<List&>(*this)[i] )) {
          Rf_error(
            "Invalid ListOf<T> object: expected '%s' but got '%s' at index %i",
            Rf_type2char( TYPEOF( T() ) ),
            Rf_type2char( TYPEOF( static_cast<List&>(*this)[i] ) ),
            i
          );
        }
      }
    }
    
  }; // ListOf<T>
  
} // Rcpp

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
