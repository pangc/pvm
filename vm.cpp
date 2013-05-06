#include "vm.h"
#include "object.h"
#include<string.h>
#include <iostream>

VMachine::VMachine(){
	instr = NULL;
}


VMachine::~VMachine(){
	Reset();
}

void VMachine::Reset(){
    ip =0;
	int count = vmstack.Len();
		if(count>0)
			for(;count >0;count--)
				vmstack.Pop();
}
void VMachine::Read(){
    instr = new Instr[16];
    instr[0] = Instr(OP_PUSH);      //50
    instr[1] = Instr(OP_PUSH);      //50
    instr[2] = Instr(OP_ADD);       //100
    instr[3] = Instr(OP_JMP,4);
    instr[4] = Instr(OP_NOP);
    instr[5] = Instr(OP_NOP);
    instr[6] = Instr(OP_NOP);
    instr[7] = Instr(OP_NOP);
    instr[8] = Instr(OP_PUSH);      //50
    instr[9] = Instr(OP_PUSH);
    instr[10] = Instr(OP_MUL);
    instr[11] = Instr(OP_DIV);
    instr[12] = Instr(OP_PUSH);      //50
    instr[13] = Instr(OP_PUSH);
    instr[14] = Instr(OP_LT);      //50
    instr[15] = Instr(OP_PUSH);
}
void VMachine::Execute(){
	obj *o1,*o2;
	int step=0;
	ip = 0;
    void *op_lables[13];
    goto PREPARE;
LABEL_NOP:
    goto NEXT_INSTR;
LABEL_PUSH:
    o1= (obj *)malloc(sizeof(obj));
    o1->t = NUMBER;
    o1->v.num = 50;
    vmstack.Push(o1);
    goto NEXT_INSTR;
LABEL_POP:
    tvalue = vmstack.Pop();
    goto NEXT_INSTR;
LABEL_GETTOP:
    tvalue = vmstack.Pop();
    goto NEXT_INSTR;
LABEL_JMP:
    step = instr[ip]._operand;
    goto NEXT_INSTR;
LABEL_IFJMP:
    o1= vmstack.Pop();
    if(o1->t == BOOLEAN)
        if(o1->v.b ==1)
            step = instr[ip]._operand;
    goto NEXT_INSTR;
LABEL_ADD:
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
    goto NEXT_INSTR;
LABEL_SUB:
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
    goto NEXT_INSTR;
LABEL_MUL:
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
    goto NEXT_INSTR;
LABEL_DIV:
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
    goto NEXT_INSTR;
LABEL_EQ:
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
    goto NEXT_INSTR;
LABEL_LT:
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
    goto NEXT_INSTR;
LABEL_GT:
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
    if(ip > 15) goto END;
    ip = ip+step;
    step = 1;
    showStack();
    if(instr[ip]._opcode<256 || instr[ip]._opcode>(257+13)) goto LABLE_UNKNOWN;
    goto *op_lables[instr[ip]._opcode-257];
END:
	getchar();
}

void VMachine::showStack(){
    obj *o;
    VMStack tmpStack;
    while(!vmstack.IsEmpty()){
        o = vmstack.Pop();
        printf("ip = %d\t type = %d\tdata = %d\n",ip,o->t,o->v.num);
        tmpStack.Push(o);
    }
    while(!tmpStack.IsEmpty()){
        vmstack.Push(tmpStack.Pop());
    }
    tmpStack.Empty();
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
