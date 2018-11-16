#include <iostream>
#include <fstream>
#include <sstream>
#include "Utilities/MatrixTypes.h"
#include "Utilities/UtilFunctions.h"

using easymath::zeros ;
using easymath::rand ;

int main(){
  std::cout << "Testing binary write in read out of 2D matrices\n" ;
  std::cout << "WARNING: CODE USES DEPRECATED LIBRARIES MATRIXTYPES.H AND UTILFUNTIONS.H\n" ;
  
  matrix2d randMatrix = zeros(3,2) ;
  for (size_t i = 0; i < randMatrix.size(); i++){
    for (size_t j = 0; j < randMatrix[i].size(); j++){
      randMatrix[i][j] = rand(0.0,1.0) ;
      std::cout << randMatrix[i][j] << "," ;
    }
    std::cout << std::endl ;
  }
  
  matrix2d onesMatrix = zeros(6,10) ;
  for (size_t i = 0; i < onesMatrix.size(); i++){
    for (size_t j = 0; j < onesMatrix[i].size(); j++){
      onesMatrix[i][j] = 1.0 ;
      std::cout << onesMatrix[i][j] << "," ;
    }
    std::cout << std::endl ;
  }
  
  const char * fName = "test" ;
	std::stringstream fileName ;
  fileName << fName ;
  std::ofstream testFile ;
  testFile.open(fileName.str().c_str(), std::ios::out | std::ios::binary) ;
  
  for (size_t i = 0; i < randMatrix.size(); i++){
    char * pMatrix = reinterpret_cast<char *>(&randMatrix[i][0]) ;
    size_t bytes = randMatrix[i].size() * sizeof(randMatrix[i][0]) ;
    std::cout << "randMatrix[i].size() * sizeof(randMatrix[i][0]): " << bytes << std::endl ;
    testFile.write(pMatrix, bytes) ;
  }
  
  for (size_t i = 0; i < onesMatrix.size(); i++){
    char * pMatrix = reinterpret_cast<char *>(&onesMatrix[i][0]) ;
    size_t bytes = onesMatrix[i].size() * sizeof(onesMatrix[i][0]) ;
    std::cout << "onesMatrix[i].size() * sizeof(onesMatrix[i][0]): " << bytes << std::endl ;
    testFile.write(pMatrix, bytes) ;
  }
  
  testFile.close() ;
  
  matrix2d newRandMatrix = zeros(3,2) ;
  matrix2d newOnesMatrix = zeros(6,10) ;
  
  std::ifstream readFile ;
  readFile.open(fileName.str().c_str(), std::ios::in | std::ios::binary) ;
  
  for (size_t i = 0; i < newRandMatrix.size(); i++){
    char * pNewMatrix = reinterpret_cast<char *>(&newRandMatrix[i][0]) ;
    size_t newBytes = newRandMatrix[i].size() * sizeof(newRandMatrix[i][0]) ;
    readFile.read(pNewMatrix, newBytes) ;
  }
  
  for (size_t i = 0; i < newOnesMatrix.size(); i++){
    char * pNewMatrix = reinterpret_cast<char *>(&newOnesMatrix[i][0]) ;
    size_t newBytes = newOnesMatrix[i].size() * sizeof(newOnesMatrix[i][0]) ;
    readFile.read(pNewMatrix, newBytes) ;
  }
  
  readFile.close() ;
  
  for (size_t i = 0; i < newRandMatrix.size(); i++){
    for (size_t j = 0; j < newRandMatrix[i].size(); j++){
      std::cout << newRandMatrix[i][j] << "," ;
    }
    std::cout << std::endl ;
  }
  
  for (size_t i = 0; i < newOnesMatrix.size(); i++){
    for (size_t j = 0; j < newOnesMatrix[i].size(); j++){
      std::cout << newOnesMatrix[i][j] << "," ;
    }
    std::cout << std::endl ;
  }
  
  std::cout << "Test complete!\n" ;
  return 0 ;
}
