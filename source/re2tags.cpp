#import "re2tags.hpp"

std::vector<rtt::Tag> rtt::tagsForFile(std::string definitionspath, std::string path, std::string content) {
    
    Language* lang = Manager::getManager(definitionspath).detectLanguage(path, content);
    Manager::getManager(definitionspath).debug();
    printf("lang = %lu\n", lang);
    rtt::Parser parser(*lang, content);
    
    parser.parseFile();
    
    return parser.tags;
}
