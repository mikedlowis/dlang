#ifndef DLPARSER_H
#define DLPARSER_H

#include <map>
#include "btparser.h"
#include "dllexer.h"
#include "syntax.h"

class DLParser : public BTParser
{
    protected:
        std::map<std::string,eTokenTypes> core_forms;
        std::map<std::string,Syntax*> syntaxes;
    public:
        DLParser();
        ~DLParser();
        bool isSyntaxName(void);
        bool isCoreFormName(void);
        eTokenTypes getCoreFormId(void);
        void parse(void);

        // Entry Rules
        AST* Program(void);
        AST* Expression(void);
        AST* CoreForm(void);
        AST* FuncApp(void);
        AST* BasicExp(void);
        AST* Literal(void);
        AST* ExpList(eTokenTypes term);
        AST* IdList(void);
};

#endif
