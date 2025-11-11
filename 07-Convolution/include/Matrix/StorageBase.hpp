#ifndef STORAGE_H
#define STORAGE_H

#include <array>
#include <cstddef>
#include <cstring>
#include <exception>

#include "Base.hpp"

namespace KaliMatrix
{
  using std::size_t;

  namespace Internal
  {
    constexpr size_t Dynamic{0};

    /**
     * @brief Dynamically allocated Array
     *
     */
    template<typename T, size_t Size>
    struct DynamicArray : ArrayBase<DynamicArray<T, Size>> {
      size_t size_ = Size;
      T *arr;

      DynamicArray() : arr(Size == 0 ? nullptr : new T[Size]) {}

      DynamicArray(size_t rows, size_t cols) :
        size_(rows * cols),
        arr((rows == 0 || cols == 0) ? nullptr : new T[rows * cols])
      {}

      DynamicArray(const DynamicArray &other)
      {
        // Copy parameters
        this->arr = new T[other.size_];
        std::memcpy(this->arr, other.arr, other.size_ * sizeof(T));
        this->size_ = other.size_;
      }

      // Copy Assignment operator;
      DynamicArray &operator=(const DynamicArray &other)
      {
        if (this == &other) {
          return *this;
        }

        delete[] this->arr;
        this->arr = new T[other.size_];
        std::memcpy(this->arr, other.arr, other.size_ * sizeof(T));
        this->size_ = other.size_;
        return *this;
      }

      // Resizes and deletes the buffer
      void resize(size_t new_size)
      {
        if (new_size != size_) {
          delete[] arr;
          arr = new T[new_size];
          size_ = new_size;
        }
      }

      // Conserves the elements on resize
      void cons_resize(size_t new_size)
      {
        if (new_size == size_) {
          return;
        }

        T *tmp = new T[new_size];
        std::memcpy(tmp, arr, std::min(size_, new_size) * sizeof(T));
        delete[] arr;
        arr = tmp;
        size_ = new_size;
      }

      // TODO(kalika): What happens if T needs allocation?
      constexpr ~DynamicArray() { delete[] arr; }
    };

    /**
     * @brief Statically allocated Array
     *
     */
    template<typename T, size_t Size>
    struct StaticArray : ArrayBase<StaticArray<T, Size>> {
      size_t size_ = Size;
      constexpr StaticArray() : arr() {};

      std::array<T, Size> arr;
    };

    template<typename T, size_t Size> struct Traits<DynamicArray<T, Size>> {
      using Type = T;
    };

    template<typename T, size_t Size> struct Traits<StaticArray<T, Size>> {
      using Type = T;
    };

    /**
     * @brief Statically Allocated Matrix
     *
     */
    template<typename T, size_t Rows, size_t Cols, bool Static = true>
    struct StorageBaseImpl {
      // Constructors
      constexpr StorageBaseImpl() : m_data_() {};

      constexpr StorageBaseImpl(size_t rows, size_t cols) {}

      constexpr StorageBaseImpl(const StorageBaseImpl &) = default;
      constexpr StorageBaseImpl &operator=(const StorageBaseImpl &) = default;

      // Resize the storage
      constexpr void resize(const size_t rows, const size_t cols)
      {
        static_assert(
          Rows != Dynamic && Cols != Dynamic,
          "Cannot resize a fixed-size matrix."
        );
      }

      constexpr void cons_resize(size_t rows, size_t cols)
      {
        resize(rows, cols);
      }

    protected:
      StaticArray<T, Rows * Cols> m_data_;
      size_t rows_ = Rows;
      size_t cols_ = Cols;
    };

    /**
     * @brief Dynamically Allocated Matrix
     *
     */
    template<typename T> struct StorageBaseImpl<T, Dynamic, Dynamic, false> {
      // Constructors
      StorageBaseImpl() = delete;

      constexpr StorageBaseImpl(size_t rows, size_t cols) :
        m_data_(rows, cols), rows_(rows), cols_(cols)
      {}

      constexpr StorageBaseImpl(const StorageBaseImpl &) = default;
      constexpr StorageBaseImpl &operator=(const StorageBaseImpl &) = default;

      constexpr void resize(size_t rows, size_t cols)
      {
        size_t new_size = rows * cols;
        if (new_size != m_data_.size()) {
          m_data_.resize(new_size);
        }
        rows_ = rows;
        cols_ = cols;
      }

      constexpr void cons_resize(size_t rows, size_t cols)
      {
        size_t new_size = rows * cols;
        if (new_size != m_data_.size()) {
          m_data_.cons_resize(new_size);
        }
        rows_ = rows;
        cols_ = cols;
      }

    protected:
      DynamicArray<T, 0> m_data_;
      size_t rows_ = 0;
      size_t cols_ = 0;
    };
  }  //namespace Internal

  /**
   * @brief Storage base to hold values in a contiguous fashion
   * @tparam T Data type
   * @tparam Rows Number of rows
   * @tparam Cols Number of cols
   * @tparam Static Mem Alloc status
   */
  template<typename T, size_t Rows, size_t Cols, bool Static>
  struct StorageBase : Internal::StorageBaseImpl<T, Rows, Cols, Static> {
    using Base = Internal::StorageBaseImpl<T, Rows, Cols, Static>;
    using Base::cols_;
    using Base::rows_;

    StorageBase() = default;

    StorageBase(size_t rows, size_t cols) : Base(rows, cols) {}

    constexpr StorageBase(const StorageBase &) = default;
    constexpr StorageBase &operator=(const StorageBase &) = default;

    constexpr const T &operator()(size_t i, size_t j) const
    {
      return Base::m_data_((i * cols_) + j);
    }

    constexpr T &operator()(size_t i, size_t j)
    {
      return Base::m_data_((i * cols_) + j);
    }

    constexpr size_t rows() const { return this->rows_; }

    constexpr size_t cols() const { return this->cols_; }
  };

};  //namespace KaliMatrix

#endif
