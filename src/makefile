LINK_TARGET = KLogo

OBJS = KLogo.o Parser.o Scanner.o Driver.o ScannerException.o StringTable.o StringTableEntry.o AST.o SymbolTable.o SymbolTableEntry.o Value.o ActivationRecord.o ValueStack.o RuntimeException.o FunctionTerminatedException.o

DERIVED = Parser.cc Scanner.cc

REBUILDABLES = $(OBJS) $(LINK_TARGET) $(DERIVED)

all: $(LINK_TARGET)
	echo All done

$(LINK_TARGET): $(OBJS)
	g++ $(ARGS) -g -o $@ $^

Scanner.cc: Scanner.ll
	flex --outfile Scanner.cc Scanner.ll
		
Parser.cc: Parser.yy AST.hh
	bison -r all --language=c++ --output Parser.cc Parser.yy

Parser.hh: Parser.yy AST.hh
	bison -r all --language=c++ --output Parser.cc Parser.yy

Driver.o: Driver.cc Parser.hh Driver.hh AST.hh SymbolTable.hh StringTable.hh
	g++ -c Driver.cc

ScannerException.o: ScannerException.cc ScannerException.hh
	g++ -c ScannerException.cc

StringTable.o: StringTable.cc StringTable.hh StringTableEntry.hh
	g++ -c StringTable.cc

StringTableEntry.o: StringTableEntry.cc StringTableEntry.hh Parser.hh
	g++ -c StringTableEntry.cc
	
SymbolTable.o: SymbolTable.cc SymbolTable.hh SymbolTableEntry.hh StringTableEntry.hh
	g++ -c SymbolTable.cc	
	
Value.o: Value.cc Value.hh
	g++ -c Value.cc	
	
ValueStack.o: ValueStack.cc ValueStack.hh Value.hh
	g++ -c ValueStack.cc	

ActivationRecord.o: ActivationRecord.cc ActivationRecord.hh
	g++ -c ActivationRecord.cc
		
RuntimeException.o: RuntimeException.cc RuntimeException.hh
	g++ -c RuntimeException.cc

FunctionTerminatedException.o: FunctionTerminatedException.cc FunctionTerminatedException.hh
	g++ -c FunctionTerminatedException.cc	

SymbolTableEntry.o: SymbolTableEntry.cc SymbolTableEntry.hh
	g++ -c SymbolTableEntry.cc

Scanner.o: Scanner.cc Parser.hh Driver.hh AST.hh
	g++ -c Scanner.cc 

Parser.o: Parser.cc Parser.hh Driver.hh AST.hh
	g++ -c Parser.cc

KLogo.o: KLogo.cc Parser.hh Driver.hh AST.hh ValueStack.hh
	g++ -c KLogo.cc

AST.o: AST.cc AST.hh Parser.hh ActivationRecord.hh Value.hh
	g++ -c AST.cc

clean:
	rm -f $(REBUILDABLES)
	echo Clean done
