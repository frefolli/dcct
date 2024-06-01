#include <dcct/matrix.hh>
#include <fstream>
#include <filesystem>

bool dcct::LoadMatrix(Eigen::MatrixXd& matrix, const std::string& filepath) {
  if (!std::filesystem::exists(filepath))
    return false;
  
  std::ifstream file;
  file.open(filepath);
  uint32_t N = 0, M = 0;
  double_t value = 0.0f;
  file >> N >> M;
  matrix.resize(N, M);
  for (uint32_t i = 0; i < N; ++i) {
    for (uint32_t j = 0; j < N; ++j) {
      file >> value;
      matrix.coeffRef(i, j) = value;
    }
  }
  file.close();
  return true;
}

bool dcct::DumpMatrix(const Eigen::MatrixXd& matrix, const std::string& filepath) {
  std::ofstream file;
  file.open(filepath);
  uint32_t N = matrix.rows(), M = matrix.cols();
  file << N << " " << M;
  for (uint32_t i = 0; i < N; ++i) {
    file << std::endl;
    for (uint32_t j = 0; j < N; ++j) {
      if (j != 0)
        file << " ";
      file << matrix.coeff(i, j);
    }
  }
  file.close();
  return true;
}

bool dcct::FromMatrixSpecifier(Eigen::MatrixXd& matrix, std::string pattern) {
  dcct::MatrixSpecifier specifier = dcct::ParseMatrixSpecifier(pattern);
  return dcct::FromMatrixSpecifier(matrix, specifier);
}

bool dcct::FromMatrixSpecifier(Eigen::MatrixXd& matrix, dcct::MatrixSpecifier& specifier) {
  bool ok = true;
  switch (specifier.type) {
    case dcct::MatrixSpecifier::Type::SRC: {
      ok = dcct::LoadMatrix(matrix, "resources/matrices/" + specifier.ID + ".mat");
    }; break;
    case dcct::MatrixSpecifier::Type::RND: {
      // TODO:
    }; break;
    case dcct::MatrixSpecifier::Type::NONE: {
      ok = false;
    }; break;
  }
  return ok;
}