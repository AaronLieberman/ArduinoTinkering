#pragma once

#undef min
#undef max
//#undef abs

// namespace std {
// void __throw_bad_alloc();
// void __throw_length_error(char const* e);
// }  // namespace std

// #include <cstddef>
#include <vector>
#include <memory>
// #include <type_traits>
// #include <utility>

// namespace std {
// template <class T>
// struct _Unique_if {
//     typedef unique_ptr<T> _Single_object;
// };

// template <class T>
// struct _Unique_if<T[]> {
//     typedef unique_ptr<T[]> _Unknown_bound;
// };

// template <class T, size_t N>
// struct _Unique_if<T[N]> {
//     typedef void _Known_bound;
// };

// template <class T, class... Args>
// typename _Unique_if<T>::_Single_object make_unique(Args&&... args) {
//     return unique_ptr<T>(new T(std::forward<Args>(args)...));
// }

// template <class T>
// typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {
//     typedef typename remove_extent<T>::type U;
//     return unique_ptr<T>(new U[n]());
// }

// template <class T, class... Args>
// typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;
// }  // namespace std

std::string StringJoin(const std::vector<std::string> &elems, const std::string &separator = ", ");
