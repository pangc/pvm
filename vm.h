#include <stack>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include "object.h"
#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#define vmerror(x)  printf(x)


enum Opcode{
 	OP_NOP = 257,
 	OP_PUSH,
 	OP_POP,
 	OP_GETTOP,
 	OP_JMP,
 	OP_IFJMP,
 	OP_ADD,
 	OP_SUB,
 	OP_MUL,
 	OP_DIV,
 	OP_EQ,
 	OP_LT,
 	OP_GT
};



typedef TValue obj;
typedef std::stack<int> OperandStack;         //index of object sstable


class Instr{
public:
	Instr(){};
	Instr(Opcode opcode)
	{
		_opcode = opcode;
	}
	Instr(Opcode opcode,int operand)
	{
		_opcode = opcode;
		_operand = operand;
	}
	Opcode _opcode;
	int _operand;
};

class  VMachine{
public:
	VMachine();
	~VMachine();
	void Read();
	void Execute();
	void Reset();
private:
    obj *tvalue;
	Instr *instr;
	OperandStack operstack;
	obj ObjTable[MAX_OBJ];
	std::set< int > FreeIndex;
	int ip;
	inline void PushBoolean(int b){
	    int index = GetFreeIndex();
	    ObjTable[index].t = BOOLEAN;
	    ObjTable[index].v.b = b;
	    FreeIndex.erase(index);
        operstack.push(index);
	}
	inline void PushNil(){
	    int index = GetFreeIndex();
	    ObjTable[index].t = NIL;
	    FreeIndex.erase(index);
        operstack.push(index);

	}
	inline int GetFreeIndex(){
        std::set<int>::iterator it = FreeIndex.begin();
        return *it;
	}
	inline void InsertObj(int index,int t,Value v)
	{
	    ObjTable[index].t = t;
	    switch(t){
	        case NUMBER:
                ObjTable[index].v.num = v.num;
                break;
            case STRING:
                ObjTable[index].v.str = v.str;
                break;
            case BOOLEAN:
                ObjTable[index].v.b = v.b;
	    }
	    FreeIndex.erase(index);
	}
	void showFree();
    void showStack();
    void showObj();
    void printObj(obj);
};


#endif // VM_H_INCLUDED
