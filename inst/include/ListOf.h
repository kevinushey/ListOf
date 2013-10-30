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
      Proxy& operator=(const Proxy& rhs) {
        debug("Proxy& operator=(const Proxy& rhs)\n");
        static_cast<List>(list)[index] = static_cast<List>(rhs.list)[rhs.index];
        return *this;
      }
      
      Proxy& operator=(T vector) {
        debug("Proxy& operator=(T vector)\n");
        static_cast<List>(list)[index] = as<T>(vector);
        return *this;
      }
      
      // addition operators
      T operator+(const Proxy& rhs) {
        debug("T operator+(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) + as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      friend T operator+(const T& lhs, const Proxy& rhs) {
        debug("friend T operator+(const T& lhs, const Proxy& rhs)\n");
        return lhs + as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      T operator+(const U& rhs) {
        debug("T operator+(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) + rhs;
      }
      
      template <typename U>
      Proxy& operator+=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) + rhs;
        return *this;
      }
      
      // subtraction operators
      T operator-(const Proxy& rhs) {
        debug("T operator-(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) - as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      friend T operator-(const T& lhs, const Proxy& rhs) {
        debug("friend T operator-(const T& lhs, const Proxy& rhs)\n");
        return lhs - as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      T operator-(const U& rhs) {
        debug("T operator-(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) - rhs;
      }
      
      template <typename U>
      Proxy& operator-=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) - rhs;
        return *this;
      }
      
      // multiplication operators
      T operator*(const Proxy& rhs) {
        debug("T operator*(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) * as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      friend T operator*(const T& lhs, const Proxy& rhs) {
        debug("friend T operator*(const T& lhs, const Proxy& rhs)\n");
        return lhs * as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      T operator*(const U& rhs) {
        debug("T operator*(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) * rhs;
      }
      
      template <typename U>
      Proxy& operator*=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) * rhs;
        return *this;
      }
      
      // division operators
      T operator/(const Proxy& rhs) {
        debug("T operator/(const Proxy& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) / as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      friend T operator/(const T& lhs, const Proxy& rhs) {
        debug("friend T operator/(const T& lhs, const Proxy& rhs)\n");
        return lhs / as<T>(static_cast<List>(rhs.list)[rhs.index]);
      }
      
      template <typename U>
      T operator/(const U& rhs) {
        debug("T operator/(const U& rhs)\n");
        return as<T>(static_cast<List>(list)[index]) / rhs;
      }
      
      template <typename U>
      Proxy& operator/=(const U& rhs) {
        static_cast<List>(list)[index] =
          as<T>(static_cast<List>(list)[index]) / rhs;
        return *this;
      }
      
      // read
      operator T() const {
        debug("operator T() const\n");
        return as<T>(static_cast<List>(list)[index]);
      }
      
      // TODO: reference operator
      
    private:
      ListOf& list;
      int index;
    }; // Proxy
    
    friend class Proxy;
  
    ListOf() {}
    ListOf(const List& list_): List(list_) {}
    
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
      Rf_error("No name '%s' in the names attribute of the list supplied", str.c_str());
    }
    
    const Proxy operator[](std::string str) const {
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
