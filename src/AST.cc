#include "AST.hh"
#include "Driver.hh"
SymbolTable Driver::symbolTable;

#include "Parser.hh"
#include <math.h> // math functions
#include <pthread.h> // sleep function
#include <time.h>
#include <sstream>

void AST_BinaryOp::print(std::ostream& os) {
  os << "(";
  left->print(os);
  switch (token_number) {
    case yy::Parser::token::UNARYPLUS: os << " + "; break;
    case yy::Parser::token::UNARYMINUS: os << " - "; break;
    case yy::Parser::token::OPMULTIPLY: os << " * "; break;
    case yy::Parser::token::OPDIVIDE: os << " / "; break;
    case yy::Parser::token::OPLESSTHAN: os << " < "; break;
    case yy::Parser::token::OPGREATERTHAN: os << " > "; break;
    case yy::Parser::token::OPLESSTHANEQUAL: os << " <= "; break;
    case yy::Parser::token::OPGREATERTHANEQUAL: os << " >= "; break;
    case yy::Parser::token::BOOLOR: os << " or "; break;
    case yy::Parser::token::BOOLAND: os << " and "; break;
    case yy::Parser::token::OPEQUAL: os << " == "; break;
    case yy::Parser::token::OPNOTEQUAL: os << " <> "; break;


  }
  right->print(os);
  os << ")";
}

NameCheckResult AST_BinaryOp::runNameChecking(){
	NameCheckResult result = NameCheckSuccess;

	if (left->runNameChecking() == NameCheckFailed) result = NameCheckFailed;
	if (right->runNameChecking() == NameCheckFailed) result = NameCheckFailed;

	return result;
}

TypeCheckResult AST_BinaryOp::runTypeChecking(){
	TypeCheckResult result = TypeCheckSuccess;

	if (left != 0){
		if (left->runTypeChecking() == TypeCheckFailed) result = TypeCheckFailed;
	}

	if (right != 0){
		if (right->runTypeChecking() == TypeCheckFailed) result = TypeCheckFailed;
	}

	switch (token_number) {
	    case yy::Parser::token::UNARYPLUS:
	    case yy::Parser::token::UNARYMINUS:
	    case yy::Parser::token::OPMULTIPLY:
	    case yy::Parser::token::OPDIVIDE:
	    case yy::Parser::token::OPLESSTHAN:
	    case yy::Parser::token::OPGREATERTHAN:
	    case yy::Parser::token::OPLESSTHANEQUAL:
	    case yy::Parser::token::OPGREATERTHANEQUAL:
	    	if (left->getType() != TypeNumber && left->getType() != TypeGeneric){
				reportTypeError(TypeNumber, left->getType());
				result = TypeCheckFailed;
	    	}
	    	if (right->getType() != TypeNumber && right->getType() != TypeGeneric){
	    		reportTypeError(TypeNumber, right->getType());
	    		result = TypeCheckFailed;
	    	}
	    	break;
	    case yy::Parser::token::BOOLOR:
	    case yy::Parser::token::BOOLAND:
	    	if (left->getType() != TypeBool && left->getType() != TypeGeneric){
				reportTypeError(TypeBool, left->getType());
				result = TypeCheckFailed;
	    	}
	    	if (right->getType() != TypeBool && right->getType() != TypeGeneric){
	    		reportTypeError(TypeBool, right->getType());
	    		result = TypeCheckFailed;
	    	}
	    	break;

	    case yy::Parser::token::OPEQUAL:
	    case yy::Parser::token::OPNOTEQUAL:
	    	if (left->getType() != right->getType() && left->getType() != TypeGeneric && right->getType() != TypeGeneric){
				reportTypeError(left->getType(), right->getType());
				result = TypeCheckFailed;
	    	}
	    	break;
	  }

	return result;
}

ExpressionType AST_BinaryOp::getType(){
	  switch (token_number) {
	    case yy::Parser::token::UNARYPLUS:
	    case yy::Parser::token::UNARYMINUS:
	    case yy::Parser::token::OPMULTIPLY:
	    case yy::Parser::token::OPDIVIDE:
	    	return TypeNumber;

	    case yy::Parser::token::OPLESSTHAN:
	    case yy::Parser::token::OPGREATERTHAN:
	    case yy::Parser::token::OPLESSTHANEQUAL:
	    case yy::Parser::token::OPGREATERTHANEQUAL:
	    case yy::Parser::token::BOOLOR:
	    case yy::Parser::token::BOOLAND:
	    case yy::Parser::token::OPEQUAL:
	    case yy::Parser::token::OPNOTEQUAL:
	    	return TypeBool;
	    	break;
	  }
}

Value *AST_BinaryOp::getValue(){
	switch(token_number){
		case yy::Parser::token::UNARYPLUS:
			return (Value *)new NumberValue(((NumberValue *)left->getValue())->getDouble() + ((NumberValue *)right->getValue())->getDouble());
		case yy::Parser::token::UNARYMINUS:
			return (Value *)new NumberValue(((NumberValue *)left->getValue())->getDouble() - ((NumberValue *)right->getValue())->getDouble());
		case yy::Parser::token::OPMULTIPLY:
			return (Value *)new NumberValue(((NumberValue *)left->getValue())->getDouble() * ((NumberValue *)right->getValue())->getDouble());
		case yy::Parser::token::OPDIVIDE:
			if (((NumberValue *)right->getValue())->getDouble() == 0) throw RuntimeException("Division by zero");
			return (Value *)new NumberValue(((NumberValue *)left->getValue())->getDouble() / ((NumberValue *)right->getValue())->getDouble());

	    case yy::Parser::token::OPLESSTHAN:
	    	return (Value *)new BoolValue(((NumberValue *)left->getValue())->getDouble() < ((NumberValue *)right->getValue())->getDouble());
	    case yy::Parser::token::OPGREATERTHAN:
	    	return (Value *)new BoolValue(((NumberValue *)left->getValue())->getDouble() > ((NumberValue *)right->getValue())->getDouble());
	    case yy::Parser::token::OPLESSTHANEQUAL:
	    	return (Value *)new BoolValue(((NumberValue *)left->getValue())->getDouble() <= ((NumberValue *)right->getValue())->getDouble());
	    case yy::Parser::token::OPGREATERTHANEQUAL:
	    	return (Value *)new BoolValue(((NumberValue *)left->getValue())->getDouble() >= ((NumberValue *)right->getValue())->getDouble());

	    case yy::Parser::token::BOOLOR:
	    	return (Value *)new BoolValue(((BoolValue *)left->getValue())->getBool() || ((BoolValue *)right->getValue())->getBool());
	    case yy::Parser::token::BOOLAND:
	    	return (Value *)new BoolValue(((BoolValue *)left->getValue())->getBool() && ((BoolValue *)right->getValue())->getBool());
	    case yy::Parser::token::OPEQUAL:
	    	return (Value *)new BoolValue(left->getValue()->equals(right->getValue()));
	    case yy::Parser::token::OPNOTEQUAL:
	    	return (Value *)new BoolValue(!(left->getValue()->equals(right->getValue())));
	}
}

void AST_VariableDeclaration::print(std::ostream& os) {
	switch(type){
		case TypeBool: os << "Bool"; break;
		case TypeString: os << "String"; break;
		case TypeNumber: os << "Number"; break;
		case TypeList: os << "List"; break;
		case TypeVoid: os << "Void"; break;
	}
    os << " ";
    identifier->print(os);
    os << " ; ";
}

NameCheckResult AST_VariableDeclaration::runNameChecking(){
	StringTableEntry *entry = identifier->entry;

	if (Driver::symbolTable.IsSymbolInCurrentScopeLevel(entry)){
		reportError(*entry->GetName() + " multiple declaration ");
		return NameCheckFailed;
	}else{
		Driver::symbolTable.AddSymbol(entry, getType());
		return NameCheckSuccess;
	}
}

SemanticCheckResult AST_IfStatement::checkForSemanticErrors(){
	SemanticCheckResult result = SemanticCheckSuccess;

	if (expression->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
	else if (expression->getType() != TypeBool){
		result = SemanticCheckFailed;
		reportTypeError(TypeBool, expression->getType());
	}

	Driver::symbolTable.EnterScope();
	if (ifBlock->checkForSemanticErrors() == SemanticCheckFailed) result = SemanticCheckFailed;

    AddReferencesToActivationRecord(activationRecord);
	Driver::symbolTable.ExitScope();

	return result;
}

void AST_IfStatement::execute(){

	BoolValue *condition = (BoolValue *)expression->getValue();
	if (condition->isTrue()){
		activationRecord.EnterScope();
		ifBlock->execute();
		activationRecord.ExitScope();
	}
}

SemanticCheckResult AST_IfElseStatement::checkForSemanticErrors(){
	SemanticCheckResult result = SemanticCheckSuccess;

	if (expression->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
	else if (expression->getType() != TypeBool){
		result = SemanticCheckFailed;
		reportTypeError(TypeBool, expression->getType());
	}

	Driver::symbolTable.EnterScope();
	if (ifBlock->checkForSemanticErrors() == SemanticCheckFailed) result = SemanticCheckFailed;
    AddReferencesToActivationRecord(ifActivationRecord);
	Driver::symbolTable.ExitScope();

	Driver::symbolTable.EnterScope();
	if (elseBlock->checkForSemanticErrors() == SemanticCheckFailed) result = SemanticCheckFailed;
    AddReferencesToActivationRecord(elseActivationRecord);
	Driver::symbolTable.ExitScope();

	return result;

}

void AST_IfElseStatement::execute(){
	BoolValue *condition = (BoolValue *)expression->getValue();
	if (condition->isTrue()){
		ifActivationRecord.EnterScope();
		ifBlock->execute();
		ifActivationRecord.ExitScope();
	}else{
		elseActivationRecord.EnterScope();
		elseBlock->execute();
		elseActivationRecord.ExitScope();
	}
}

SemanticCheckResult AST_WhileStatement::checkForSemanticErrors(){
	SemanticCheckResult result = SemanticCheckSuccess;

	if (expression->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
	else if (expression->getType() != TypeBool){
		result = SemanticCheckFailed;
		reportTypeError(TypeBool, expression->getType());
	}

	Driver::symbolTable.EnterScope();
	if (body->checkForSemanticErrors() == SemanticCheckFailed) result = SemanticCheckFailed;

    AddReferencesToActivationRecord(activationRecord);
	Driver::symbolTable.ExitScope();

	return result;
}

void AST_WhileStatement::execute(){
	BoolValue *condition = (BoolValue *)expression->getValue();

	while (condition->isTrue()){
		activationRecord.EnterScope();
		body->execute();
		activationRecord.ExitScope();

		// Update the condition (might have changed)
		condition = (BoolValue *)expression->getValue();
	}
}

SemanticCheckResult AST_ForStatement::checkForSemanticErrors(){
	SemanticCheckResult result = SemanticCheckSuccess;

	Driver::symbolTable.EnterScope();

	/* Special declaration for identifier */
	StringTableEntry *entry = identifier->entry;

	if (Driver::symbolTable.IsSymbolInCurrentScopeLevel(entry)){
		reportError(*entry->GetName() + " multiple declaration ");
		result = SemanticCheckFailed;
	}else{
		Driver::symbolTable.AddSymbol(entry, TypeNumber); // For loops always have a number variable
		result = SemanticCheckSuccess;
	}

	if (startExpression->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
	else if (startExpression->getType() != TypeNumber){
		reportError("number expected in for loop expression #2 but " + typeToString(startExpression->getType()) + " was given ");
		result = SemanticCheckFailed;
	}

	if (stopExpression->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
	else if (stopExpression->getType() != TypeNumber){
		reportError("number expected in for loop expression #3 but " + typeToString(stopExpression->getType()) + " was given ");
		result = SemanticCheckFailed;
	}

	if (stepExpression != 0){
		if (stepExpression->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
		else if (stepExpression->getType() != TypeNumber){
			reportError("number expected in for loop expression #4 but " + typeToString(stepExpression->getType()) + " was given ");
			result = SemanticCheckFailed;
		}
	}

	if (body->checkForSemanticErrors() == SemanticCheckFailed) result = SemanticCheckFailed;

    AddReferencesToActivationRecord(activationRecord);
	Driver::symbolTable.ExitScope();

	return result;
}

void AST_ForStatement::execute(){

	activationRecord.EnterScope();

	NumberValue *startValue = (NumberValue *)startExpression->getValue();
	NumberValue *endValue = (NumberValue *)stopExpression->getValue();
	NumberValue *stepValue;
	if (stepExpression != 0) stepValue = (NumberValue *)stepExpression->getValue();
	else stepValue = new NumberValue(1);

	// Initialization
	identifier->entry->value->set((Value *)startValue);

	// In order
	if (startValue->getDouble() < endValue->getDouble()){
		for (double i = startValue->getDouble(); i <= endValue->getDouble(); i += stepValue->getDouble()){
			body->execute();

			// update identifier at the end of each iteration
			NumberValue *identifierValue = (NumberValue *)identifier->entry->value->get();
			identifierValue->setDouble(i+stepValue->getDouble());
		}
	}

	// Reverse order
	else{
		for (double i = startValue->getDouble(); i >= endValue->getDouble(); i -= stepValue->getDouble()){
			body->execute();

			// update identifier at the end of each iteration
			NumberValue *identifierValue = (NumberValue *)identifier->entry->value->get();
			identifierValue->setDouble(i-stepValue->getDouble());
		}
	}

	activationRecord.ExitScope();
}

SemanticCheckResult AST_NewCommand::checkForSemanticErrors(){
	SemanticCheckResult result = SemanticCheckSuccess;

	/* Function identifier needs special care (sort of like variable declaration)
	 * Overloading should be handled here */
	StringTableEntry *entry = commandId->entry;

	if (Driver::symbolTable.IsSymbolInCurrentScopeLevel(entry)){
		reportError(*entry->GetName() + " multiple declaration ");
		result = SemanticCheckFailed;
	}else{
		Driver::symbolTable.AddSymbol(entry, getType());
	}

	Driver::symbolTable.EnterScope();

	AST_Parameter *current = firstParameter;
    while(current != 0){
      if (current->runNameChecking() == NameCheckFailed) result = SemanticCheckFailed;
	  current = current->next;
    }

	if (body->checkForSemanticErrors() == SemanticCheckFailed) result = SemanticCheckFailed;

	AddReferencesToActivationRecord(activationRecord);
	Driver::symbolTable.ExitScope();

	return result;
}

ExpressionType AST_NewCommand::getType(){
	return type;
}


void AST_NewCommand::print(std::ostream& os) {
	switch(type){
		case TypeBool: os << "Bool"; break;
		case TypeString: os << "String"; break;
		case TypeNumber: os << "Number"; break;
		case TypeList: os << "List"; break;
		case TypeVoid: os << "Void"; break;
	}
    os << " ";
    commandId->print(os);
    os << " ( ";

    AST_Parameter *current = firstParameter;
    while (current != 0){
    	current->print(os);
    	os << " ";
    	current = current->next;
    }

    os << ") ";

    body->print(os);
}

void AST_NewCommand::execute(){
	// Just assign a value to the identifier associated with this command
	commandId->entry->value->set((Value *)new FunctionPointerValue(this));
}

Value* AST_NewCommand::call(AST_Argument *firstArgument){
	// Before we enter the scope, we need to copy whathever arguments we had into a list
	list<Value *> argumentValues;
	AST_Argument *currentArgument = firstArgument;
	while (currentArgument != 0){
		argumentValues.push_back(currentArgument->getValue());
		currentArgument = currentArgument->next;
	}

	// Enter the scope
	activationRecord.EnterScope();

	// This will hold the return value
	Value *returnValue = (Value *)new UnknownValue();

	// Copy arguments to params
	AST_Parameter *currentParam = firstParameter;

	while (currentParam != 0){
		Value *value = (Value *)argumentValues.front();

		currentParam->param->identifier->entry->value->set(value);

		argumentValues.pop_front();
		currentParam = currentParam->next;
	}

	// Should be empty
	if (argumentValues.size() != 0) throw RuntimeException("Invalid function call, arguments are different than parameters.");

	// Execute until return
	try{
		body->execute();
	}catch(const FunctionTerminatedException &e){
		returnValue = e.getValue();
	}
	activationRecord.ExitScope();

	return returnValue;
}

NameCheckResult AST_Assignment::runNameChecking(){
	NameCheckResult result = NameCheckSuccess;

	if (identifier->runNameChecking() == NameCheckFailed) result = NameCheckFailed;
	if (expression->runNameChecking() == NameCheckFailed) result = NameCheckFailed;

	return result;
}


TypeCheckResult AST_Assignment::runTypeChecking(){
	TypeCheckResult result = TypeCheckSuccess;

	if (identifier->getType() != expression->getType() && expression->getType() != TypeGeneric){
		reportError("assignment type mismatch ");
		result = TypeCheckFailed;
	}

	if (expression->runTypeChecking() == TypeCheckFailed){
		result = TypeCheckFailed;
	}

	return result;
}

ExpressionType AST_Identifier::getType(){
	SymbolTableEntry *symbol = Driver::symbolTable.LookupSymbol(entry);
	assert(symbol != 0);
	return symbol->GetType();
}

NameCheckResult AST_Identifier::runNameChecking(){
	if (!Driver::symbolTable.IsSymbolInCurrentOrLowerScopeLevel(entry)){
		reportError(*entry->GetName() + " undeclared ");
		return NameCheckFailed;
	}

	return NameCheckSuccess;
}

TypeCheckResult AST_SysFunctionCall::runTypeChecking(){
	TypeCheckResult result = TypeCheckSuccess;

	if (firstExpression != 0){
		if (firstExpression->runTypeChecking() == TypeCheckFailed) result = TypeCheckFailed;
	}

	if (secondExpression != 0){
		if (secondExpression->runTypeChecking() == TypeCheckFailed) result = TypeCheckFailed;
	}

	// Depending on the function, we need to check the parameters.
	switch(token){
	case yy::Parser::token::MATHABS:
	case yy::Parser::token::MATHMINUS:
	case yy::Parser::token::MATHROUND:
	case yy::Parser::token::MATHFLOOR:
	case yy::Parser::token::MATHCEIL:
	case yy::Parser::token::MATHCOS:
	case yy::Parser::token::MATHSIN:
	case yy::Parser::token::MATHTAN:
	case yy::Parser::token::MATHARCTAN:
	case yy::Parser::token::MATHLN:
	case yy::Parser::token::MATHRANDOM:
	case yy::Parser::token::MATHEXP:
	case yy::Parser::token::SYSWAIT:

	case yy::Parser::token::SETPENSIZE:
	case yy::Parser::token::FORWARD:
	case yy::Parser::token::BACKWARD:
	case yy::Parser::token::LEFTTURN:
	case yy::Parser::token::RIGHTTURN:
	case yy::Parser::token::SETHEADING:

		if (firstExpression == 0){
			result = TypeCheckFailed;
			reportError("parameter missing ");
		}else if (firstExpression->getType() != TypeNumber && firstExpression->getType() != TypeGeneric){
			result = TypeCheckFailed;
			reportTypeError(TypeNumber, firstExpression->getType());
		}
		break;


	case yy::Parser::token::ISNUMBER:
	case yy::Parser::token::ISSTRING:
	case yy::Parser::token::ISBOOLEAN:
	case yy::Parser::token::ISLIST:
		if (firstExpression == 0){
			result = TypeCheckFailed;
			reportError("parameter missing ");
		}else if (firstExpression->getType() != TypeNumber &&
				firstExpression->getType() != TypeString &&
				firstExpression->getType() != TypeBool &&
				firstExpression->getType() != TypeList &&
				firstExpression->getType() != TypeGeneric){
			result = TypeCheckFailed;
			reportError("number, string, bool or list type expected but " + typeToString(firstExpression->getType()) + " was given ");
		}
		break;

	case yy::Parser::token::LISTSTRINGFIRST:
	case yy::Parser::token::LISTSTRINGREST:
	case yy::Parser::token::ISLISTSTRINGEMPTY:
		if (firstExpression == 0){
			result = TypeCheckFailed;
			reportError("parameter missing ");
		}else if (firstExpression->getType() != TypeList &&
				firstExpression->getType() != TypeString &&
				firstExpression->getType() != TypeGeneric){
			result = TypeCheckFailed;
			reportError("string or list type expected but " + typeToString(firstExpression->getType()) + " was given ");
		}
		break;
	case yy::Parser::token::LISTSTRINGNTH:
		if (firstExpression == 0 || secondExpression == 0){
			result = TypeCheckFailed;
			reportError("parameters missing (2 expected) ");
		}else{
			if (firstExpression->getType() != TypeNumber && firstExpression->getType() != TypeGeneric){
				result = TypeCheckFailed;
				reportError("number type expected for parameter 1 but " + typeToString(firstExpression->getType()) + " was given ");
			}
			if (secondExpression->getType() != TypeList &&
					secondExpression->getType() != TypeString &&
					secondExpression->getType() != TypeGeneric){
				result = TypeCheckFailed;
				reportError("string or list type expected for parameter 2 but " + typeToString(secondExpression->getType()) + " was given ");
			}
		}

		break;
	case yy::Parser::token::ISLISTSTRINGEQUAL:
		if (firstExpression == 0 || secondExpression == 0){
			result = TypeCheckFailed;
			reportError("parameters missing (2 expected) ");
		}else{
			if (firstExpression->getType() != TypeList &&
					firstExpression->getType() != TypeString &&
					firstExpression->getType() != TypeGeneric){
				result = TypeCheckFailed;
				reportError("string or list type expected for parameter 1 but " + typeToString(firstExpression->getType()) + " was given ");
			}
			if (secondExpression->getType() != TypeList &&
					secondExpression->getType() != TypeString &&
					secondExpression->getType() != TypeGeneric){
				result = TypeCheckFailed;
				reportError("string or list type expected for parameter 2 but " + typeToString(secondExpression->getType()) + " was given ");
			}
		}

	case yy::Parser::token::SYSPRINT:
		if (firstExpression == 0){
			result = TypeCheckFailed;
			reportError("parameters missing ");
		}
		break;

	case yy::Parser::token::SYSREAD:
		if (readType != TypeNumber && readType != TypeList && readType != TypeString && readType != TypeBool){
			result = TypeCheckFailed;
			reportError("number, list, string or bool expected but " + typeToString(readType) + " was given ");
		}
		break;

	case yy::Parser::token::PENUP:
	case yy::Parser::token::PENDOWN:
	case yy::Parser::token::SHOWTURTLE:
	case yy::Parser::token::HIDETURTLE:
	case yy::Parser::token::POSITION:
	case yy::Parser::token::HEADING:
		if (firstExpression != 0){
			result = TypeCheckFailed;
			reportError("unexpected parameter ");
		}
		break;


	case yy::Parser::token::SETPENCOLOR:
	case yy::Parser::token::SETPOSITION:
		if (firstExpression == 0){
			result = TypeCheckFailed;
			reportError("parameter missing ");
		}else if (firstExpression->getType() != TypeList && firstExpression->getType() != TypeGeneric){
			result = TypeCheckFailed;
			reportTypeError(TypeList, firstExpression->getType());
		}
		break;

	}



	return result;
}

void AST_SysFunctionCall::print(std::ostream &os){
	switch(token){
		case yy::Parser::token::MATHABS: os << "abs"; break;
		case yy::Parser::token::MATHMINUS: os << "minus"; break;
		case yy::Parser::token::MATHROUND: os << "round"; break;
		case yy::Parser::token::MATHFLOOR: os << "floor"; break;
		case yy::Parser::token::MATHCEIL: os << "ceil"; break;
		case yy::Parser::token::MATHCOS: os << "cos"; break;
		case yy::Parser::token::MATHSIN: os << "sin"; break;
		case yy::Parser::token::MATHTAN: os << "tan"; break;
		case yy::Parser::token::MATHARCTAN: os << "arctan"; break;
		case yy::Parser::token::MATHEXP: os << "exp"; break;
		case yy::Parser::token::MATHLN: os << "ln"; break;
		case yy::Parser::token::MATHRANDOM: os << "random"; break;
		case yy::Parser::token::ISNUMBER: os << "number?"; break;
		case yy::Parser::token::ISSTRING: os << "string?"; break;
		case yy::Parser::token::ISBOOLEAN: os << "boolean?"; break;
		case yy::Parser::token::ISLIST: os << "list?"; break;
		case yy::Parser::token::LISTSTRINGFIRST: os << "first"; break;
		case yy::Parser::token::LISTSTRINGREST: os << "rest"; break;
		case yy::Parser::token::LISTSTRINGNTH: os << "nth"; break;
		case yy::Parser::token::ISLISTSTRINGEMPTY: os << "empty?"; break;
		case yy::Parser::token::ISLISTSTRINGEQUAL: os << "equal?"; break;
		case yy::Parser::token::SYSPRINT: os << "print"; break;
		case yy::Parser::token::SYSWAIT: os << "wait"; break;
		case yy::Parser::token::SYSREAD: os << "read"; break;
		case yy::Parser::token::PENUP: os << "penup"; break;
		case yy::Parser::token::PENDOWN: os << "pendown"; break;
		case yy::Parser::token::SETPENSIZE: os << "setpensize"; break;
		case yy::Parser::token::SETPENCOLOR: os << "setpencolor"; break;
		case yy::Parser::token::FORWARD: os << "forward"; break;
		case yy::Parser::token::BACKWARD: os << "backward"; break;
		case yy::Parser::token::LEFTTURN: os << "leftturn"; break;
		case yy::Parser::token::RIGHTTURN: os << "rightturn"; break;
		case yy::Parser::token::SHOWTURTLE: os << "showturtle"; break;
		case yy::Parser::token::HIDETURTLE: os << "hideturtle"; break;
		case yy::Parser::token::POSITION: os << "position"; break;
		case yy::Parser::token::HEADING: os << "heading"; break;
		case yy::Parser::token::SETPOSITION: os << "setposition"; break;
		case yy::Parser::token::SETHEADING: os << "setheading"; break;
	}

	os << " ( ";
	if (firstExpression != 0) firstExpression->print(os);
	if (secondExpression != 0){
		os << " ";
		secondExpression->print(os);
	}
	os << " ) ; ";
}

ExpressionType AST_SysFunctionCall::getType(){
	switch(token){
		case yy::Parser::token::MATHABS:
		case yy::Parser::token::MATHMINUS:
		case yy::Parser::token::MATHROUND:
		case yy::Parser::token::MATHFLOOR:
		case yy::Parser::token::MATHCEIL:
		case yy::Parser::token::MATHCOS:
		case yy::Parser::token::MATHSIN:
		case yy::Parser::token::MATHTAN:
		case yy::Parser::token::MATHARCTAN:
		case yy::Parser::token::MATHLN:
		case yy::Parser::token::MATHRANDOM:
		case yy::Parser::token::MATHEXP:
			return TypeNumber;

		case yy::Parser::token::SYSREAD:
			return readType;
		case yy::Parser::token::LISTSTRINGFIRST:
		case yy::Parser::token::LISTSTRINGREST:
			return firstExpression->getType();
		case yy::Parser::token::ISLISTSTRINGEMPTY:
		case yy::Parser::token::ISLISTSTRINGEQUAL:
		case yy::Parser::token::ISNUMBER:
		case yy::Parser::token::ISSTRING:
		case yy::Parser::token::ISBOOLEAN:
		case yy::Parser::token::ISLIST:
			return TypeBool;
		case yy::Parser::token::LISTSTRINGNTH:
			return TypeGeneric;
		default:
			cout << "Warning: unhandled getType in sysFunctionCall for " << token << " ";
			return TypeGeneric;
	}
}

Value* AST_SysFunctionCall::getValue(){
	switch(token){
		case yy::Parser::token::MATHABS:
			return (Value *)(new NumberValue(fabs(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHMINUS:
			return (Value *)(new NumberValue(-(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHROUND:
			return (Value *)(new NumberValue((int)(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHFLOOR:
			return (Value *)(new NumberValue(floor(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHCEIL:
			return (Value *)(new NumberValue(ceil(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHCOS:
			return (Value *)(new NumberValue(cos(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHSIN:
			return (Value *)(new NumberValue(sin(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHTAN:
			return (Value *)(new NumberValue(tan(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHARCTAN:
			return (Value *)(new NumberValue(atan(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHLN:
			return (Value *)(new NumberValue(log(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHEXP:
					return (Value *)(new NumberValue(exp(((NumberValue *)(firstExpression->getValue()))->getDouble())));
		case yy::Parser::token::MATHRANDOM:{
			double param = ((NumberValue *)(firstExpression->getValue()))->getDouble();
			if (param == 0.0) return (Value *)new NumberValue(0);
			else if (param == 1.0){
				// return a float number between 0 and 1
				double randomNum = ((double)rand() / (double)RAND_MAX);
				return (Value *)new NumberValue(randomNum);
			}else{
				// whole number less than param
				return (Value *)new NumberValue(rand() % ((int)param));
			}
		}
		case yy::Parser::token::POSITION:
			return (Value *)new ListValue(); // TODO retrieve from turtle
		case yy::Parser::token::HEADING:
			return (Value *)new NumberValue(11.11); // TODO retrieve from turtle


		case yy::Parser::token::ISNUMBER:
			return (Value *)new BoolValue(firstExpression->getValue()->getType() == TypeNumber);
		case yy::Parser::token::ISSTRING:
			return (Value *)new BoolValue(firstExpression->getValue()->getType() == TypeString);
		case yy::Parser::token::ISLIST:
			return (Value *)new BoolValue(firstExpression->getValue()->getType() == TypeList);
		case yy::Parser::token::ISBOOLEAN:
			return (Value *)new BoolValue(firstExpression->getValue()->getType() == TypeBool);


		case yy::Parser::token::LISTSTRINGFIRST:{
			Value *value = firstExpression->getValue();

			// String or list?
			if (value->getType() == TypeString){
				StringValue *strValue = (StringValue *)value;
				string str = strValue->getString();

				//Is there something in the string?
				if (str.length() > 0) return (Value *)new StringValue(str.substr(0, 1));
				else{
					// Nop, empty string, runtime exception
					throw RuntimeException("Cannot retrieve first from an empty string.");
				}

			}else if (value->getType() == TypeList){
				ListValue *listValue = (ListValue *)value;
				return listValue->getItem(0);
			}
		}
		case yy::Parser::token::LISTSTRINGREST:{
			Value *value = firstExpression->getValue();

			// String or list?
			if (value->getType() == TypeString){
				StringValue *strValue = (StringValue *)value;
				string str = strValue->getString();

				//Is there something in the string?
				if (str.length() > 0) return (Value *)new StringValue(str.substr(1, str.length()));
				else{
					// Nop, empty string, runtime exception
					throw RuntimeException("Cannot retrieve the rest of an empty string.");
				}

			}else if (value->getType() == TypeList){
				ListValue *listValue = (ListValue *)value;
				ListValue *retValue = (ListValue *)listValue->clone();
				retValue->popFront();

				return (Value *)retValue;
			}
		}

		case yy::Parser::token::ISLISTSTRINGEMPTY:{
			Value *value = firstExpression->getValue();

			// String or list?
			if (value->getType() == TypeString){
				StringValue *strValue = (StringValue *)value;
				return (Value *)new BoolValue(strValue->getString().length() == 0);
			}else if (value->getType() == TypeList){
				ListValue *listValue = (ListValue *)value;
				return (Value *)new BoolValue(listValue->size() == 0);
			}
		}

		case yy::Parser::token::LISTSTRINGNTH:{
			Value *value = secondExpression->getValue();
			int index = (int)((NumberValue *)firstExpression->getValue())->getDouble();

			// String or list?
			if (value->getType() == TypeString){
				StringValue *strValue = (StringValue *)value;
				string str = strValue->getString();

				//Is the string long enough?
				if (str.length() > index) return (Value *)new StringValue(str.substr(index, 1));
				else{
					// Nop, empty string, runtime exception
					throw RuntimeException("Index out of bounds error.");
				}

			}else if (value->getType() == TypeList){
				ListValue *listValue = (ListValue *)value;
				return listValue->getItem(index);
			}
		}
		case yy::Parser::token::ISLISTSTRINGEQUAL:{
			return (Value *)new BoolValue(firstExpression->getValue()->equals(secondExpression->getValue()));
		}

		case yy::Parser::token::SYSREAD:{
			Value *result = Value::CreateNew(readType);
			result->read(cin);
			return result;
		}


		default:
			throw RuntimeException("Invalid function call.");
			return 0;
	}
}

void AST_SysFunctionCall::execute(){
	switch(token){
		case yy::Parser::token::SYSPRINT:
			firstExpression->getValue()->print(cout);
			cout.flush();
			break;
		case yy::Parser::token::SYSWAIT:{
			double param = ((NumberValue *)(firstExpression->getValue()))->getDouble();
			if (param < 0) param = 0;
			usleep((param*1000000)/60);
			break;
		}

			// TODO! link to a graphic library to actually move a real turtle

		case yy::Parser::token::SETPENSIZE:
			cout << "Set pen size: " << ((NumberValue *)(firstExpression->getValue()))->getDouble() << endl;

			// TODO save value to turtle
			break;

		case yy::Parser::token::SETPENCOLOR:
			cout << "Set pen color: ";
			firstExpression->getValue()->print(cout);
			cout << endl;
			// TODO save value to turtle
			break;
		case yy::Parser::token::SETPOSITION:
			cout << "Set position: ";
			firstExpression->getValue()->print(cout);
			cout << endl;
			// TODO save value to turtle
			break;

		case yy::Parser::token::FORWARD:
			cout << "Moving turtle forward " << ((NumberValue *)(firstExpression->getValue()))->getDouble() << " steps" << endl;
			break;
		case yy::Parser::token::BACKWARD:
			cout << "Moving turtle backward " << ((NumberValue *)(firstExpression->getValue()))->getDouble() << " steps" << endl;
			break;
		case yy::Parser::token::LEFTTURN:
			cout << "Moving turtle left " << ((NumberValue *)(firstExpression->getValue()))->getDouble() << " degrees" << endl;
			break;
		case yy::Parser::token::RIGHTTURN:
			cout << "Moving turtle right " << ((NumberValue *)(firstExpression->getValue()))->getDouble() << " degrees" << endl;
			break;
		case yy::Parser::token::SETHEADING:
			cout << "Set heading: " << ((NumberValue *)(firstExpression->getValue()))->getDouble() << endl;
			break;
		case yy::Parser::token::PENUP:
			cout << "Pen is up" << endl;
			break;
		case yy::Parser::token::PENDOWN:
			cout << "Pen is down" << endl;
			break;
		case yy::Parser::token::SHOWTURTLE:
			cout << "Now showing turtle" << endl;
			break;
		case yy::Parser::token::HIDETURTLE:
			cout << "Now hiding turtle" << endl;
			break;
	}
}

void AST_Parameter::print(std::ostream& os) {
    param->print(os);
}

ExpressionType AST_FunctionCall::getType(){
	// Lookup in string table
	SymbolTableEntry *symbol = Driver::symbolTable.LookupSymbol(functionName->entry);
	assert(symbol != 0);

	return symbol->GetType();
}

Value *AST_FunctionCall::getValue(){
	AST_NewCommand *function = ((FunctionPointerValue *)functionName->entry->value->get())->getPointer();
	return function->call(firstArgument);
}

void AST_FunctionCall::execute(){
	getValue(); // Simply executes, doesn't store the return value anywhere
}

/* Executed only once at the global level */
SemanticCheckResult AST_Statements::checkForSemanticErrors()
{
	SemanticCheckResult result = SemanticCheckSuccess;

	Driver::symbolTable.EnterScope();

    AST_Statement *current = first;
    while(current != 0){
      SemanticCheckResult statementCheckResult = current->checkForSemanticErrors();
	  if (statementCheckResult == SemanticCheckFailed){
		  result = SemanticCheckFailedAndLineNumberPrinted;
		  cout << "on line " << current->GetLineNumber() << endl;
	  }else if (statementCheckResult == SemanticCheckFailedAndLineNumberPrinted){
		  result = SemanticCheckFailedAndLineNumberPrinted;
	  }

		  current = current->next;
    }

    AddReferencesToActivationRecord(globalActivationRecord);
	Driver::symbolTable.ExitScope();

	return result;
}

void AST_Statements::execute()
{
	// Reset random seed
	srand ( time(NULL) );

	globalActivationRecord.EnterScope();

	AST_Statement *current = first;
	while(current != 0){
		try{
			current->execute();
		}catch(const RuntimeException &e){
			// If an exception occurs, add the line number to the description and throw it again
			std::stringstream error;

			error << e.what() << " on line " << current->GetLineNumber();
			throw RuntimeException(error.str());
		}
		current = current->next;
	}

	globalActivationRecord.ExitScope();
}

void AddReferencesToActivationRecord(ActivationRecord &ar){
	list<StringTableEntry *> activeEntries = Driver::symbolTable.GetEntriesInCurrentScopeLevel();

    for (list<StringTableEntry *>::iterator listItem = activeEntries.begin(); listItem != activeEntries.end(); listItem++){
    	ar.AddReference(*listItem);
    	//cout << *(*listItem)->GetName() << endl;
    }
}
