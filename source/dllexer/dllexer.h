#ifndef DLLEXER_H
#define DLLEXER_H

#include "llnlexer.h"
#include <sstream>

typedef enum TokenTypes
{
    // Datatypes
    ID       = 0,
    NUM      = 1,
    CHAR     = 2,
    STRING   = 3,
    SYMBOL   = 4,
    LIST     = 5,
    VECTOR   = 6,
    FUNC     = 7,
    MAP      = 8,

    // Symbols
    LBRACK   = 10,
    RBRACK   = 11,
    LPAR     = 12,
    RPAR     = 13,
    LBRACE   = 14,
    RBRACE   = 15,
    COMMA    = 16,
    PIPE     = 17,

    // Operators
    AND      = 20,
    OR       = 21,
    NOT      = 22,
    EQ       = 23,
    NE       = 24,
    LT       = 25,
    GT       = 26,
    LTE      = 27,
    GTE      = 28,
    ADD      = 29,
    SUB      = 30,
    MUL      = 31,
    DIV      = 32,
    DEFN     = 33,
    ASSIGN   = 34,
    MEMB     = 35,
    SEP      = 36,
    ARRY_IDX = 37,
    MACRO    = 38,
    IMPORT   = 39,
    MUTATE   = 40,

    // AST "Virtual" Node Types
    PROGRAM  = 50,
    BLOCK    = 51,
    FN_CALL  = 52,
    PARAMS   = 53,
    PATT     = 54,
} eTokenTypes;

typedef struct {
    char match;
    eTokenTypes type;
} SingleCharMatch_T;

class DLLexer : public LLNLexer {
    public:
        DLLexer(std::istream& in);
        bool isWhiteSpace(void);
        bool isLetter(void);
        bool isDigit(void);
        bool isOperator(void);
        bool isStringChar(void);
        void WS(void);
        void COMMENT(void);

        Token next(void);
        void Id(Token& tok);
        void Number(Token& tok, bool isNegative);
        std::string FloatingPoint(bool isNegative);
        void Decimal(std::ostringstream& oss);
        void Char(Token& tok);
        void String(Token& tok);
        void Symbol(Token& tok);
        void SingleCharOp(Token& tok);
        void MultiCharOp(Token& tok);
        std::string EscapeSequence();
};

#endif
