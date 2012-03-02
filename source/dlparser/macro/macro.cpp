#include "macro.h"
#include "macroapplication.h"
#include "cork.h"

using namespace std;

Macro::Macro(AST* macro_def)
{
    list<AST*>::iterator it = macro_def->children()->begin();

    // Set Name
    macro_name = (*it++)->text();

    // Set Body
    setUpParamList( *it++ );

    // Set Params
    macro_body = (*it++)->clone();

}

Macro::~Macro()
{

    std::list<Param*>::iterator iter;
    for (iter = macro_params.begin(); iter != macro_params.end(); ++iter) {
        delete *iter;
    }
    delete macro_body;
}

const std::string& Macro::name(void)
{
    return macro_name;
}

const std::list<Param*>& Macro::params(void)
{
    return macro_params;
}

AST* Macro::apply(void)
{
    MacroApplication application(macro_body->clone(), macro_params);
    application.visit();
    return application.getAST()->clone();
}

void Macro::setUpParamList( AST* param_tree )
{
    list<AST*>::iterator it = param_tree->children()->begin();
    for(; it != param_tree->children()->end(); it++)
    {
        macro_params.push_back( _new Param( *it ) );
    }
}
