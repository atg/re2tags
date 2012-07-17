#import <string>

namespace rtt {
    
struct Tag {
    std::string name;     // e.g. hash_map
    std::string parents;  // e.g. std::tr1
    std::string kind;     // e.g. class
    std::string kindPath; // e.g. namespace::namespace
    
    long indentation; // In spaces
    std::string lineContent; // Content of the line in which it was found
    
    long line;
};

}