#import <string>
#import <vector>
#import "re2.h"
#import "json.h"

namespace rtt {
    
RE2* cachedRegexForString(std::string str);

struct SymbolDef {
    std::string kind;
    std::string sourceRegex;
    std::string scoped;
    
    RE2* _regex;
    
    RE2& regex() {
        if (_regex)
            return *_regex;
        
        _regex = cachedRegexForString(sourceRegex);
        return *_regex;
    }
};
struct Language {
    
    std::vector<std::string> extensions;
    std::vector<SymbolDef> symbols;
    
    Language(Json::Value j);
};

}