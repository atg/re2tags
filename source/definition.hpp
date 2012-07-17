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
    
    public:
    SymbolDef(std::string k, std::string src, std::vector<std::string> scp) : kind(k), sourceRegex(src), scoped(scp), _regex(0) {
//        regex();
    }
    
    RE2* regex() {
        if (_regex)
            return _regex;
        return _regex = new RE2(sourceRegex);
    }
    
    void debug() {
        printf("    SYMBOL: [%s] %s\n", kind.c_str(), sourceRegex.c_str());
    }
};

struct Language {
    
    std::string name;
    std::vector<std::string> extensions;
    std::vector<SymbolDef> symbols;
    
    Language() : name(), extensions(), symbols() { }
    Language(Json::Value& j);
    
    void debug() {
        printf("  LANGUAGE: %s\n", name.c_str());
        for (std::string& ext : extensions) {
            printf("    EXTENSION: [%s]\n", ext.c_str());
        }
        for (SymbolDef& symbol : symbols) {
            symbol.debug();
        }
    }
};

struct Manager {
    std::vector<Language> langs;
    static Manager getManager(std::string path_to_definitions);
    Manager(std::string path_to_definitions);
    
    Language detectLanguage(std::string path, std::string& content);
    
    void debug() {
        printf("MANAGER:\n");
        for (Language& lang : langs) {
            lang.debug();
        }
    }
};

}