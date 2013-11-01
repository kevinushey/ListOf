#ifndef MatrixDims_h_
#define MatrixDims_h_

#define DEBUG

#ifdef DEBUG
#define debug(x) Rprintf(x)
#define debug2(x, y) Rprintf(x, y)
#define debug3(x, y, z) Rprintf(x, y, z)
#else
#define debug(x)
#define debug2(x, y)
#define debug3(x, y, z)
#endif

#include <RcppCommon.h>

namespace Rcpp {
  
  class NamesProxy {
    public:
    NamesProxy() {};
    NamesProxy(const NamesProxy& sexp_): sexp(sexp_) {};
    NamesProxy(const RObject& sexp_): sexp(sexp_) {};
    
    NamesProxy& operator=(const RObject& rhs) {
      if (Rf_length(rhs) != Rf_length(sexp)) {
        Rf_error("'names' attribute [%i] must be the same length as the vector [%i]", 
          Rf_length(rhs), Rf_length(sexp));
      }
      sexp.attr("names") = as<CharacterVector>(rhs);
      return *this;
    }
    
    operator CharacterVector() const {
      return as<CharacterVector>( sexp.attr("names") );
    }
    
    private:
      RObject sexp;
  };
  
  NamesProxy names(const RObject& x) {
    return NamesProxy(x);
  }
  
  class DimNamesProxy {
    public:
      DimNamesProxy() {};
      DimNamesProxy(const DimNamesProxy& sexp_, int i_): sexp(sexp_), i(i_) {};
      DimNamesProxy(const RObject& sexp_, int i_): sexp(sexp_), i(i_) {
        debug("DimNamesProxy(const RObject& sexp_): sexp(sexp_)\n");
      };
      
      DimNamesProxy& operator=(const RObject& rhs) {
        debug("DimNamesProxy& operator=(const RObject& rhs)\n");
        if (!Rf_isArray(sexp)) {
          Rf_warning("not an array");
          return *this;
        }
        if (Rf_isNull( Rf_getAttrib(sexp, R_DimNamesSymbol) )) {
          debug("DimNames slot null; creating List(2)\n");
          Rf_setAttrib(sexp, R_DimNamesSymbol, List::create(2));
        }
        debug("Setting DimNames slot\n");
        SET_VECTOR_ELT(Rf_getAttrib(sexp, R_DimNamesSymbol), i, rhs);
        return *this;
      }
      
      operator CharacterVector() const {
        debug("operator CharacterVector() const\n");
        return as<CharacterVector>(
          VECTOR_ELT(Rf_getAttrib(sexp, R_DimNamesSymbol), i)
        );
      }
      
    private:
      RObject sexp;
      int i;
  }; // DimNamesProxy
  
  DimNamesProxy rownames(const RObject& x) {
    return DimNamesProxy(x, 0);
  }
  
  DimNamesProxy colnames(const RObject& x) {
    return DimNamesProxy(x, 1);
  }
  
} // Rcpp

#endif
