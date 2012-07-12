#import "definition.hpp"

RE2* cachedRegexForString(std::string str) {
    static std::map<std::string, std::unique_ptr<RE2>> cache;
    if (cache.count(str))
        return cache[str].get();
    
    cache[str] = std::unique_ptr<RE2>(new RE2(str));
    return cache[str].get();
}
