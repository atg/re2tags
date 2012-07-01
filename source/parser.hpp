#import <string>
#import <vector>
#import "definition.hpp"
#import "tag.hpp"

namespace rtt {
    
    struct Parser {
        
        Language language;
        std::string content;
        
        std::vector<Tag> tags;
        
        void parseFile();
        void parseLine(size_t lineOffset);

        void produceTag(Tag t);
    };
}