#ifndef _SRC_OPTIONAL_
#define _SRC_OPTIONAL_

#include "errors.hpp"

namespace crdy::internal {
template<typename T>
class Optional {
public:
	Optional(T val) : m_value(val), m_hasValue(true) { }
	Optional() : m_value{}, m_hasValue(false) { } 
	T getValue() const {
		if(!m_hasValue) throw INTERNAL_ERROR("Trying to acces an invalid value of crdy::Optional\n");
		return m_value;
	}
	bool isValid() const { return m_hasValue; }
private:
	bool m_hasValue;
	T m_value;
};
}
#endif // !_SRC_OPTIONAL_
