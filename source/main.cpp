#import <stdlib.h>
#import <stdio.h>
#import "re2tags.hpp"

#include <string>
#include <fstream>
#include <streambuf>

#import "parseselector.hpp"

int main(int argc, char *argv[]) {
    
//    printf("%s\n", selectorFromMethodDeclaration("- (T)abc:abc:(id)abc :(T)abc").c_str());
    
    if (argc < 2) {
        printf("No file specified.\n");
        return 1;
    }
    
    std::ifstream t(argv[1]);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    
    std::vector<rtt::Tag> tags = rtt::tagsForFile("/Users/alexgordon/Temporary/re2tags/defs/re2tags.json", argv[1], str);
    
    for (rtt::Tag tag : tags) {
        printf("[%s] %s%s\n", tag.kind.c_str(), tag.parents.c_str(), tag.name.c_str());
    }
    
    return 0;
}
