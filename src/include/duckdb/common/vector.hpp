//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/vector.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/assert.hpp"
#include "duckdb/common/constants.hpp"
#include <vector>

namespace duckdb {

// TODO: inline this, needs changes to 'exception.hpp' and other headers to avoid circular dependency
void AssertIndexInBounds(idx_t index, idx_t size);

template <class _Tp, class _Allocator = std::allocator<_Tp>>
class vector : public std::vector<_Tp, _Allocator> {
public:
	using original = std::vector<_Tp, _Allocator>;
	using original::original;
	using size_type = typename original::size_type;
	using const_reference = typename original::const_reference;
	using reference = typename original::reference;

	// TODO: add [[ clang:reinitializes ]] attribute
	inline void clear() noexcept {
		original::clear();
	}

	typename original::reference operator[](typename original::size_type __n) {
#ifdef DEBUG
		AssertIndexInBounds(__n, original::size());
#endif
		return original::operator[](__n);
	}
	typename original::const_reference operator[](typename original::size_type __n) const {
#ifdef DEBUG
		AssertIndexInBounds(__n, original::size());
#endif
		return original::operator[](__n);
	}
};

} // namespace duckdb
