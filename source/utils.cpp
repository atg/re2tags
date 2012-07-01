template<typename T>
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
        f(p)
        delete T[];
    }
}

void split_and_trim_into(std::string str, std::string delim, std::vector<std::string> into) {



}