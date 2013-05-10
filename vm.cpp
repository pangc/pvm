#include "vm.h"
#include "object.h"
#include<string.h>
#include <iostream>

VMachine::VMachine(){
    for(int i=0;i<MAX_OBJ;i++)
        FreeIndex.insert(i);
	instr = NULL;
}


VMachine::~VMachine(){
	Reset();
}

void VMachine::Reset(){
    ip =0;
	if(!operstack.empty())
        operstack.pop();
}
void VMachine::Read(){
    instr = new Instr[16];
    Value vnum;
    vnum.num = 50;
    Value vbool;
    vbool.b = 1;
    InsertObj(0,NUMBER,vnum);
    InsertObj(1,NUMBER,vnum);
    InsertObj(2,NUMBER,vnum);
    InsertObj(3,NUMBER,vnum);
    InsertObj(4,NUMBER,vnum);
    InsertObj(5,NUMBER,vnum);
    InsertObj(6,NUMBER,vnum);
    InsertObj(7,BOOLEAN,vbool);
    instr[0] = Instr(OP_PUSH,0);      //50
    instr[1] = Instr(OP_PUSH,1);      //50
    instr[2] = Instr(OP_ADD);       //100
    instr[3] = Instr(OP_JMP,4);
    instr[4] = Instr(OP_NOP);
    instr[5] = Instr(OP_NOP);
    instr[6] = Instr(OP_NOP);
    instr[7] = Instr(OP_NOP);
    instr[8] = Instr(OP_PUSH,2);      //50
    instr[9] = Instr(OP_PUSH,3);
    instr[10] = Instr(OP_MUL);
    instr[11] = Instr(OP_DIV);
    instr[12] = Instr(OP_PUSH,4);      //50
    instr[13] = Instr(OP_PUSH,5);
    instr[14] = Instr(OP_LT);      //50
    instr[15] = Instr(OP_PUSH,6);
}
void VMachine::Execute(){
	int step=0;
	int index = -1;
	int index1,index2;
	ip = 0;
    void *op_lables[13];
    goto PREPARE;
LABEL_NOP:
    goto NEXT_INSTR;
LABEL_PUSH:
    operstack.push(instr[ip]._operand);
    goto NEXT_INSTR;
LABEL_POP:
     index = operstack.top();
     operstack.pop();
     FreeIndex.insert(index);
    goto NEXT_INSTR;
LABEL_GETTOP:
    index = operstack.top();
    goto NEXT_INSTR;
LABEL_JMP:
    step = instr[ip]._operand;
    goto NEXT_INSTR;
LABEL_IFJMP:
    index= operstack.top();
    operstack.pop();
    if(ObjTable[index].t == BOOLEAN)
        if(ObjTable[index].v.b ==1)
            step = instr[ip]._operand;
    goto NEXT_INSTR;
LABEL_ADD:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t)
        vmerror("diffrent type to add\n");
    else{
        if(ObjTable[index1].t == NUMBER){
            ObjTable[index1].v.num = ObjTable[index1].v.num + ObjTable[index2].v.num;
            operstack.push(index1);
            FreeIndex.insert(index2);
        }
        else if(ObjTable[index1].t = STRING){
            ObjTable[index1].v.str = strcat(ObjTable[index1].v.str,ObjTable[index2].v.str);
            operstack.push(index1);
            FreeIndex.insert(index2);
        }
        else{
            vmerror("can not add this type\n");
        }
    }
    goto NEXT_INSTR;
LABEL_SUB:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t)
        vmerror("diffrent type to add\n");
    else{
        if(ObjTable[index1].t == NUMBER){
            ObjTable[index1].v.num = ObjTable[index1].v.num + ObjTable[index2].v.num;
            operstack.push(index1);
            FreeIndex.insert(index2);
        }
        else{
            vmerror("can not add this type\n");
        }
    }
    goto NEXT_INSTR;
LABEL_MUL:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t)
        vmerror("diffrent type to mul\n");
    else if(ObjTable[index1].t != NUMBER)
        vmerror("can not multiply non-numeric\n");
    else{
        ObjTable[index1].v.num = ObjTable[index1].v.num * ObjTable[index2].v.num;
        operstack.push(index1);
        FreeIndex.insert(index2);
    }
    goto NEXT_INSTR;
LABEL_DIV:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t)
        vmerror("diffrent type to mul\n");
    else if(ObjTable[index1].t != NUMBER)
        vmerror("can not multiply non-numeric\n");
    else{
        ObjTable[index1].v.num = ObjTable[index1].v.num / ObjTable[index2].v.num;
        operstack.push(index1);
        FreeIndex.insert(index2);
    }
    goto NEXT_INSTR;
LABEL_EQ:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t){
           vmerror("diffrent type");
    }
    else{
        switch(ObjTable[index1].t){
            case NUMBER:
                if(ObjTable[index1].v.num == ObjTable[index1].v.num)
                    PushBoolean(1);         //1表示真，压入栈vmstark
                else
                    PushBoolean(0);           //0表示假
                break;
            case STRING:
                if(strcmp(ObjTable[index1].v.str,ObjTable[index2].v.str)==0)
                    PushBoolean(1);
                else
                    PushBoolean(0);
                break;
            case BOOLEAN:
                if(ObjTable[index1].v.b == ObjTable[index1].v.b)
                    PushBoolean(1);         //1表示真，压入栈vmstark
                else
                    PushBoolean(0);           //0表示假
                break;
        }
    }
    goto NEXT_INSTR;
LABEL_LT:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t){
           vmerror("diffrent type");
    }
    else{
        switch(ObjTable[index1].t){
            case NUMBER:
                if(ObjTable[index1].v.num < ObjTable[index1].v.num)
                    PushBoolean(1);         //1表示真，压入栈vmstark
                else
                    PushBoolean(0);           //0表示假
                break;
            case STRING:
                if(strcmp(ObjTable[index1].v.str,ObjTable[index2].v.str)<0)
                    PushBoolean(1);
                else
                    PushBoolean(0);
                break;
            case BOOLEAN:
                PushNil();
                break;
        }
    }
    goto NEXT_INSTR;
LABEL_GT:
    index1 = operstack.top();
    operstack.pop();
    index2 = operstack.top();
    operstack.pop();
    if(ObjTable[index1].t != ObjTable[index2].t){
           vmerror("diffrent type");
    }
    else{
        switch(ObjTable[index1].t){
            case NUMBER:
                if(ObjTable[index1].v.num > ObjTable[index1].v.num)
                    PushBoolean(1);         //1表示真，压入栈vmstark
                else
                    PushBoolean(0);           //0表示假
                break;
            case STRING:
                if(strcmp(ObjTable[index1].v.str,ObjTable[index2].v.str)>0)
                    PushBoolean(1);
                else
                    PushBoolean(0);
                break;
            case BOOLEAN:
                PushNil();
                break;
        }
    }
    goto NEXT_INSTR;
LABLE_UNKNOWN:
    vmerror("unknown opcode");
    goto NEXT_INSTR;
PREPARE:
    op_lables[0] = &&LABEL_NOP;
    op_lables[1] = &&LABEL_PUSH;
    op_lables[2] = &&LABEL_POP;
    op_lables[3] = &&LABEL_GETTOP;
    op_lables[4] = &&LABEL_JMP;
    op_lables[5] = &&LABEL_IFJMP;
    op_lables[6] = &&LABEL_ADD;
    op_lables[7] = &&LABEL_SUB;
    op_lables[8] = &&LABEL_MUL;
    op_lables[9] = &&LABEL_DIV;
    op_lables[10] = &&LABEL_EQ;
    op_lables[11] = &&LABEL_LT;
    op_lables[12] = &&LABEL_GT;
NEXT_INSTR:
    if(ip > 14) goto END;
    ip = ip+step;
    step = 1;
  //  showFree();
    showStack();
//    showObj();
    if(instr[ip]._opcode<256 || instr[ip]._opcode>(257+13)) goto LABLE_UNKNOWN;
    goto *op_lables[instr[ip]._opcode-257];
END:
	getchar();
}
void VMachine::showFree(){
    printf("\nip= %d\t",ip);
    for(std::set<int>::iterator it = FreeIndex.begin();it!=FreeIndex.end();it++)
        printf("%d|",*it);
}
void VMachine::showStack(){
    int o;
    OperandStack tmpStack;
    while(!operstack.empty()){
        o = operstack.top();
        operstack.pop();
        printf("ip = %d\t",ip);
        printObj(ObjTable[o]);
        printf("\n");
        tmpStack.push(o);
    }
    while(!tmpStack.empty()){
        operstack.push(tmpStack.top());
        tmpStack.pop();
    }
}

void VMachine::printObj(obj o){
    switch(o.t){
        case NUMBER:
            printf("number = %d",o.v.num);
            break;
        case STRING:
            printf("string = %s",o.v.str);
            break;
        case BOOLEAN:
            printf("boolean = %d",o.v.b);
            break;
        case NIL:
            printf("nil");
            break;
        default:
            printf("unkown");
    }
}

void VMachine::showObj(){
    for(int index = 0;index<MAX_OBJ;index++)
    if(ObjTable[index].t==NUMBER||ObjTable[index].t==BOOLEAN)
        printObj(ObjTable[index]);
}

/*
while(ip < 16){
	    step = 1;
		switch(instr[ip]._opcode){
			case OP_NOP:
				break;
			case OP_PUSH:
                o1= (obj *)malloc(sizeof(obj));
                o1->t = NUMBER;
                o1->v.num = 50;
                vmstack.Push(o1);
				break;
            case OP_POP:
                tvalue = vmstack.Pop();
                break;
            case OP_GETTOP:
                tvalue = vmstack.GetTop();
                break;
            case OP_JMP:
                step = instr[ip]._operand;
                break;
            case OP_IFJMP:
                o1= vmstack.Pop();
                if(o1->t == BOOLEAN)
                    if(o1->v.b ==1)
                        step = instr[ip]._operand;
                break;
            case OP_ADD:
                o1 = vmstack.Pop();
                o2 = vmstack.Pop();
                if(o1->t != o2->t)
                    vmerror("diffrent type to add\n");
                else{
                    if(o1->t == NUMBER){
                        o1->v.num = o1->v.num + o2->v.num;
                        vmstack.Push(o1);
                    }
                    else if(o1->t = STRING){
                        o1->v.str = strcat(o1->v.str,o2->v.str);
                        vmstack.Push(o1);
                    }
                    else{
                        vmerror("can not add this type\n");
                    }
                }
                break;
            case OP_SUB:
                o1 = vmstack.Pop();
                o2 = vmstack.Pop();
                if(o1->t != o2->t)
                    vmerror("diffrent type to sub\n");
                else{

                    if(o1->t!=NUMBER)
                        vmerror("can not sub other type");
                    else{
                        o1->v.num = o1->v.num - o2->v.num;
                        vmstack.Push(o1);
                    }
                }
                break;
            case OP_MUL:
                o1 = vmstack.Pop();
                o2 = vmstack.Pop();
                if(o1->t != o2->t)
                    vmerror("diffrent type to mul\n");
                else if(o1->t != NUMBER)
                    vmerror("can not multiply non-numeric\n");
                else{
                    o1->v.num = o1->v.num * o2->v.num;
                    vmstack.Push(o1);
                }
                break;
            case OP_DIV:
                o1 = vmstack.Pop();
                o2 = vmstack.Pop();
                if(o1->t != o2->t)
                    vmerror("diffrent type to sub\n");
                else if(o1->t != NUMBER)
                    vmerror("can not multiply non-numeric\n");
                else{
                    if(o2->v.num!=0){
                        o1->v.num = o1->v.num / o2->v.num;
                        vmstack.Push(o1);
                    }
                    else
                        vmerror("div error\n");
                }
                break;
            case OP_EQ:
                o1 = vmstack.Pop();
                o2 = vmstack.Pop();
               if(o1->t != o2->t){
                       vmerror("diffrent type");
                }
                else{
                    switch(o1->t){
                        case NUMBER:
                            if(o1->v.num == o2->v.num)
                                PushBoolean(1);         //1表示真，压入栈vmstark
                            else
                                PushBoolean(0);           //0表示假
                            break;
                        case STRING:
                            if(strcmp(o1->v.str,o2->v.str)==0)
                                PushBoolean(1);
                            else
                                PushBoolean(0);
                            break;
                        case BOOLEAN:
                            if(o1->v.b == o2->v.b)
                                PushBoolean(1);         //1表示真，压入栈vmstark
                            else
                                PushBoolean(0);           //0表示假
                            break;
                    }
                }
                break;
            case OP_LT:
                o1 = vmstack.Pop();
                o2 = vmstack.Pop();
               if(o1->t != o2->t){
                   vmerror("diffrent type");
                }
                else{
                    switch(o1->t){
                        case NUMBER:
                            if(o1->v.num < o2->v.num)
                                PushBoolean(1);      //1表示真，压入栈vmstark
                            else
                                PushBoolean(0);           //0表示假
                            break;
                        case STRING:
                            if(strcmp(o1->v.str,o2->v.str)<0)
                                PushBoolean(1);
                            else
                                PushBoolean(0);
                            break;
                        case BOOLEAN:
                                Pushnil();
                            break;
                    }
                }
                break;
            case OP_GT:
                    o1 = vmstack.Pop();
                    o2 = vmstack.Pop();
               if(o1->t != o2->t){
                        vmerror("diffrent type");
                }
                else{
                    switch(o1->t){
                        case NUMBER:
                            if(o1->v.num > o2->v.num)
                                PushBoolean(1);         //1表示真，压入栈vmstark
                            else
                                PushBoolean(0);           //0表示假
                            break;
                        case STRING:
                            if(strcmp(o1->v.str,o2->v.str)>0)
                                PushBoolean(1);
                            else
                                PushBoolean(0);
                            break;
                        case BOOLEAN:
                                Pushnil();
                            break;
                    }
                }
                break;
            default :
                vmerror("error instruction");
		}

    showStack();
    ip=ip+step;
   // printf("ip=%d",ip);
	}
*/
