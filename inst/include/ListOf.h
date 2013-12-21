#ifndef ListOf_h_
#define ListOf_h_

// a define used to clean up some of the code repetition
#define THIS static_cast<List>(list)[index]
#define LHS static_cast<List>(lhs.list)[lhs.index]
#define RHS static_cast<List>(rhs.list)[rhs.index]

#include <RcppCommon.h>

namespace Rcpp {
  
  template <typename T>
  class ListOf: public List {
    
  public:
  
    // Proxy class for distinguishing [] read/write
    class Proxy {
    public:
    
      Proxy(ListOf& list_, int index_): list(list_), index(index_) {
        RCPP_DEBUG("Proxy(ListOf& list_, int index_): list(list_), index(index_)\n");
      }
      
      ~Proxy() {
        RCPP_DEBUG("~Proxy()\n");
      }
      
      // assignment operators
      Proxy& operator=(const Proxy& rhs) {
        RCPP_DEBUG("Proxy& operator=(const Proxy& rhs)\n");
        THIS = RHS;
        return *this;
      }
      
      Proxy& operator=(T vector) {
        RCPP_DEBUG("Proxy& operator=(T vector)\n");
        THIS = vector;
        return *this;
      }
      
      // addition operators
      T operator+(const Proxy& rhs) {
        RCPP_DEBUG("T operator+(const Proxy& rhs)\n");
        return as<T>(THIS) + as<T>(RHS);
      }
      
      friend T operator+(const T& lhs, const Proxy& rhs) {
        RCPP_DEBUG("friend T operator+(const T& lhs, const Proxy& rhs)\n");
        return lhs + as<T>(RHS);
      }
      
      template <typename U>
      T operator+(const U& rhs) {
        RCPP_DEBUG("T operator+(const U& rhs)\n");
        return as<T>(THIS) + rhs;
      }
      
      template <typename U>
      Proxy& operator+=(const U& rhs) {
        THIS = as<T>(THIS) + rhs;
        return *this;
      }
      
      // subtraction operators
      T operator-(const Proxy& rhs) {
        RCPP_DEBUG("T operator-(const Proxy& rhs)\n");
        return as<T>(THIS) - as<T>(RHS);
      }
      
      friend T operator-(const T& lhs, const Proxy& rhs) {
        RCPP_DEBUG("friend T operator-(const T& lhs, const Proxy& rhs)\n");
        return lhs - as<T>(RHS);
      }
      
      template <typename U>
      T operator-(const U& rhs) {
        RCPP_DEBUG("T operator-(const U& rhs)\n");
        return as<T>(THIS) - rhs;
      }
      
      template <typename U>
      Proxy& operator-=(const U& rhs) {
        THIS = as<T>(THIS) - rhs;
        return *this;
      }
      
      // multiplication operators
      T operator*(const Proxy& rhs) {
        RCPP_DEBUG("T operator*(const Proxy& rhs)\n");
        return as<T>(THIS) * as<T>(RHS);
      }
      
      friend T operator*(const T& lhs, const Proxy& rhs) {
        RCPP_DEBUG("friend T operator*(const T& lhs, const Proxy& rhs)\n");
        return lhs * as<T>(RHS);
      }
      
      template <typename U>
      T operator*(const U& rhs) {
        RCPP_DEBUG("T operator*(const U& rhs)\n");
        return as<T>(THIS) * rhs;
      }
      
      template <typename U>
      Proxy& operator*=(const U& rhs) {
        THIS = as<T>(THIS) * rhs;
        return *this;
      }
      
      // division operators
      T operator/(const Proxy& rhs) {
        RCPP_DEBUG("T operator/(const Proxy& rhs)\n");
        return as<T>(THIS) / as<T>(RHS);
      }
      
      friend T operator/(const T& lhs, const Proxy& rhs) {
        RCPP_DEBUG("friend T operator/(const T& lhs, const Proxy& rhs)\n");
        return lhs / as<T>(RHS);
      }
      
      template <typename U>
      T operator/(const U& rhs) {
        RCPP_DEBUG("T operator/(const U& rhs)\n");
        return as<T>(THIS) / rhs;
      }
      
      template <typename U>
      Proxy& operator/=(const U& rhs) {
        THIS = as<T>(THIS) / rhs;
        return *this;
      }
      
      // read
      operator T() const {
        RCPP_DEBUG("operator T() const\n");
        return as<T>(THIS);
      }
      
      // TODO: reference operator
      
    private:
      ListOf& list;
      int index;
    }; // Proxy
    
    friend class Proxy;
  
    ListOf() {}
    
    template <typename U>
    ListOf(const U& data_): List(data_) {}
    
    operator SEXP() const {
      return wrap(static_cast<const List&>(*this));
    }

    Proxy operator[](int i) {
      return Proxy(*this, i);
    }
    
    const Proxy operator[](int i) const {
      return Proxy(const_cast< ListOf<T>& >(*this), i);
    }
    
    Proxy operator[](std::string str) {
      std::vector<std::string> names = as< std::vector<std::string> >(this->attr("names"));
      for (int i=0; i < this->size(); ++i) {
        if (names[i] == str) {
          return Proxy(*this, i);
        }
      }
      std::stringstream s;
      s << "No name '" <<
        str <<
        "' in the names of the attributes of the list supplied"
      ;
      throw Rcpp::exception(s.str().c_str());
    }
    
    const Proxy operator[](std::string str) const {
      std::vector<std::string> names = as< std::vector<std::string> >(this->attr("names"));
      for (int i=0; i < this->size(); ++i) {
        if (names[i] == str) {
          return Proxy(const_cast< ListOf<T>& >(*this), i);
        }
      }
      std::stringstream s;
      s << "No name '" <<
        str <<
        "' in the names of the attributes of the list supplied"
      ;
      throw Rcpp::exception(s.str().c_str());
    }
    
    void validate() {
      for (int i=0; i < this->size(); ++i) {
        if (!is<T>( static_cast<List&>(*this)[i] )) {
          std::stringstream s;
          s << "Invalid ListOf<T> object: expected '" <<
            Rf_type2char( TYPEOF( T() ) ) <<
            "' but got '" <<
            Rf_type2char( TYPEOF( static_cast<List&>(*this)[i] ) ) <<
            "' at index " << i << "."
          ;
          throw Rcpp::exception(s.str().c_str());
        }
      }
    }
    
  }; // ListOf<T>
  
  template <class T>
  class SafeListOf: public ListOf<T> {
    public:
      SafeListOf() {}
      template <typename U>
      SafeListOf(const U& data_): ListOf<T>(data_) {
        for (List::iterator it = this->begin(); it != this->end(); ++it) {
          if (!is<T>(*it)) 
            *it = as<T>(*it);
        }
      }
  }; // SafeListOf<T>
  
} // Rcpp

#endif
