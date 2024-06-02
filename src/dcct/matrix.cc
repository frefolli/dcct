#include <dcct/matrix.hh>
#include <fstream>
#include <filesystem>
#include <random>

inline void LoadMatrixBin(Eigen::MatrixXd& matrix, const std::string& filepath) {
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  uint32_t rows, cols;
  in.read((char*) &rows,sizeof(uint32_t));
  in.read((char*) &cols,sizeof(uint32_t));
  matrix.resize(rows, cols);
  in.read((char *) matrix.data() , rows * cols * sizeof(double));
  in.close();
}

inline void DumpMatrixBin(const Eigen::MatrixXd& matrix, const std::string& filepath) {
  std::ofstream out(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
  uint32_t rows = matrix.rows();
  uint32_t cols = matrix.cols();
  out.write((const char*) &rows, sizeof(uint32_t));
  out.write((const char*) &cols, sizeof(uint32_t));
  out.write((const char*) matrix.data(), rows * cols * sizeof(double));
  out.close();
}

inline void LoadMatrixMat(Eigen::MatrixXd& matrix, const std::string& filepath) {
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
}

inline void DumpMatrixMat(const Eigen::MatrixXd& matrix, const std::string& filepath) {
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
}

bool dcct::LoadMatrix(Eigen::MatrixXd& matrix, const std::string& filepath, dcct::MatrixFileFormat format) {
  if (!std::filesystem::exists(filepath))
    return false;
  switch (format) {
    case dcct::MatrixFileFormat::BIN: {
      LoadMatrixBin(matrix, filepath);
    }; break;
    case dcct::MatrixFileFormat::MAT: {
      LoadMatrixMat(matrix, filepath);
    }; break;
  }
  return true;
}

bool dcct::DumpMatrix(const Eigen::MatrixXd& matrix, const std::string& filepath, dcct::MatrixFileFormat format) {
  switch (format) {
    case dcct::MatrixFileFormat::BIN: {
      DumpMatrixBin(matrix, filepath);
    }; break;
    case dcct::MatrixFileFormat::MAT: {
      DumpMatrixMat(matrix, filepath);
    }; break;
  }
  return true;
}

bool dcct::FromMatrixSpecifier(Eigen::MatrixXd& matrix, const std::string& pattern) {
  dcct::MatrixSpecifier specifier = dcct::ParseMatrixSpecifier(pattern);
  return dcct::FromMatrixSpecifier(matrix, specifier);
}

bool dcct::FromMatrixSpecifier(Eigen::MatrixXd& matrix, const dcct::MatrixSpecifier& specifier) {
  bool ok = true;
  switch (specifier.type) {
    case dcct::MatrixSpecifier::Type::SRC: {
      ok = dcct::LoadMatrix(matrix, "resources/matrices/" + specifier.ID + ".bin", dcct::MatrixFileFormat::BIN);
      if (!ok)
        ok = dcct::LoadMatrix(matrix, "resources/matrices/" + specifier.ID + ".mat", dcct::MatrixFileFormat::MAT);
    }; break;
    case dcct::MatrixSpecifier::Type::RND: {
      ok = dcct::RandomizeMatrix(matrix, specifier.N, specifier.M);
    }; break;
    case dcct::MatrixSpecifier::Type::NONE: {
      ok = false;
    }; break;
  }
  return ok;
}

bool dcct::RandomizeMatrix(Eigen::MatrixXd& matrix, uint32_t N, uint32_t M) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::normal_distribution distribution{-10.0f, 10.0f};
  matrix.resize(N, M);
  for (uint32_t i = 0; i < N; ++i) {
    for (uint32_t j = 0; j < M; ++j) {
      matrix.coeffRef(i, j) = distribution(generator);
    }
  }
  return true;
}