#import "definition.hpp"
#import "utils.hpp"
#import <stdio.h>

namespace rtt {
/*
RE2* cachedRegexForString(std::string str) {
    static std::map<std::string, RE2*> cache;
    if (cache.count(str))
        return cache[str];
    
    RE2* compiledRegex = new RE2(str);
    cache[str] = compiledRegex;
    return cache[str];
}
 */

static std::vector<std::string> JsonStringListToCPPStringList(Json::Value& j) {
    std::vector<std::string> strs;
    for (Json::Value& strj : j) {
//    for (Json::Value::iterator it = j.begin(), et = j.begin(); it != et; ++it) {
//        Json::Value& strj = *it;
        
        strs.push_back(strj.asString());
    }
    return strs;
}
Language::Language(Json::Value& j) : symbols(), name(), extensions() {
    extensions = JsonStringListToCPPStringList(j["exts"]);
    
    for (Json::Value& symbolj : j["symbols"]) {
//    Json::Value& syms = j["symbols"];
//    for (Json::Value::iterator it = syms.begin(), et = syms.begin(); it != et; ++it) {
//        Json::Value& symbolj = *it;
        
        std::vector<std::string> scoped;
        if (symbolj.isMember("scope")) {
            Json::Value& scopej = symbolj["scope"];
            scoped = JsonStringListToCPPStringList(scopej);
        }
        
        SymbolDef symdef = SymbolDef(
            symbolj["kind"].asString(),
            symbolj["regex"].asString(),
            scoped
        );
        
        symbols.push_back(symdef);
    }
}

Manager Manager::getManager(std::string path_to_definitions) {
    static Manager m(path_to_definitions);
    return m;
}
Manager::Manager(std::string path_to_definitions) : langs() {
    std::ifstream stream(path_to_definitions.c_str(), std::ifstream::in);
    Json::Reader reader;
    
    Json::Value root;
    reader.parse(stream, root);
    
    for (std::string langname : root.getMemberNames()) {
        Language lang(root[langname]);
        lang.name = langname;
        langs.push_back(lang);
    }
}
Language Manager::detectLanguage(std::string path, std::string& content) {
    
    for (Language& lang : langs) {
        for (std::string& ext : lang.extensions) {
            std::string dot_ext = std::string(".") + ext;
            if (string_ends_with(path, dot_ext))
                return lang;
        }
    }
    
    for (Language& lang : langs) {
        if (lang.name == path) {
            return lang;
        }
    }
    
    return Language();
}

}