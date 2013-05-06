#include "stack.h"
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
typedef Stack<obj *> VMStack;         //index of object sstable


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
	void showStack();
private:
    inline void Pushnil(){
        obj *o3 = (obj *)malloc(sizeof(obj));
        o3->t = NIL;
        vmstack.Push(o3);
    }
    inline void PushBoolean(int b){
        obj *o3 = (obj *)malloc(sizeof(obj));
        o3->t = BOOLEAN;
        o3->v.b = b;
        vmstack.Push(o3);
    }
    obj *tvalue;
	Instr *instr;
	VMStack vmstack;
	obj ObjTable[MAX_OBJ];
	int ip;
};


#endif // VM_H_INCLUDED
