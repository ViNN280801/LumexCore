#ifndef LUMEX_MACROS_HPP
#define LUMEX_MACROS_HPP

#include "lumex/core/utility/LumexAssert.hpp"

#define LUMEX_STRINGIZE_DETAIL(x) #x
#define LUMEX_STRINGIZE(x) LUMEX_STRINGIZE_DETAIL(x)

#define LUMEX_CONCAT_DETAIL(x, y) x##y
#define LUMEX_CONCAT(x, y) LUMEX_CONCAT_DETAIL(x, y)

#define LUMEX_ASSERT(cond) ((cond) ? static_cast<void>(0) : lumex_assert_handler(#cond, __FILE__, __LINE__))

#endif // !LUMEX_MACROS_HPP
