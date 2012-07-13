#import <string>
#import <vector>
#import <fstream>
#import "re2.h"
#import "json.h"
#import <iostream.h>

namespace rtt {
    
RE2* cachedRegexForString(std::string str);

struct SymbolDef {
    std::string kind;
    std::string sourceRegex;
    std::vector<std::string> scoped;
    
    RE2* _regex;
    SymbolDef() : kind(), sourceRegex(), scoped(), _regex(0) {
        regex();
        printf("_regex = %lu\n", _regex);
    }
    
    RE2* regex() {
//        return NULL;
        if (_regex) return _regex;
        return _regex = new RE2(sourceRegex);
//        return cachedRegexForString(sourceRegex);
        printf("sourceRegex.c_str() = %lu\n", sourceRegex.c_str());
        printf("@@regex = %s\n", sourceRegex.c_str());
        printf("@regex = %lu\n", _regex);
//        if (_regex)
//            return *_regex;
        
        _regex = cachedRegexForString(sourceRegex);
        return _regex;
    }
    
    void debug() {
        printf("    SYMBOL: [%s] %s\n", kind.c_str(), sourceRegex.c_str());
    }
};

struct Language {
    
    std::string name;
    std::vector<std::string> extensions;
    std::vector<SymbolDef> symbols;
    
    Language(Json::Value& j);
    
    void debug() {
        printf("  LANGUAGE: %s\n", name.c_str());
        for (SymbolDef& symbol : symbols) {
            symbol.debug();
        }
    }
};

struct Manager {
    std::vector<Language> langs;
    static Manager getManager(std::string path_to_definitions);
    Manager(std::string path_to_definitions);
    
    Language* detectLanguage(std::string path, std::string content);
    
    void debug() {
        printf("MANAGER:\n");
        for (Language& lang : langs) {
            lang.debug();
        }
    }
};

}