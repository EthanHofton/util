#ifndef UTIL_MEMORY_HPP
#define UTIL_MEMORY_HPP

#include <memory>

namespace util {

template<typename T>
using scope = std::unique_ptr<T>;
template<typename T, typename... args>
constexpr scope<T> create_scope(args&&... t_args) {
    return std::make_unique<T>(std::forward<args>(t_args)...);
}

template<typename T>
using ref = std::shared_ptr<T>;
template<typename T, typename... args>
constexpr ref<T> create_ref(args&&... t_args) {
    return std::make_shared<T>(std::forward<args>(t_args)...);
}

template<typename T>
using weak_ref = std::weak_ptr<T>;

}

#endif