#import <stdlib.h>
#import <stdio.h>
#import "re2tags.hpp"

int main(int argc, char *argv[]) {
    
//    RE2 r2("xyz");
//    printf("? %d\n", r2.Match("abxyzab", 0, 7, RE2::UNANCHORED, NULL, 0));
    
    std::vector<rtt::Tag> tags =
        rtt::tagsForFile("/Users/alexgordon/Temporary/re2tags/defs/compiled.json", "foo.py", "class Foo\n\tdef bar():\n\t\tpass\n\n");
    
    for (rtt::Tag tag : tags) {
        printf("Found [%s] %s\n", tag.kind.c_str(), tag.name.c_str());
    }
    
    
    return 0;
}
