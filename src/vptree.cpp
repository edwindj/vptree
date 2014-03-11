
#include <Rcpp.h>
#include <iostream>
#include <stdio.h>
#include "vptree.h"

using namespace Rcpp;

inline double eudist(const NumericVector& x, const NumericVector& y) {
    int nx = x.size();
    int ny = y.size();
    //Rcout << nx << '\n' << ny << std::endl;
    if(nx != ny) {
        throw std::invalid_argument("Two vectors are not of the same length!");
    }

    double dist=0;
    for(int i = 0; i < nx; i++) {
        dist += pow(x[i] - y[i], 2);
    }

    return sqrt(dist);
}

//[[Rcpp::export]]
List vptree(NumericMatrix m, int k){
  std::vector<NumericVector> rows;
  for (int r=0; r < m.nrow(); r++){
    rows.push_back(m(r,_));
  }
  VpTree<NumericVector, eudist> tree;
  tree.create(rows);
  
  NumericMatrix dist = NumericMatrix(m.nrow(),k);
  
  for (int r=0; r < m.nrow(); r++){
    std::vector<NumericVector> n;
    std::vector<double> d;
    tree.search(rows[r], k+1, &n, &d);
    NumericMatrix::Row row = dist(r,_);
    NumericVector v(d.begin(),d.end());
    row = v;
  }
  List z = List::create(_["dist"] = dist);
  return z;
}
