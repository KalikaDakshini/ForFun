#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include "Base.hpp"
#include "StorageBase.hpp"

namespace KaliMatrix
{
  template<typename T, size_t Rows, size_t Cols, bool Static>
  struct MatrixBase : Internal::Base<MatrixBase<T, Rows, Cols, Static>> {
    MatrixBase() : m_storage_() {}

    MatrixBase(size_t rows, size_t cols) : m_storage_(rows, cols) {}

    constexpr T operator()(size_t i, size_t j) const
    {
      return m_storage_(i, j);
    }

    constexpr size_t rows() const { return m_storage_.rows(); }

    constexpr size_t cols() const { return m_storage_.cols(); }

    constexpr void resize(size_t rows, size_t cols)
    {
      this->m_storage_.resize(rows, cols);
    }

    constexpr void cons_resize(size_t rows, size_t cols)
    {
      this->m_storage_.cons_resize(rows, cols);
    }

    constexpr T &operator()(size_t i, size_t j) { return m_storage_(i, j); }

    StorageBase<T, Rows, Cols, Static> m_storage_;
  };

  namespace Internal
  {

    template<typename T, size_t Rows, size_t Cols, bool Static>
    struct Traits<MatrixBase<T, Rows, Cols, Static>> {
      using Type = T;
    };
  }  //namespace Internal
};  //namespace KaliMatrix

#endif
