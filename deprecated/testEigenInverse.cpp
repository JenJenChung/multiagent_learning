#include <iostream>
#include <Eigen/Dense>
#include "Utilities/UtilFunctions.h"
#include "Utilities/MatrixTypes.h"

using easymath::rand ;
using easymath::zeros ;
using easymath::matrix_mult ;
using namespace Eigen;

int main()
{
//   Matrix3f A;
//   A << 1, 2, 1,
//        2, 1, 0,
//        -1, 1, 2;
//   cout << "Here is the matrix A:\n" << A << endl;
//   cout << "The determinant of A is " << A.determinant() << endl;
//   cout << "The inverse of A is:\n" << A.inverse() << endl;

  size_t N = 3 ;
  matrix2d A = zeros(N,N) ;
  for (size_t i = 0; i < A.size(); i++)
    for (size_t j = i; j < A[i].size(); j++)
      A[i][j] = rand(1.0, 10.0) ;
  
  matrix2d At = zeros(N,N) ;
  for (size_t i = 0; i < A.size(); i++)
    for (size_t j = 0; j < i+1; j++)
      At[i][j] = A[j][i] ;
  
  printf("A:\n") ;
  for (size_t i = 0; i < A.size(); i++){
    for (size_t j = 0; j < A[i].size(); j++){
      printf("%f ",A[i][j]) ;
    }
    printf("\n") ;
  }
  printf("A transpose:\n") ;
  for (size_t i = 0; i < At.size(); i++){
    for (size_t j = 0; j < At[i].size(); j++){
      printf("%f ",At[i][j]) ;
    }
    printf("\n") ;
  }
  
  matrix2d B = matrix_mult(A,At) ;
  printf("B = A*A: \n") ;
  for (size_t i = 0; i < B.size(); i++){
    for (size_t j = 0; j < B[i].size(); j++){
      printf("%f ",B[i][j]) ;
    }
    printf("\n") ;
  }

  MatrixXd BB(N,N) ;
  for (size_t i = 0; i < B.size(); i++)
    for (size_t j = 0; j < B[i].size(); j++)
      BB(i,j) = B[i][j] ;
  std::cout << "MatrixXd BB:\n" << BB << std::endl ;
  MatrixXd invBB = BB.inverse() ;
  double detInvBB = invBB.determinant() ;
  std::cout << "MatrixXd invBB:\n" << invBB << std::endl ;
  std::cout << "double detInvBB:\n" << detInvBB << std::endl ;
  
  matrix2d invB = zeros(N,N) ;
  matrix2d diffInvB = zeros(N,N) ;
  printf("invB:\n") ;
  for (size_t i = 0; i < invB.size(); i++){
    for (size_t j = 0; j < invB[i].size(); j++){
      invB[i][j] = invBB(i,j) ;
      diffInvB[i][j] = invB[i][j] - invBB(i,j) ;
      printf("%f ",invB[i][j]) ;
    }
    printf("\n") ;
  }
  for (size_t i = 0; i < invB.size(); i++){
    for (size_t j = 0; j < invB[i].size(); j++){
      printf("%f ",diffInvB[i][j]) ;
    }
    printf("\n") ;
  }
  return 0 ;
}

