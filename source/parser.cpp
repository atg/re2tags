#import "re2.h"
#import "parser.hpp"
#import "utils.hpp"

namespace rtt {

void Parser::parseFile() {
//    RE2 r3("abcbawiudj");

    size_t len = content.size();
    size_t lastLineStart = 0;
    size_t lastLineLength = 0;
    for (size_t i = 0; i < len; i++) {
        char c = content[i];
        
        if (c == '\0')
            return;
        
        if (c == '\r' || c == '\n') {
            
            parseLine(lastLineStart, lastLineLength);
            
            lastLineStart = i;
            lastLineLength = 0;
        }
        else {
            lastLineLength++;
        }
    }
}
void Parser::parseLine(size_t lineOffset, size_t lineLength) {
    
    if (!lineLength)
        return;

    // Find the indentation
    size_t len = content.size();
    long indent = 0;
    for (size_t i = lineOffset; i < len; i++) {
        char c = content[i];
        if (c == ' ')
            indent += 1;
        else if (c == '\t')
            indent += 4;
        else
            break;
    }
    
    Tag t;
    t.indentation = indent;
    
    const re2::StringPiece piece = re2::StringPiece(content.c_str() + lineOffset, lineLength);
    
    std::vector<std::string> names;
    bool hadMatch = false;
//    const re2::StringPiece piece2("abc");

//    printf("LANG: /%ul/\n", (void*)&language);
    printf("LANGNAME: /%s/\n", language.name.c_str());
//    language.debug();
    for (SymbolDef sym : language.symbols) {
        // Check that the scope is specified
//        printf("sym.scoped = %lu\n", *(unsigned long*)&(sym.scoped));
//        sym.debug();
        if (sym.scoped.size()) {
            bool foundMatchingScope = false;
            if (scopeStack.size()) {
                for (std::string scope : sym.scoped) {
                    if (scopeStack.back().kind == scope) {
                        foundMatchingScope = true;
                        break;
                    }
                }
            }
            if (!foundMatchingScope)
                continue;
        }

        bool shouldContinue = false;
//        __block Parser& me = *this;
//        std::string sourcestring = sym.sourceRegex;
//        const re2::StringPiece piece("abc");
//        RE2 r2(piece2);
        int ngroups = 0;//.NumberOfCapturingGroups();
//        int ngroups = sym.regex()->NumberOfCapturingGroups();
//        sym.debug();
//        continue;

//        fast_stack_malloc<re2::StringPiece>(ngroups, //^ void (re2::StringPiece* groups) {//
//            [&](re2::StringPiece* groups) {
        re2::StringPiece* groups = new re2::StringPiece[ngroups];
//        std::string copystring(sym.sourceRegex.c_str());
//        RE2 rxx(copystring);
//        sym.debug();
//        continue;
            // Just match on the line itself, it's faster
            if (!sym.regex()->Match(piece, 0, lineLength, RE2::UNANCHORED, groups, ngroups)) {
//                shouldContinue = true;
//                return;
                delete[] groups;
                continue;
            }
        printf("NEXT WILL BE SECOND SYMBOL\n");
        sym.debug();
            // Does it have a "name" group ?
            auto namedGroups = sym.regex()->NamedCapturingGroups();
            if (namedGroups.count("name")) {
                names.push_back(groups[namedGroups["name"]].as_string());
            }
            else if (namedGroups.count("names")) {
                std::string namesstring = "ABC";//groups[namedGroups["names"]].as_string();
                split_and_trim_into(namesstring, std::string(","), names);
            }
            else {
//                shouldContinue = true;
//                return;
                delete[] groups;
                continue;

            }
//        });
        delete[] groups;
        
        if (shouldContinue || !names.size())
            continue;
        
        t.kind = sym.kind;
        
        hadMatch = true;
        break;
    }
    
    if (!hadMatch)
        return;
    
    t.lineContent = content.substr(lineOffset, lineLength);
    
    for (ScopePart part : scopeStack) {
        t.parents.append(part.name);
        t.parents.append("::");
        t.kindPath.append(part.kind);
        t.kindPath.append("::");
    }
    
    for (std::string name : names) {
        t.name = name;
        produceTag(t);
    }
}
    
void Parser::produceTag(Tag t) {
    tags.push_back(t);
}

}