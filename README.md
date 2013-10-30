ListOf
======

This package implements typed List containers for Rcpp.

Hopefully, this will become a part of Rcpp after a proper test suite has been 
developed and behavior is documented / working as intended. For now, we
can play around more easily by installing this separately.

To install,

    if (!require(devtools)) install.packages("devtools")
    devtools::install_github("ListOf", "kevinushey")
    
To use in a C++ source file, place the Rcpp attribute

    // [[Rcpp::depends(ListOf)]]
    
at the top of your source file; to use in a package, make sure you have

    LinkingTo: Rcpp, ListOf
    
in your package `DESCRIPTION`.
