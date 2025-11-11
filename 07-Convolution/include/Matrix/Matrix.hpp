#include "MatrixBase.hpp"

#include <cstddef>

namespace KaliMatrix
{
  using std::size_t;

  template<typename T, size_t Rows, size_t Cols, bool Static = false>
  struct Matrix : MatrixBase<T, Rows, Cols, Static> {
    using Base = MatrixBase<T, Rows, Cols, Static>;
    using Base::Base;
  };

  using Matrix2i = Matrix<int, 2, 2, true>;
  using Matrix3i = Matrix<int, 3, 3, true>;
  using Matrix4i = Matrix<int, 4, 4, true>;
  using Matrix2d = Matrix<double, 2, 2, true>;
  using Matrix3d = Matrix<double, 2, 2, true>;
  using Matrix4d = Matrix<double, 2, 2, true>;
  using MatrixXi = Matrix<int, 0, 0, false>;
  using MatrixXd = Matrix<double, 0, 0, false>;

}  //namespace KaliMatrix
