#pragma once

#undef min
#undef max

namespace std
{
	void __throw_bad_alloc();
	void __throw_length_error(char const* e);
}
