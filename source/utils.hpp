#import <string>
#import <vector>
#import <functional>

//template<typename T>
// Clang doesn't seem to support std::function yet, so use clang blocks instead
//void fast_stack_malloc(const size_t n, void(^f)(T*));
//void fast_stack_malloc(const size_t n, std::function<void(T*)> f);

void split_and_trim_into(std::string str, std::string delim, std::vector<std::string> into);
bool string_ends_with(std::string &haystack, std::string &needle);
