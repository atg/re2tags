#import "re2tags.hpp"

std::vector<rtt::Tag> rtt::tagsForFile(std::string definitionspath, std::string path, std::string content) {
    
    Language lang = Manager::getManager(definitionspath).detectLanguage(path, content);
    
    if (lang.name.size()) {
        
        for (int i = 0; i < 10000; i++) {
            rtt::Parser parser(lang, content);
            parser.parseFile();
        }
//        return parser.tags;
    }
    
    return std::vector<rtt::Tag>();
}
