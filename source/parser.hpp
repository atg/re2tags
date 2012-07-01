#import <string>
#import <vector>
#import "definition.hpp"
#import "tag.hpp"

namespace rtt {
    
struct ScopePart {
    std::string name;
    std::string kind;
    long indentation;
};

struct Parser {
    
    Language language;
    std::string content;
    
    std::vector<Tag> tags;
    std::deque<ScopePart> scopeStack;
    
    void parseFile();
    void parseLine(size_t lineOffset);
    
    void produceTag(Tag t);
};

}