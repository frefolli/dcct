#ifndef DCCT_MATRIX_HH
#define DCCT_MATRIX_HH
/** @file matrix.hh matrix related utilities */
#include <dcct/specifiers.hh>
#include <string>
#include <eigen3/Eigen/Core>
namespace dcct {
  /**
   * Loads Matrix from File
   * @param matrix output matrix
   * @param filepath input file path
  */
  bool LoadMatrix(Eigen::MatrixXd& matrix, const std::string& filepath);

  /**
   * Dumps Matrix to File
   * @param matrix input matrix
   * @param filepath output file path
  */
  bool DumpMatrix(const Eigen::MatrixXd& matrix, const std::string& filepath);

  /**
   * Creates a matrix from a pattern of specifier
   * @param matrix output matrix
   * @param pattern input pattern
  */
  bool FromMatrixSpecifier(Eigen::MatrixXd& matrix, std::string pattern);

  /**
   * Creates a matrix from a concrete specifier
   * @param matrix output matrix
   * @param specifier input specifier
  */
  bool FromMatrixSpecifier(Eigen::MatrixXd& matrix, MatrixSpecifier& specifier);
}
#endif//DCCT_MATRIX_HH