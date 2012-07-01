#import <string>
#import <vector>
#import "re2.h"
#import "jsoncpp.h"

namespace rtt {
    
    RE2 cachedRegexForString(std::string str);
    
    struct SymbolDef {
        std::string regex;
        std::string scoped;
    };
    struct Language {
        
        std::vector<std::string> extensions;
        std::vector<SymbolDef> symbols;
        
        Language(Json::Value j);
    };
}