#ifndef DCCT_TRAITS_HH
#define DCCT_TRAITS_HH
#include <type_traits>
/** @file traits.hh trait implementation for C++ */

/** Defines a Function for a Trait */
#define Fn(type, name, args) \
  std::is_member_function_pointer<decltype(&T::name)>::value && \
  std::is_same<decltype(&T::name), type (T::*) args>::value &&

/** Defines a Trait with member Functions */
#define Trait(name, Fns) \
template <typename T> \
struct name { \
  static constexpr bool value = ( \
    Fns \
    true \
  ); \
};

/** Example of Trait
```
Trait(IterativeEngine, 
  Fn(void, post_initialize, ())
  Fn(void, pre_compute_y, ())
  Fn(void, compute_y, ())
  Fn(void, post_compute_x, ())
)
```
*/

/** Expects trait implamantation */
#define ImplTrait(Type, Trait) \
  static_assert(Trait<Type>::value, #Type " should implement trait " #Trait)
#endif//DCCT_TRAITS_HH