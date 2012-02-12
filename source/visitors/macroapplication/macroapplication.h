#ifndef MacroApplication_H
#define MacroApplication_H 

#include "visitor.h"
#include <iostream>
#include <sstream>
#include "param.h"

class MacroApplication : public Visitor {
	protected:
		AST* mod_ast;
		ostringstream stream;
		std::list<Param*> macro_params;
	public:
		MacroApplication(AST* root,std::list<Param*>& params) : Visitor(root), mod_ast(root), macro_params(params) {};
		Param* getParamByName(std::string name);
		AST* getAST(void);
	private:
		void beforeVisit(AST* cur, int depth);
		void afterVisit(AST* cur, int depth);
		void beforeChildren(AST* cur, int depth);
		void afterChildren(AST* cur, int depth);
		void beforeChild(AST* cur, int depth);
		void afterChild(AST* cur, int depth);
};

#endif