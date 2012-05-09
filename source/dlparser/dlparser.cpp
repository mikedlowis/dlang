#include "dlparser.h"
#include "exception.h"
#include "common.h"

using namespace std;

DLParser::DLParser() : BTParser()
{
    core_forms.insert( pair<string,eTokenTypes>("define", DEFINE) );
    core_forms.insert( pair<string,eTokenTypes>("set!",   ASSIGN) );
    core_forms.insert( pair<string,eTokenTypes>("lambda", LAMBDA) );
    core_forms.insert( pair<string,eTokenTypes>("begin",  BEGIN) );
    core_forms.insert( pair<string,eTokenTypes>("if",     IF) );
    core_forms.insert( pair<string,eTokenTypes>("macro",  MACRO) );
}

DLParser::~DLParser()
{
}

bool DLParser::isMacroName(void)
{
    return false;
}

bool DLParser::isCoreFormName(void)
{
    return false;
}

eTokenTypes DLParser::getCoreFormId(void)
{
    return (eTokenTypes)0;
}

void DLParser::parse(void)
{
    result = Program();
}

AST* DLParser::Program(void)
{
    AST* node = new AST( PROGRAM );
    while( lookaheadType(1) != EOF )
    {
        node->addChild( Expression() );
    }
    return node;
}

AST* DLParser::Expression(void)
{
    AST* ret = NULL;

    // Expression := CoreForm
    //             | BasicExp
    //
    // CoreForm := 'define' ID Expression TERM
    //           | 'set' ID Expression TERM
    //           | 'lambda' IdList ExpList? TERM
    //           | 'begin' ExpList* TERM
    //           | 'if' Expression Expression Expression? TERM
    //           | 'macro' ID IdList ID ExpList TERM
    //
    // BasicExp := MacroName ExpList? TERM
    //           | '(' Expression ID Expression ')'
    //           | ID '(' ExpList ')'
    //           | Literal
    //
    // Literal := ID
    //          | CHAR
    //          | SYMBOL
    //          | STRING
    //          | NUMBER
    //
    // ExpList := Expression+
    //
    // IdList := '(' ID* ')'
    //

    if( isCoreFormName() )
    {
        ret = CoreForm();
    }
    else
    {
        ret = Application();
    }

    return ret;
}

AST* DLParser::CoreForm(void)
{
    AST* ret = NULL;
    eTokenTypes form_id = getCoreFormId();
    consume(); // Throw away the form name (we don't need it anymore)
    switch( form_id )
    {
        case DEFINE:
        case ASSIGN:
            ret = new AST( lookaheadToken(1) );
            match(ID);
            ret = new AST(form_id, 2, ret, Expression());
            break;

        case LAMBDA:
            ret = new AST(LAMBDA, 2, IdList(), ExpList());
            break;

        case BEGIN:
            ret = new AST(BEGIN, 1, ExpList());
            break;

        case IF:
            ret = new AST(IF, 2, Expression(), Expression());
            if(lookaheadType(1) != RPAR)
            {
                ret->addChild( Expression() );
            }
            break;

        case MACRO:
            ret = new AST(MACRO);

            // Get the macro name
            ret->addChild( new AST( lookaheadToken(1) ) );
            match(ID);

            // Get the macro keywords
            ret->addChild( IdList() );

            // Get the macro terminator
            ret->addChild( new AST( lookaheadToken(1) ) );
            match(ID);

            // Get the macro transform rules
            while (TERM != lookaheadType(1))
            {
                AST* transform = new AST( TRANSFORM );
                transform->addChild( IdList() );
                transform->addChild( Expression() );
                ret->addChild( transform );
            }
            break;

        default:
            throw Exception( lookaheadToken(1) );
            break;
    }
    match(TERM);
    return ret;
}

AST* DLParser::Application(void)
{
    AST* ret = NULL;

    // Macro Expression
    if ( isMacroName() )
    {
    }

    // Traditional Function Application
    else if( (lookaheadType(1) == ID) && (lookaheadType(2) == LPAR) )
    {
        ret = new AST( lookaheadToken(1) );
        consume();
        match(LPAR);
        ret = new AST(APPLY, 2, ret, ExpList());
        match(RPAR);
    }

    // Infix Function Application
    else if( lookaheadType(1) == LPAR )
    {
        AST* operation = NULL;
        AST* operand1  = NULL;
        AST* operand2  = NULL;

        match(LPAR);
        operand1 = Expression();
        operation = new AST( lookaheadToken(1) );
        match(ID);
        operand2 = Expression();
        match(RPAR);

        ret = new AST( APPLY, 2, operation, new AST(EXP_LIST, 2, operand1, operand2) );
    }

    // Literal
    else
    {
        ret = Literal();
    }

    return ret;
}

AST* DLParser::Literal(void)
{
    AST* ret = new AST( lookaheadToken(1) );
    consume();
    return ret;
}

AST* DLParser::ExpList(void)
{
    AST* ret = new AST(EXP_LIST);
    while(RPAR != lookaheadType(1))
    {
        ret->addChild( Expression() );
    }
    return ret;
}

AST* DLParser::IdList(void)
{
    AST* ret = new AST(ID_LIST);
    match(LPAR);
    while(ID == lookaheadType(1))
    {
        ret->addChild( new AST( lookaheadToken(1) ) );
        consume();
    }
    match(RPAR);
    return ret;
}

