#ifndef MACRO_PROCESSOR_H
#define MACRO_PROCESSOR_H

#include <map>
#include "ivisitor.h"
#include "dllexer.h"
#include "macro.h"

class MacroProcessor : public IVisitor {
    protected:
        std::map<std::string,Macro*>& macro_registry;
    public:
        MacroProcessor(std::map<std::string,Macro*>& macros);
    private:
        void beforeVisit(AST* cur, int depth);
        void afterVisit(AST* cur, int depth);
        void beforeChildren(AST* cur, int depth);
        void afterChildren(AST* cur, int depth);
        void beforeChild(AST* cur, int depth);
        void afterChild(AST* cur, int depth);
};

#endif
