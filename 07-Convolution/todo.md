## Eigen inspired Matrix library

Compile time polymorphism using CRTP

### Traits

- ???

### Base.h

- Base class with simple accessor methods
- Methods: none, just interface

### StorageBase.h

- Storage Class, uses arrays to store data
  - Static: Stack based allocation. Resize not allowed
  - Dynamic: Start with zero and allocate based on size. Custom array wrapper
    with constructor/destructor
- Methods: Constructor, Destructor, Copy using swap, resize, rows, cols, size,
  data ptr
- Use Pimpl idiom to abstract away implementation details and inherit Storage
  class from it

### Object class
