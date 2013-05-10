#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#define MAX_OBJ 16

enum DataType{
    BOOLEAN,
    NUMBER,
    NIL,
    STRING
};

typedef union Value{
    int b;  /* booleans */
    int num; /* number */
    char *str;
} Value;

typedef struct TValue{
    Value v;
    int t;
}TValue;

#define assignTV(obj,src)  \
    {TValue *io2=(obj2); TValue *io1=(obj1);    \
     io1->v = io2->v;io1->t = io2->t; \
    }
#endif // OBJECT_H_INCLUDED
