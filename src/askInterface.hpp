#ifndef _SRC_ASKINTERFACE_
#define _SRC_ASKINTERFACE_

#include <string>
#include <vector>

#include "optional.hpp"

namespace crdy::internal {


std::string ask(std::string question, std::vector<std::string> validOpts = {},
	  crdy::internal::Optional<std::string> defVal = {}, bool caseSensitive = false);
	
}
#endif // !_SRC_ASKINTERFACE_
