/* Copyright 2026 Dylan Leothaud
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 *
 * 
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

grammar cidmol;

program: items += programItem* EOF;

programItem:
	functionDeclaration
	| structDeclaration
	| unionDeclaration
	| aliasDeclaration
	| variableDeclaration;

aliasDeclaration: ALIAS name = ID EQUAL base = type;

functionDeclaration:
	FUN returnType = type name = ID LP (
		args += argument (COMMA args += argument)*
	)? RP (body = compoundStatement | SEMI);

structDeclaration:
	STRUCT name = ID LB (
		fields += variableDeclaration
		| ops += operationDeclaration
		| methods += functionDeclaration
		| casts += castDeclaration
	)+ RB;

unionDeclaration:
	UNION name = ID LB fields += argument (
		COMMA fields += argument
	)* RB;

castDeclaration: CAST t = type body = compoundStatement;

operationDeclaration:
	OPERATOR opType = operatorType LP RB body = compoundStatement;

operatorType:
	PLUS				# AddOperator
	| MINUS				# SubOperator
	| STAR				# MulOperator
	| PERCENT			# ModOperator
	| SLASH				# DivOperator
	| DOUBLE_AMPERSAND	# LandOperator
	| DOUBLE_PIPE		# LorOperator
	| DOUBLE_WEDGE		# LxorOperator
	| EXCLAMATION		# LnotOperator
	| DOUBLE_EQUAL		# EqOperator
	| LAB				# LtOperator;

variableDeclaration:
	VAR varType = type decls += declarator (
		COMMA decls += declarator
	) SEMI;

declarator: name = ID (EQUAL value = expression)?;

argument: t = type name = ID;

statement:
	variableDeclaration
	| compoundStatement
	| whileStatement
	| doWhileStatement
	| forStatement
	| ifStatement
	| switchStatement
	| expressionStatement
	| nopStatement
	| continueStatement
	| breakStatement
	| returnStatement;

compoundStatement: LB items += statement* RB;

whileStatement: WHILE LP cond = expression RP body = statement;

doWhileStatement:
	DO body = statement WHILE LP cond = expression RP;

forStatement:
	FOR LP (init = variableDeclaration | SEMI) cond = expression? SEMI step = expression? RP body =
		statement;

ifStatement:
	IF LP cond = expression RP thenPart = statement (
		ELSE elsePart = statement
	)?;

switchStatement:
	SWITCH LP cond = expression RP LB items += switchItem+ RB;

switchItem: switchCase | switchDefault;

switchCase: CASE cond = expression COLON body = statement;

switchDefault: DEFAULT COLON body = statement;

expressionStatement: value = expression SEMI;

nopStatement: SEMI;

continueStatement: CONTINUE;

breakStatement: BREAK;

returnStatement: RETURN value = expression? SEMI;

type: base = baseType modifiers += typeModifiers*;

typeModifiers: arrayTypeModifier | ptrTypeModifier;

arrayTypeModifier: LSB size = expression RSB;

ptrTypeModifier: STAR;

baseType: voidType | bitType | structType | unionType | idType;

voidType: VOID;
bitType: BIT;
structType: STRUCT name = ID;
unionType: UNION name = ID;
idType: name = ID;

expression: assignExpression;

assignOperator:
	EQUAL						# Assign
	| PLUS_EQUAL				# AddAssign
	| MINUS_EQUAL				# SubAssign
	| STAR_EQUAL				# MulAssign
	| PERCENT_EQUAL				# ModAssign
	| SLASH_EQUAL				# DivAssign
	| DOUBLE_AMPERSAND_EQUAL	# LandAssign
	| DOUBLE_PIPE_EQUAL			# LorAssign
	| DOUBLE_WEDGE_EQUAL		# LxorAssign
	| AMPERSAND_EQUAL			# BandAssign
	| PIPE_EQUAL				# BorAssign
	| WEDGE_EQUAL				# BxorAssign
	| DOUBLE_LAB_EQUAL			# LshiftAssign
	| DOUBLE_RAB_EQUAL			# RshiftAssign;

assignExpression:
	operands += ternaryExpression (
		operators += assignOperator operands += ternaryExpression
	)*;

ternaryExpression:
	cond = lorExpression QUESTION thenPart = expression COLON elsePart = expression
	| lorExpression;

lorExpression:
	operands += lxorExpression (
		DOUBLE_PIPE operands += lxorExpression
	)*;

lxorExpression:
	operands += landExpression (
		DOUBLE_WEDGE operands += landExpression
	)*;

landExpression:
	operands += borExpression (
		DOUBLE_AMPERSAND operands += borExpression
	)*;

borExpression:
	operands += bxorExpression (PIPE operands += bxorExpression)*;

bxorExpression:
	operands += bandExpression (WEDGE operands += bandExpression)*;

bandExpression:
	operands += equalityExpression (
		AMPERSAND operands += equalityExpression
	)*;

equalityOperator:
	DOUBLE_EQUAL		# EqOp
	| EXCLAMATION_EQUAL	# NeqOp;

equalityExpression:
	operands += comparisonExpression (
		operators += equalityOperator operands += comparisonExpression
	)*;

comparisonOperator:
	LAB			# LtOp
	| LAB_EQUAL	# LeOp
	| RAB		# GtOp
	| RAB_EQUAL	# GeOp;

comparisonExpression:
	operands += shiftExpression (
		operators += comparisonOperator operands += shiftExpression
	)*;

shiftOperator: DOUBLE_LAB # LshiftOp | DOUBLE_RAB # RshiftOp;

shiftExpression:
	operands += additiveExpression (
		operators += shiftOperator operands += additiveExpression
	)*;

additiveOperator: PLUS # AddOp | MINUS # SubOp;

additiveExpression:
	operands += multiplicativeExpression (
		operators += additiveOperator operands += multiplicativeExpression
	)*;

multiplicativeOperator:
	STAR		# MulOp
	| SLASH		# DivOp
	| PERCENT	# ModOp;

multiplicativeExpression:
	operands += prefixExpression (
		operators += multiplicativeOperator operands += prefixExpression
	)*;

prefixExpression:
	prefixIncrExpression
	| prefixDecrExpression
	| addrofExpression
	| derefExpression
	| syscallExpression
	| sizeofExpression
	| postfixExpression;

prefixIncrExpression: DOUBLE_PLUS base = prefixExpression;

prefixDecrExpression: DOUBLE_MINUS base = prefixExpression;

addrofExpression: AMPERSAND base = prefixExpression;

derefExpression: STAR base = prefixExpression;

syscallExpression:
	SYSCALL LP code = expression (COMMA args += expression)* RP;

sizeofExpression: SIZEOF LP t = type RP;

postfixExpression:
	base = baseExpression modifiers += postfixModifiers*;

postfixModifiers:
	arrayExpressionModifier
	| fieldModifier
	| ptrFieldModifier
	| incrModifier
	| decrModifier
	| funModifier
	| castModifier
	| bitcastModifier;

arrayExpressionModifier: LSB index = expression RSB;

fieldModifier: DOT name = ID;

ptrFieldModifier: ARROW name = ID;

incrModifier: DOUBLE_PLUS;

decrModifier: DOUBLE_MINUS;

funModifier:
	LP (args += expression (COMMA args += expression)*)? RP;

castModifier: CAST t = type;

bitcastModifier: BITCAST t = type;

baseExpression:
	idExpression
	| intExpression
	| charExpression
	| stringExpression
	| trueExpression
	| falseExpression
	| thisExpression
	| otherExpression
	| parentExpression;

idExpression: name = ID;

intExpression: value = INT;

charExpression: value = CHAR;

stringExpression: value = STRING;

trueExpression: TRUE;

falseExpression: FALSE;

thisExpression: THIS;

otherExpression: OTHER;

parentExpression: LP base = expression RP;

//

//

//

//

WS: [ \t\r\n] -> skip;
SL_COMMENT: '//' ~[\n]* -> skip;
ML_COMMENT: '/*' .*? '*/' -> skip;
DOUBLE_MINUS: '--';
MINUS_EQUAL: '-=';
ARROW: '->';
MINUS: '-';
COMMA: ',';
SEMI: ';';
COLON: ':';
EXCLAMATION_EQUAL: '!=';
EXCLAMATION: '!';
QUESTION: '?';
DOT: '.';
LP: '(';
RP: ')';
LSB: '[';
RSB: ']';
LB: '{';
RB: '}';
STAR_EQUAL: '*=';
STAR: '*';
SLASH_EQUAL: '/=';
SLASH: '/';
DOUBLE_AMPERSAND_EQUAL: '&&=';
DOUBLE_AMPERSAND: '&&';
AMPERSAND_EQUAL: '&=';
AMPERSAND: '&';
PERCENT_EQUAL: '%=';
PERCENT: '%';
DOUBLE_WEDGE_EQUAL: '^^=';
DOUBLE_WEDGE: '^^';
WEDGE_EQUAL: '^=';
WEDGE: '^';
DOUBLE_PLUS: '++';
PLUS_EQUAL: '+=';
PLUS: '+';
DOUBLE_LAB_EQUAL: '<<=';
DOUBLE_LAB: '<<';
LAB_EQUAL: '<=';
LAB: '<';
DOUBLE_EQUAL: '==';
EQUAL: '=';
DOUBLE_RAB_EQUAL: '>>=';
DOUBLE_RAB: '>>';
RAB_EQUAL: '>=';
RAB: '>';
DOUBLE_PIPE_EQUAL: '||=';
DOUBLE_PIPE: '||';
PIPE_EQUAL: '|=';
PIPE: '|';
TILDE: '~';
ALIAS: 'alias';
BIT: 'bit';
BITCAST: 'bitcast';
BREAK: 'break';
CASE: 'case';
CAST: 'cast';
CONTINUE: 'continue';
DEFAULT: 'default';
DO: 'do';
ELSE: 'else';
FALSE: 'false';
FOR: 'for';
FUN: 'fun';
IF: 'if';
OPERATOR: 'operator';
OTHER: 'other';
RETURN: 'return';
SIZEOF: 'sizeof';
STRUCT: 'struct';
SWITCH: 'switch';
SYSCALL: 'syscall';
THIS: 'this';
TRUE: 'true';
UNION: 'union';
VAR: 'var';
VOID: 'void';
WHILE: 'while';
INT: [0-9]+;
fragment ESC: '\\' [btnrf"'\\];
CHAR: '\'' ( ESC | ~['\\\r\n]) '\'';
STRING: '"' (ESC | ~["\\\r\n])* '"';
ID: [a-zA-Z_][a-zA-Z0-9_]*;