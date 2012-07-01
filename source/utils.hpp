#import <string>
#import <vector>

template<typename T>
void fast_stack_malloc(const size_t n, std::function<void(T*)> f);

void split_and_trim_into(std::string str, std::string delim, std::vector<std::string> into);