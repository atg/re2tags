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
    for (SymbolDef sym : language.symbols) {
        // Check that the scope is specified
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
        RE2 r(sym.sourceRegex);
        int ngroups = r.NumberOfCapturingGroups() + 1;

//        fast_stack_malloc<re2::StringPiece>(ngroups, //^ void (re2::StringPiece* groups) {//
//            [&](re2::StringPiece* groups) {
        re2::StringPiece* groups = new re2::StringPiece[ngroups];
        
            // Just match on the line itself, it's faster
            if (!r.Match(piece, 0, lineLength, RE2::UNANCHORED, groups, ngroups)) {
                delete[] groups;
                continue;
            }
        
            // Does it have a "name" group ?
            const std::map<std::string, int>& namedGroups = r.NamedCapturingGroups();
            if (namedGroups.count("name")) {
                int groupidx = namedGroups.find("name")->second;
                names.push_back(groups[groupidx].as_string());
            }
            else if (namedGroups.count("names")) {
                int groupidx = namedGroups.find("names")->second;
                std::string namesstring = groups[groupidx].as_string();
                
                split_and_trim_into(namesstring, std::string(","), names);
            }
            else {
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