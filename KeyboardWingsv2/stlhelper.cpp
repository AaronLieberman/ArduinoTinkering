#include "stlhelper.h"

#include <Arduino.h>

#include <numeric>
#include <vector>

// namespace std {
// void __throw_bad_alloc() {
//     Serial.println("Unable to allocate memory");
// }

// void __throw_length_error(char const* e) {
//     Serial.print("Length Error :");
//     Serial.println(e);
// }
// }  // namespace std

std::string StringJoin(const std::vector<std::string> &elems, const std::string &separator) {
    return std::accumulate(std::next(elems.begin()), elems.end(), elems[0],
        [&separator](std::string a, std::string b) { return a + separator + b; });
}
