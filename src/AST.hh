#ifndef AST_HH
#define AST_HH
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include "StringTableEntry.hh"
#include "Value.hh"
#include "ValueStack.hh"
#include "ActivationRecord.hh"
#include "RuntimeException.hh"
#include "FunctionTerminatedException.hh"

using namespace std;

typedef enum { TypeCheckSuccess, TypeCheckFailed } TypeCheckResult;
typedef enum { NameCheckSuccess, NameCheckFailed } NameCheckResult;

typedef enum { SemanticCheckSuccess, SemanticCheckFailed, SemanticCheckFailedAndLineNumberPrinted } SemanticCheckResult;

class AST_Node {
  public:
	virtual SemanticCheckResult checkForSemanticErrors(){
		 SemanticCheckResult result = SemanticCheckSuccess;

		 if (runNameChecking() == NameCheckFailed){
			 result = SemanticCheckFailed;
		 }
		 else if (runTypeChecking() == TypeCheckFailed){
			 result = SemanticCheckFailed;
		 }

		 return result;
	}

    virtual void print(std::ostream& os = std::cout) {}
  	virtual TypeCheckResult runTypeChecking() { return TypeCheckSuccess; }
  	virtual NameCheckResult runNameChecking() { return NameCheckSuccess; }

  	virtual void execute(){ };
  	virtual Value *getValue(){ assert(false); };

  	/* A few helper methods */
  	void reportError(const string& msg){
  		cout << msg;
  	}
  	void reportTypeError(ExpressionType expected, ExpressionType given){
  		reportError(typeToString(expected) + " expected but " + typeToString(given) + " was given ");
  	}

  	string typeToString(ExpressionType type){
  		switch(type){
			case TypeVoid: return "void";
			case TypeString: return "string";
			case TypeNumber: return "number";
			case TypeBool: return "bool";
			case TypeList: return "list";
			case TypeGeneric: return "generic";
  		}
  	}


};

class AST_Expression : public AST_Node {
public:

	virtual ExpressionType getType() { assert(false); } //Shall never be executed, must be overriden
};

class AST_Number : public AST_Expression  {
  private:
    AST_Number();
    Value *number;
  public:

    AST_Number(double n) { number = (Value *)(new NumberValue(n)); }
    ~AST_Number() { delete number; }
    void print(std::ostream& os = std::cout) { number->print(os); }

    ExpressionType getType(){ return TypeNumber; }
    Value *getValue(){ return number; }
};

class AST_String : public AST_Expression  {
  private:
    AST_String() {}

  public:
    Value *str;

    AST_String(const string &str) { this->str = (Value *)(new StringValue(str)); }
    ~AST_String() { delete str; }
    void print(std::ostream& os = std::cout) { os << "\""; str->print(os); os << "\""; }

    ExpressionType getType(){	return TypeString;   }
    Value *getValue(){ return str; }
};

class AST_Bool : public AST_Expression  {
  private:
	AST_Bool() {}

  public:
    Value *value;

    AST_Bool(short int value) { this->value = (Value *)(new BoolValue(value)); }
    ~AST_Bool() { delete value; }
    void print(std::ostream& os = std::cout) { value->print(os); }

    ExpressionType getType(){ 	return TypeBool;  }
    Value *getValue(){ return value; }
};

class AST_ListElement : public AST_Expression {
  private:
	AST_ListElement() {}

  public:
	AST_Expression *expression;
	AST_ListElement *next;

	AST_ListElement(AST_Node *value) {
      this->expression = (AST_Expression *)value;
      next = 0;
    }
	AST_ListElement(AST_Node *value, AST_Node *next) {
      this->expression = (AST_Expression *)value;
      this->next = (AST_ListElement *)next;
    }
    ~AST_ListElement() { delete expression; delete next; }
    void print(std::ostream& os = std::cout) {
    	expression->print(os);
    }

    NameCheckResult runNameChecking(){	return expression->runNameChecking();  }

    ExpressionType getType(){ 	return expression->getType();   }
    Value *getValue(){ return expression->getValue(); }
};

class AST_List : public AST_Expression  {
  private:
	AST_List() {}

  public:
	AST_ListElement *first;
	Value *list;

    AST_List(AST_Node *first) {
    	this->first = (AST_ListElement *)first;
    	this->list = 0;
    }
    ~AST_List() { delete first; delete list; }
    void print(std::ostream& os = std::cout) {
    	AST_ListElement *current = first;
    	os << "[";
    	while(current != 0){
            os << " ";
    		current->print(os);
  		    current = current->next;
        }
    	os << " ]";
    }

  	NameCheckResult runNameChecking() {
  		NameCheckResult result = NameCheckSuccess;

    	AST_ListElement *current = first;
    	while(current != 0){

          if (current->runNameChecking() == NameCheckFailed) result = NameCheckFailed;
  		  current = current->next;
        }

  		return result;
  	}


    ExpressionType getType(){ 	return TypeList;  }
    Value *getValue() {
		// Create list value
		ListValue *list = new ListValue();

		// Populate the list
		AST_ListElement *current = first;
		while (current != 0){
			list->addItem(current->getValue());
			current = current->next;
		}

		// Assign
		this->list = (Value *)list;

    	return this->list;
    }
};

class AST_Identifier : public AST_Expression {
  private:
    AST_Identifier() {}

  public:
    StringTableEntry *entry;

    AST_Identifier(StringTableEntry *entry) { this->entry = entry; }
    ~AST_Identifier() {}
    void print(std::ostream& os = std::cout) { os << *entry->GetName(); }

    NameCheckResult runNameChecking();

    ExpressionType getType();
    Value *getValue(){ return entry->value->get(); }
};

class AST_Assignment : public AST_Node {
  private:
    AST_Assignment() {}

  public:
    AST_Identifier *identifier;
    AST_Expression *expression;

    AST_Assignment(AST_Node *var, AST_Node *expr) {
      identifier = (AST_Identifier *) var;
      expression = (AST_Expression *)expr;
    }

    ~AST_Assignment() { delete identifier; delete expression; }
    void print(std::ostream& os = std::cout) {
      os << "make ";
      identifier->print(os);
      os << " ";
      expression->print(os);
      os << " ;";
    }

    NameCheckResult runNameChecking();
    TypeCheckResult runTypeChecking();
  	void execute(){
  		identifier->entry->value->set(expression->getValue());
  	};
};

class AST_Statement : public AST_Node {
  private:
	AST_Statement() {}
    int lineNumber;
  public:
    AST_Node *value;
    AST_Statement *next;


    AST_Statement(AST_Node *value, int lineNumber) {
      this->value = value;
      this->lineNumber = lineNumber;
      next = 0;
    }
    ~AST_Statement() { delete value; delete next; }
    void print(std::ostream& os = std::cout) {
    	value->print(os);
    }

    int GetLineNumber(){
    	return lineNumber;
    }

    SemanticCheckResult checkForSemanticErrors(){
    	return value->checkForSemanticErrors();
    }

    void execute(){
    	return value->execute();
    }
};

class AST_Statements : public AST_Node {
  private:
	AST_Statements() {}
	ActivationRecord globalActivationRecord;

  public:
    AST_Statement *first;

    AST_Statements(AST_Node *first) {
      this->first = (AST_Statement *)first;
    }
    ~AST_Statements() { delete first; }
    void print(std::ostream& os = std::cout) {
      AST_Statement *current = first;
      while(current != 0){
    	  current->print(os);
		  current = current->next;
		  os << endl;
      }
    }

    SemanticCheckResult checkForSemanticErrors();
    void execute();

};

class AST_Argument : public AST_Expression {
  private:
	AST_Argument() {}

  public:
    AST_Expression *expression;
    AST_Argument *next;

    AST_Argument(AST_Node *expression) {
      this->expression = (AST_Expression *)expression;
      next = 0;
    }
    ~AST_Argument() { delete expression; delete next; }
    void print(std::ostream& os = std::cout) {
    	expression->print(os);
    }

  	SemanticCheckResult checkForSemanticErrors() {
		return expression->checkForSemanticErrors();
  	}

    ExpressionType getType(){ 	return expression->getType();  }
    Value* getValue(){ return expression->getValue(); }

};

class AST_VariableDeclaration : public AST_Node {
  private:
	AST_VariableDeclaration() {}

  public:
    ExpressionType type;
    AST_Identifier* identifier;

    AST_VariableDeclaration(ExpressionType type, AST_Node *identifier) {
      this->type = type;
      this->identifier = (AST_Identifier *)identifier;
    }

    ~AST_VariableDeclaration() { delete identifier; }
    void print(std::ostream& os);
    NameCheckResult runNameChecking();
	ExpressionType getType() { return type; }
};

class AST_Parameter : public AST_Node {
  private:
	AST_Parameter() {}

  public:
	AST_VariableDeclaration *param;
	AST_Parameter *next;

	AST_Parameter(AST_Node *param) {
      this->param = (AST_VariableDeclaration *)param;
      this->next = 0;
    }
    ~AST_Parameter() { delete param; delete next; }
    void print(std::ostream& os = std::cout);

    NameCheckResult runNameChecking(){
    	return param->runNameChecking();
    }
};

class AST_FunctionCall : public AST_Expression {
  private:
	AST_FunctionCall() {}

  public:
    AST_Argument* firstArgument;
    AST_Identifier* functionName;

    AST_FunctionCall(AST_Node *functionName, AST_Node *firstArgument) {
      this->functionName = (AST_Identifier *)functionName;
      this->firstArgument = (AST_Argument *)firstArgument;
    }

    ~AST_FunctionCall() { delete firstArgument; delete functionName; }
    void print(std::ostream& os = std::cout) {

      functionName->print(os);
      os << " ( ";
      AST_Argument *current = firstArgument;

      while(current != 0){
        current->print(os);
        current = current->next;
        os << " ";
      }

      os << ") ; ";
    }

  	NameCheckResult runNameChecking() {
  		NameCheckResult result = NameCheckSuccess;
        if (functionName->runNameChecking() == NameCheckFailed) result = NameCheckFailed;

        AST_Argument *current = firstArgument;
        while(current != 0){
          if (current->runNameChecking() == NameCheckFailed) result = NameCheckFailed;
  		  current = current->next;
        }

  		return result;
  	}

  	ExpressionType getType();
  	void execute();
  	Value *getValue();
};

class AST_SysFunctionCall : public AST_Expression {
  private:
	AST_SysFunctionCall() {}

  public:
    AST_Expression* firstExpression;
    AST_Expression* secondExpression;

    int token; // token::SYSPRINT, etc.
    ExpressionType readType; // Used specifically for the read function

    AST_SysFunctionCall(int token) {
      this->token = token;
      this->firstExpression = (AST_Expression *)0;
      this->secondExpression = (AST_Expression *)0;
    }

    AST_SysFunctionCall(int token, ExpressionType readType) {
      this->token = token;
      this->readType = readType;
      this->firstExpression = (AST_Expression *)0;
      this->secondExpression = (AST_Expression *)0;
    }

    AST_SysFunctionCall(int token, AST_Node *firstExpression) {
      this->token = token;
      this->firstExpression = (AST_Expression *)firstExpression;
      this->secondExpression = (AST_Expression *)0;
    }


    AST_SysFunctionCall(int token, AST_Node *firstExpression, AST_Node *secondExpression) {
      this->token = token;
      this->firstExpression = (AST_Expression *)firstExpression;
      this->secondExpression = (AST_Expression *)secondExpression;
    }

    ~AST_SysFunctionCall() { delete firstExpression; delete secondExpression; }
    void print(std::ostream& os = std::cout);

  	NameCheckResult runNameChecking() {
  		NameCheckResult result = NameCheckSuccess;

  		if (firstExpression != 0)
  			if (firstExpression->runNameChecking() == NameCheckFailed) result = NameCheckFailed;

        if (secondExpression != 0)
        	if (secondExpression->runNameChecking() == NameCheckFailed) result = NameCheckFailed;

  		return result;
  	}

  	TypeCheckResult runTypeChecking();
  	ExpressionType getType();
  	Value *getValue();
  	void execute();
};

class AST_CodeBlock : public AST_Node {
  private:
	AST_CodeBlock() {}

  public:
    AST_Statement* firstStatement;

    AST_CodeBlock(AST_Node *firstStatement) {
      this->firstStatement = (AST_Statement *)firstStatement;
    }

    ~AST_CodeBlock() { delete firstStatement; }
    void print(std::ostream& os = std::cout) {
      static int indentation_level = 0;
      AST_Statement *current = firstStatement;

      os << "{" << endl;
      indentation_level++;

      while(current != 0){
    	printIndentation(indentation_level, os);
        current->print(os);
        current = current->next;
        os << endl;
      }

      indentation_level--;
      printIndentation(indentation_level, os);
      os << "}";

    }

    void printIndentation(int indentation_level, std::ostream& os = std::cout){
    	for (int i = 0; i<indentation_level; i++) os << "  ";
    }

    /* No increase of scope level. This is taken care of by the parent structure */
    SemanticCheckResult checkForSemanticErrors(){
    	SemanticCheckResult result = SemanticCheckSuccess;

        AST_Statement *current = firstStatement;
        while(current != 0){
          SemanticCheckResult statementResult = current->checkForSemanticErrors();

          if (statementResult == SemanticCheckFailed){
        	  result = SemanticCheckFailedAndLineNumberPrinted;
        	  cout << "on line " << current->GetLineNumber() << endl;
          }else if (statementResult == SemanticCheckFailedAndLineNumberPrinted){
        	  result = SemanticCheckFailedAndLineNumberPrinted;
          }

          current = current->next;
        }

  		return result;
    }

    void execute(){
        AST_Statement *current = firstStatement;
        while(current != 0){
          current->execute();
          current = current->next;
        }
    }

};

class AST_IfStatement : public AST_Node {
  private:
	AST_IfStatement() {}
    ActivationRecord activationRecord;
  public:
    AST_CodeBlock* ifBlock;
    AST_Expression *expression;


    AST_IfStatement(AST_Node *ifBlock, AST_Node *expression) {
      this->ifBlock = (AST_CodeBlock *)ifBlock;
      this->expression = (AST_Expression *)expression;
    }

    ~AST_IfStatement() { delete ifBlock; delete expression; }
    void print(std::ostream& os){
    	os << "if ";
    	expression->print(os);
    	ifBlock->print(os);
    }

    SemanticCheckResult checkForSemanticErrors();
    void execute();
};

class AST_IfElseStatement : public AST_Node {
  private:
	AST_IfElseStatement() {}
    ActivationRecord ifActivationRecord;
    ActivationRecord elseActivationRecord;
  public:
    AST_CodeBlock* ifBlock;
    AST_CodeBlock* elseBlock;
    AST_Expression *expression;



    AST_IfElseStatement(AST_Node *ifBlock, AST_Node *elseBlock, AST_Node *expression) {
      this->ifBlock = (AST_CodeBlock *)ifBlock;
      this->elseBlock = (AST_CodeBlock *)elseBlock;
      this->expression = (AST_Expression *)expression;
    }

    ~AST_IfElseStatement() { delete ifBlock; delete elseBlock; delete expression; }
    void print(std::ostream& os){
    	os << "ifelse ";
    	expression->print(os);
    	ifBlock->print(os);
   		elseBlock->print(os);
    }

    SemanticCheckResult checkForSemanticErrors();
    void execute();
};

class AST_WhileStatement : public AST_Node {
  private:
	AST_WhileStatement() {}
	ActivationRecord activationRecord;
  public:
    AST_CodeBlock* body;
    AST_Expression *expression;

    AST_WhileStatement(AST_Node *body, AST_Node *expression) {
      this->body = (AST_CodeBlock *)body;
      this->expression = (AST_Expression *)expression;
    }

    ~AST_WhileStatement() { delete body; delete expression; }
    void print(std::ostream& os){
    	os << "while ";
    	expression->print(os);
    	body->print(os);
    }

    SemanticCheckResult checkForSemanticErrors();
    void execute();
};

//for { VariableName StartExpr StopExpr [ StepExpr ] } { CommandList }
class AST_ForStatement : public AST_Node {
  private:
	AST_ForStatement() {}
    ActivationRecord activationRecord;
  public:
	AST_Identifier *identifier;
    AST_CodeBlock* body;
    AST_Expression *startExpression;
    AST_Expression *stopExpression;
    AST_Expression *stepExpression;


    AST_ForStatement(AST_Node *identifier, AST_Node *startExpression, AST_Node *stopExpression, AST_Node *stepExpression, AST_Node *body) {
	  this->startExpression = (AST_Expression *)startExpression;
	  this->stopExpression = (AST_Expression *)stopExpression;
	  this->stepExpression = (AST_Expression *)stepExpression;
      this->body = (AST_CodeBlock *)body;
      this->identifier = (AST_Identifier *)identifier;
    }

    ~AST_ForStatement() { delete body; delete startExpression; delete stopExpression; delete stepExpression; delete identifier; }
    void print(std::ostream& os){
    	os << "for ";
    	identifier->print(os);
    	os << " ";
    	startExpression->print(os);
    	os << " ";
    	stopExpression->print(os);
    	if (stepExpression != 0){
        	os << " ";
    		stepExpression->print(os);
    	}
    	body->print(os);
    }

    SemanticCheckResult checkForSemanticErrors();
    void execute();
};

// newCommand: varType id OPENPARENTHESES parameters CLOSEPARENTHESES codeBlock { $$ = (AST_Node *) (new AST_NewCommand($1, $2, $4, $6)); };
class AST_NewCommand : public AST_Node {
  private:
	AST_NewCommand() {}

  public:
    ExpressionType type;
    AST_Identifier* commandId;
    AST_Parameter* firstParameter;
    AST_CodeBlock* body;
    ActivationRecord activationRecord;

    AST_NewCommand(ExpressionType type, AST_Node *commandId, AST_Node *firstParameter, AST_Node *body) {
      this->type = type;
      this->commandId = (AST_Identifier *)commandId;
      this->firstParameter = (AST_Parameter *)firstParameter;
      this->body = (AST_CodeBlock *)body;
    }

    ~AST_NewCommand() { delete commandId; delete firstParameter; delete body; }
    void print(std::ostream& os);

    SemanticCheckResult checkForSemanticErrors();
    ExpressionType getType();
    void execute();

    Value* call(AST_Argument *firstArgument);
};

class AST_ReturnCommand : public AST_Expression {
  private:
	AST_ReturnCommand() {}

  public:
    AST_Expression* expression;

    AST_ReturnCommand(AST_Node *expression) {
		this->expression = (AST_Expression *)expression;
    }
    ~AST_ReturnCommand() { delete expression; }
    void print(std::ostream& os = std::cout){
    	os << "return ";
    	if (expression != 0) expression->print(os);
    }

    NameCheckResult runNameChecking() {
    	if (expression != 0) return expression->runNameChecking();
    	else return NameCheckSuccess;
    }

    ExpressionType getType(){
    	if (expression != 0) return expression->getType();
    	else return TypeVoid;
    }

    void execute(){
    	// When a return statement is called, we throw a functionterminatedexception
    	throw FunctionTerminatedException(expression->getValue());
    }
};

class AST_NegationOp : public AST_Expression {
  private:
	AST_NegationOp() {}

  public:
    AST_Expression* expression;

    AST_NegationOp(AST_Node *expression) {
		this->expression = (AST_Expression *)expression;
    }
    ~AST_NegationOp() { delete expression; }
    void print(std::ostream& os = std::cout){
    	os << "not ";
    	expression->print(os);
    }

    NameCheckResult runNameChecking() { return expression->runNameChecking(); }
    TypeCheckResult runTypeChecking(){
    	if (expression->getType() != TypeBool){
    		reportError("bool required to negate but " + typeToString(expression->getType()) + " was given ");
    		return TypeCheckFailed;
    	}
    	return TypeCheckSuccess;
    }

    ExpressionType getType(){  return TypeBool;  }
    Value *getValue(){
    	BoolValue *value = (BoolValue *)expression->getValue();
    	return (Value *)new BoolValue(!value->getBool());
    }
};


class AST_BinaryOp : public AST_Expression {
  private:
    AST_BinaryOp() {}

  public:
    AST_Expression* left;
    AST_Expression* right;
    int token_number;

    AST_BinaryOp(int tok_num, AST_Node *lft, AST_Node *rgt) {
      token_number = tok_num;
      left = (AST_Expression *)lft;
      right = (AST_Expression *)rgt;
    }
    ~AST_BinaryOp() { delete left; delete right; }
    void print(std::ostream& os = std::cout);
    NameCheckResult runNameChecking();
    TypeCheckResult runTypeChecking();

    ExpressionType getType();
    Value *getValue();
};

// Helper method
void AddReferencesToActivationRecord(ActivationRecord &ar);

#endif
