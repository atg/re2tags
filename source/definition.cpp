#import "definition.hpp"

RE2 cachedRegexForString(std::string str) {
    static std::map<std::string, RE2> cache;
    if (cache.map.count(str))
        return cache[str];
    
    cache[str] = RE2(str);
    return cache[str];
}
