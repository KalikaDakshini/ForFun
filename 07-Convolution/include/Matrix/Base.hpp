#ifndef BASE_H
#define BASE_H

#include <cassert>
#include <cstddef>
#include <iostream>

namespace KaliMatrix::Internal
{
  template<typename Derived> struct Traits;

  using std::size_t;

  /**
   * @brief Base class for all
   *
   *
   * @tparam Derived
   */
  template<typename Derived> struct Base {
    using T = typename Traits<Derived>::Type;

    constexpr const Derived &derived() const
    {
      return *static_cast<const Derived *>(this);
    }

    [[nodiscard]] constexpr size_t rows() const
    {
      return this->derived().rows();
    }

    [[nodiscard]] constexpr size_t cols() const
    {
      return this->derived().cols();
    }

    [[nodiscard]] constexpr T operator()(size_t i, size_t j) const
    {
      return this->derived()(i, j);
    }

  private:
    friend Derived;
    Base() = default;
  };

  template<typename Derived>
  constexpr std::ostream &operator<<(std::ostream &out, const Base<Derived> &b)
  {
    for (size_t i = 0; i < b.rows(); i++) {
      for (size_t j = 0; j < b.cols(); j++) {
        out << b(i, j) << ' ';
      }
      out << '\n';
    }

    return out;
  }

  template<typename Derived> struct ArrayBase {
    using T = typename Traits<Derived>::Type;

    constexpr const Derived &derived() const
    {
      return *static_cast<const Derived *>(this);
    }

    constexpr Derived &derived() { return *static_cast<Derived *>(this); }

    constexpr size_t size() const { return derived().size_; }

    constexpr const T &operator()(size_t idx) const
    {
      assert(idx <= derived().size_);
      return derived().arr[idx];
    }

    T &operator()(size_t idx)
    {
      assert(idx <= derived().size_);
      return derived().arr[idx];
    }

  private:
    ArrayBase() = default;
    friend Derived;
  };

};  //namespace KaliMatrix::Internal

#endif
