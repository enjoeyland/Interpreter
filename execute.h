#ifndef EXECUTE_H
#define EXECUTE_H

#include "parser.h"

ParseNode* execute(ParseNode*);

ParseNode* solveVariable(Token*);
ParseNode* operate(TokenType, ParseNode*, ParseNode*, ParseNode*);
ParseNode* operatePlus(Token*, Token*);
ParseNode* operateMinus(Token*, Token*);
ParseNode* operateMultiply(Token*, Token*);
ParseNode* operateDivide(Token*, Token*);
ParseNode* operateAssign(Token*, Token*);
ParseNode* operateSub(Token*, Token*, Token*);

#endif  // EXECUTE_H