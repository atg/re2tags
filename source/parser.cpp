#import "re2.h"

namespace rtt {


void parser::parseFile() {
    size_t len = content.size();
    size_t lastLineStart = 0;
    size_t lastLineLength = 0;
    for (size_t i = lineOffset; i < len; i++) {
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
void parser::parseLine(size_t lineOffset, size_t lineLength) {
    
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
    t.lineContent = content.substr(lineOffset, lineLength);
    
    bool hadMatch = false;
    for (SymbolDef sym : language.symbols) {
        // Check that the scope is specified
        if (sym.scoped.size()) {
            if (scopeStack.back() != sym.scoped)
                continue;
        }
        
        bool shouldContinue = false;
        int ngroups = sym.regex.NumberOfCapturingGroups();
        std::vector<std::string> names;
        fast_stack_malloc<StringPiece>(ngroups, [&](StringPiece* groups) {
            
            // Just match on the line itself, it's faster
            if (!sym.regex.Match(lineContent, sym.regex, 0, lineLength, UNANCHORED, groups, ngroups)) {
                shouldContinue = true;
                return;
            }
            
            // Does it have a "name" group ?
            auto namedGroups = sym.regex.NamedCapturingGroups();
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
        
        hadMatch = true;
        break;
    }
    
    produceTag(t);
}

}