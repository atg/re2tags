#import "re2.h"
#import "parser.hpp"
#import "utils.hpp"

namespace rtt {

void Parser::parseFile() {
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
    for (__block SymbolDef sym : language.symbols) {
        // Check that the scope is specified
        if (sym.scoped.size()) {
            if (scopeStack.back().kind != sym.scoped)
                continue;
        }
        
        bool shouldContinue = false;
//        __block Parser& me = *this;
        int ngroups = sym.regex().NumberOfCapturingGroups();
        
        fast_stack_malloc<re2::StringPiece>(ngroups, //^ void (re2::StringPiece* groups) {//
            [&](re2::StringPiece* groups) {
            
            // Just match on the line itself, it's faster
            if (!sym.regex().Match(piece, 0, lineLength, RE2::UNANCHORED, groups, ngroups)) {
                shouldContinue = true;
                return;
            }
            
            // Does it have a "name" group ?
            auto namedGroups = sym.regex().NamedCapturingGroups();
            if (namedGroups.count("name")) {
                names.push_back(groups[namedGroups["name"]]);
            }
            else if (namedGroups.count("names")) {
                split_and_trim_into(groups[namedGroups["names"]], ",", names);
            }
            else {
                shouldContinue = true;
                return;
            }
        });
        
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

}