#include <iostream>

#include "../include/Matrix/Matrix.hpp"

void get_size(const auto &m)
{
  std::cout << "Size: " << m.rows() << "x" << m.cols() << "\n\n";
}

int main()
{
  KaliMatrix::Matrix3i m_static;
  m_static(0, 0) = 1;
  m_static(1, 1) = 2;
  m_static(2, 2) = 3;
  std::cout << m_static;
  get_size(m_static);

  KaliMatrix::MatrixXd m_dynamic(2, 2);
  m_dynamic(0, 0) = 1;
  m_dynamic(1, 1) = 2;
  std::cout << m_dynamic;
  get_size(m_dynamic);

  m_dynamic.cons_resize(4, 4);
  std::cout << m_dynamic;
  get_size(m_dynamic);
  std::cout << "Matrix Test passed succesfully\n";
}
