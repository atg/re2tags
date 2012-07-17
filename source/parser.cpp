#import "re2.h"
#import "parser.hpp"
#import "utils.hpp"

namespace rtt {
    
void Parser::parseFile() {

    size_t len = content.size();
    size_t lastLineStart = 0;
    size_t lastLineLength = 0;
    long lineNumber = 0;
    for (size_t i = 0; i < len; i++) {
        char c = content[i];
        
        if (c == '\0')
            return;
        
        if (c == '\r' || c == '\n') {
            
            if (lastLineStart + lastLineLength <= len)
                parseLine(lastLineStart, lastLineLength, lineNumber);
            
            lastLineStart = i + 1;
            lastLineLength = 0;
        }
        else {
            lastLineLength++;
        }
    }
}

    
// Clang crashes when either C++ lambdas or C blocks are used, so we have to resort to a function for the moment

static void regexCallback(re2::StringPiece* groups, long ngroups, const re2::StringPiece& piece, long lineLength, bool& shouldContinue, SymbolDef& sym, std::vector<std::string>& names, std::string& parentIdentifier) {
    
    // Just match on the line itself, it's faster
    if (!sym.regex()->Match(piece, 0, lineLength, RE2::UNANCHORED, groups, ngroups)) {
        shouldContinue = true;
        return;
    }
    
    // Does it have a "name" group ?
    const std::map<std::string, int>& namedGroups = sym.regex()->NamedCapturingGroups();
    
    if (namedGroups.count("parent")) {
        int groupidx = namedGroups.find("parent")->second;
        parentIdentifier = groups[groupidx].as_string();
    }
    
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
        shouldContinue = true;
        return;
    }
}

void Parser::parseLine(size_t lineOffset, size_t lineLength, long lineNumber) {
    
    if (!lineLength)
        return;
    
    // Find the indentation
    long indent = 0;
    for (size_t i = lineOffset; i < lineOffset + lineLength; i++) {
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
    t.line = lineNumber;
    
    const re2::StringPiece piece = re2::StringPiece(content.c_str() + lineOffset, lineLength);
    
    while (scopeStack.size() && scopeStack.back().indentation >= indent) {        
        scopeStack.pop_back();
    }
    
    std::string parentIdentifier;
    std::vector<std::string> names;
    bool hadMatch = false;
    for (SymbolDef& sym : language.symbols) {
        
        // Check that the scope is specified
        if (sym.scoped.size()) {
            bool foundMatchingScope = false;
            if (sym.scoped.front() == std::string("ROOT")) {
                foundMatchingScope = scopeStack.size() == 0;
            }
            else if (scopeStack.size()) {
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
        int ngroups = sym.regex()->NumberOfCapturingGroups() + 1;
        
        // 128 bytes is a reasonable amount to allocate on the stack
        const size_t bytes = sizeof(re2::StringPiece) * ngroups;
        if (bytes <= 128) {
            
            re2::StringPiece* p0 = (re2::StringPiece*)alloca(bytes);
            re2::StringPiece* p = new (p0) re2::StringPiece[ngroups];
            
            regexCallback(p, ngroups, piece, lineLength, shouldContinue, sym, names, parentIdentifier);
            
            for (size_t i = 0; i < ngroups; i++) {
                p[i].~StringPiece();
            } 
        }
        else {
            // Too big! Put it on the heap
            re2::StringPiece* p = new re2::StringPiece[ngroups]; 
            regexCallback(p, ngroups, piece, lineLength, shouldContinue, sym, names, parentIdentifier);
            delete[] p;
        }        
        
        
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
    
    if (parentIdentifier.size()) {
        
        std::vector<std::string> parentParts;
        split_and_trim_into(parentIdentifier, ".:/\\", parentParts);
        for (std::string parentPart : parentParts) {
            t.parents.append(parentPart);
            t.parents.append("::");
            t.kindPath.append("misc");
            t.kindPath.append("::");

        }
    }
    
    for (std::string name : names) {
        t.name = name;
        produceTag(t);
    }
}

    
void Parser::produceTag(Tag t) {
    tags.push_back(t);
    
    ScopePart p;
    p.name = t.name;
    p.kind = t.kind;
    p.indentation = t.indentation;
    scopeStack.push_back(p);
}

}