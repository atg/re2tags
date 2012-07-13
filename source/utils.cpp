#import "utils.hpp"
/*
template<typename T>
// Clang crashes on std::function, so until then we use macros
void fast_stack_malloc(const size_t n, std::function<void(T*)> f) {
    
    // 128 bytes is a reasonable amount to allocate on the stack
    const size_t bytes = sizeof(T) * n;
    if (bytes <= 128) {
        
        T* const p0 = alloca(bytes);
        T* const p = new (p0) T[n];
        
        f(p);
        
        for (size_t i = 0; i < n; i++) {
            p[i].~T();
        } 
    }
    else {
        // Too big! Put it on the heap
        T* const p = new T[n]; 
        f(p);
        delete[] p;
    }
}*/
/*
#define fast_stack_malloc(T, n, b) do { \
const size_t bytes = sizeof(T) * n; \
if (bytes <= 128) { \
T* const p0 = alloca(bytes); \
T* const p = new (p0) T[n]; \
b \
for (size_t i = 0; i < n; i++) { p[i].~T(); } \
} \
else { \
T* const p = new T[n]; \
f(p); \
delete[] p; \
} \
} while(0)
*/

bool string_ends_with(std::string &haystack, std::string &needle) {
    if (haystack.length() < needle.length())
        return false;
    
    return 0 == haystack.compare(haystack.size() - needle.size(), needle.size(), needle);
}

void split_and_trim_into(std::string str, std::string delim, std::vector<std::string> into) {

    

}