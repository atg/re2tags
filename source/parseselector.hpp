#import "utils.hpp"



enum IgnoringStage {
    IgnoringStage_Start = 4, // *-* (T)
    
    IgnoringStage_NotIgnoring = 0, // *foo* : (int) bar
    IgnoringStage_Colon = 1,       // foo *:* (int) bar
    IgnoringStage_Brackets = 2,    // foo : *(int)* bar
    IgnoringStage_Variable = 3,    // foo : (int) *bar*
};

static bool isident(char c) {
    return isalnum(c) || c == '_' || c == '$';
}

static std::string selectorFromMethodDeclaration(std::string decl) {
    // Trim whitespace
    decl = trim(decl);
    
    if (decl.size() == 0)
        return std::string();
    
    // Determine if this method is class or instance
    bool isClassMethod = false;
    if (decl[0] == '+')
        isClassMethod = true;
    else if (decl[0] != '-')
        return std::string(); // Not a method decl
    
    // Find colon
    std::string sel;
    if (isClassMethod)
        sel.push_back('+');
    else
        sel.push_back('-');
    
    IgnoringStage ignoringStage = IgnoringStage_Start;
    
    bool isStart = true;
    long bracketCount = 0;
    long variableLength = 0;
    for (long i = 0, len = decl.size(); i < len; i++) {
        char c = decl[i];
        char next_c = i + 1 < len ? decl[i + 1] : '\0';
        
        if (c == '/' && next_c == '/') {
            // Oops it's a comment
            for (; i < len; i++) {
                char c1 = decl[i];
                char next_c1 = i + 1 < len ? decl[i + 1] : '\0';
                if (c1 == '\n' || (c1 == '\r' && next_c1 != '\n'))
                    break;
            }
            continue;
        }
        
        if (c == '/' && next_c == '*') {
            i += 2; // Don't allow /*/
            
            // Oops it's a comment
            for (; i < len; i++) {
                char c1 = decl[i];
                char next_c1 = i + 1 < len ? decl[i + 1] : '\0';
                if (c1 == '*' && next_c1 == '/')
                    break;
            }
            i++;
            continue;
        }
        
        if (ignoringStage == IgnoringStage_Start) {
//            printf("@start\n");
            
            if (isspace(c))
                continue;
            
            if (c == '-' || c == '+')
                continue;
            
            if (c == '(') {
                bracketCount++;
                ignoringStage = IgnoringStage_Brackets; // printf("->brackets\n");
                continue;
            }
            
            if (isident(c)) {
                ignoringStage = IgnoringStage_NotIgnoring; // printf("->selector\n");
                isStart = false;
                i--;
                continue;
            }
            
            return std::string(); // Not a method
        }
        if (ignoringStage == IgnoringStage_NotIgnoring) {
//            printf("@selector\n");
            
            variableLength = 0;
            
            if (isspace(c))
                continue;
            if (isident(c)) {
                sel.push_back(c);
                continue;
            }
            if (c == ':') {
                sel.push_back(c);
                ignoringStage = IgnoringStage_Colon; // printf("->colon\n");
                continue;
            }
            
            return std::string(); // Not a method
        }
        else if (ignoringStage == IgnoringStage_Colon) {
            // printf("@colon\n");
            
            if (isspace(c))
                continue;
            if (c == '(') {
                bracketCount++;
                ignoringStage = IgnoringStage_Brackets; // printf("->brackets\n");
                continue;
            }
            if (isident(c)) {
                ignoringStage = IgnoringStage_Variable; // printf("->variable\n");
                i--; // Redo this loop as a variable
                continue;
            }
            
            return std::string(); // Not a method
        }
        else if (ignoringStage == IgnoringStage_Brackets) {
//            printf("@brackets\n");
            
            if (c == '(') {
                bracketCount++;
                continue;
            }
            if (c == ')') {
                bracketCount--;
                if (bracketCount == 0) {
                    if (isStart) {
                        ignoringStage = IgnoringStage_NotIgnoring; // printf("->selector\n");
                        isStart = false;
                    }
                    else {
                        ignoringStage = IgnoringStage_Variable; // printf("->variable\n");
                    }
                }
                continue;
            }
            
            continue;
        }
        else if (ignoringStage == IgnoringStage_Variable) {
//            printf("@variable\n");
            
            if (isident(c)) {
                variableLength++;
                continue;
            }
            else {
                if (variableLength == 0)
                    return std::string(); // Not a method. Needs an argument
            }
            
            if (isspace(c)) {
                ignoringStage = IgnoringStage_NotIgnoring; // printf("->selector\n");
                continue;
            }
            
            if (c == ':') {
                sel.push_back(c);
                ignoringStage = IgnoringStage_Colon; // printf("->colon\n");
                continue;
            }
            
            return std::string(); // Not a method. Needs an argument
        }
    }
    return sel;
}