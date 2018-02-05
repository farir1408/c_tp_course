#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define ERROR "[error]"
#define MUL 2
#define STACK_TYPE(type)                                                                    \
    typedef struct {                                                                        \
        type* root;                                                                         \
        type* last;                                                                         \
        size_t size;                                                                        \
        size_t capacity;                                                                    \
    } STACKT##type;                                                                         \
                                                                                            \
    STACKT##type* stackInit##type() {                                                       \
        STACKT##type* st = (STACKT##type*) malloc(sizeof(STACKT##type));                    \
        if (st == NULL)                                                                     \
            return NULL;                                                                    \
        st->root = (type*) malloc(sizeof(type) * 100);                                      \
        st->last = NULL;                                                                    \
        st->size = 0;                                                                       \
        st->capacity = 100;                                                                 \
        return st;                                                                          \
    }                                                                                       \
                                                                                            \
    void copy##type(type* src, type* dst, size_t count) {                                   \
        if (src == NULL || dst == NULL) {                                                   \
            return;                                                                         \
        }                                                                                   \
                                                                                            \
        const type* tmpVal1 = src;                                                          \
        type* tmpVal2 = dst;                                                                \
                                                                                            \
        for (size_t i = 0; i < count; i++) {                                                \
            *(tmpVal2 + i) = *(tmpVal1 + i);                                                \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    void push##type(STACKT##type* src, type value) {                                        \
        if (src == NULL)                                                                    \
            return;                                                                         \
                                                                                            \
        size_t newSize = src->size + 1;                                                     \
        int dif = src->capacity - newSize;                                                  \
        if (dif <= 0) {                                                                     \
            resize##type(src);                                                              \
            src->size = newSize;                                                            \
            src->last = src->root + newSize;                                                \
            *(src->last - 1) = value;                                                       \
        } else {                                                                            \
            src->size = newSize;                                                            \
            src->last = src->root + newSize;                                                \
            *(src->last - 1) = value;                                                       \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    type pop##type(STACKT##type* src) {                                                     \
        if (src == NULL)                                                                    \
            return;                                                                         \
        src->size = src->size - 1;                                                          \
        type res = *(src->last - 1);                                                        \
        src->last = src->last - 1;                                                          \
        return res;                                                                         \
    }                                                                                       \
                                                                                            \
                                                                                            \
    void resize##type(STACKT##type* src) {                                                  \
        if (src == NULL)                                                                    \
            return;                                                                         \
        size_t newCapacity = src->capacity * MUL;                                           \
        type newStack = (type*) malloc(sizeof(type) * newCapacity);                         \
                                                                                            \
        if (newStack == NULL || src->root == NULL)                                          \
            return;                                                                         \
                                                                                            \
        copy##type(src->root, newStack, src->size + 1);                                     \
        free(src->root);                                                                    \
        src->root = newStack;                                                               \
        src->capacity = newCapacity;                                                        \
    }                                                                                       \
                                                                                            \
    void stackDestr##type(STACKT##type* src) {                                              \
        if (src != NULL) {                                                                  \
            if (src->root != NULL)                                                          \
                free(src->root);                                                            \
                                                                                            \
            src->root = NULL;                                                               \
            src->last = NULL;                                                               \
            src->size = 0;                                                                  \
            src->capacity = 0;                                                              \
                                                                                            \
            free(src);                                                                      \
            src = NULL;                                                                     \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    bool isEmpty##type(STACKT##type* src) {                                                 \
        return src->size == 0;                                                              \
    }                                                                                       \
    size_t getSize##type(STACKT##type* src) {                                               \
        return src->size;                                                                   \
    }                                                                                       \
                                                                                            \
    type getTop##type(STACKT##type* src) {                                                  \
        return *(src->last - 1);                                                            \
    }                                                                                       \

STACK_TYPE(char)
STACK_TYPE(int)

#define STACK(type) STACKT##type
#define CONSTRUCTOR(type) stackInit##type()
#define PUSH(type, stack, value) push##type(stack, value)
#define POP(type, stack) pop##type(stack)
#define DESTRUCTOR(type, stack) stackDestr##type(stack)
#define ISEMPTY(type, stack) isEmpty##type(stack)
#define SIZE(type, stack) getSize##type(stack)
#define GETTOP(type, stack) getTop##type(stack)

bool isSymvol(char c) {
    char symbols[] = "0123456789[](),U^\\";

    for (size_t i = 0; i < sizeof(symbols); i++) {
        if (c == symbols[i])
            return true;
    }
    return false;
}

bool isOperation(char c) {
    return c == 'U' || c == '^' || c == '\\';
}

int priority(char c) {
    return c == 'U' || c == '\\' ? 1 : c == '^' ? 2 : -1;
}

void getSet(STACK(int)* set, STACK(int)* part) {
    size_t size = POP(int, set);

    for (size_t i = 0; i < size; i++) {
        PUSH(int, part, POP(int, set));
    }
}

bool isIn(int value, STACK(int)* set) {

    for (size_t i = 0; i < SIZE(int, set); i++) {
        if (value == set->root[i])
            return true;
    }

    return false;
}

int association(STACK(int)* set, int value) {

    for (size_t i = 0; i < SIZE(int, set); i++) {
        if (set->root[i] == value)
            return 1;
    }

    return 0;
}

void Process(STACK(int)* set, char c) {
    STACK(int)* left = CONSTRUCTOR(int);
    STACK(int)* right = CONSTRUCTOR(int);
    STACK(int)* buf = CONSTRUCTOR(int);

    getSet(set, right);
    getSet(set, left);

    switch (c) {
        case 'U':
            for (size_t i = 0; i < SIZE(int, right); i++) {
                if (association(left, right->root[i]) == 0)
                    PUSH(int, left, right->root[i]);
            }

            for (size_t i = 0; i < SIZE(int, left); i++) {
                PUSH(int, set, left->root[i]);
            }
            PUSH(int, set, (int)SIZE(int, left));

            break;

        case '\\':
            for (size_t i = 0; i < SIZE(int, left); i++) {
                if (association(right, left->root[i]) == 0)
                    PUSH(int, set, left->root[i]);
            }
            PUSH(int, set, (int)SIZE(int, left));
            break;

        case '^':
            for (size_t i = 0; i < SIZE(int, left); i++) {
                for (size_t j = 0; j < SIZE(int, right); i++) {
                    if (left->root[i] == right->root[i] && !isIn(left->root[i], buf))
                        PUSH(int, buf, left->root[i]);
                }
            }
            for (size_t i = 0; i < SIZE(int, buf); i++) {
                PUSH(int, set, buf->root[i]);
            }
            PUSH(int, set, (int)SIZE(int, buf));
            break;

        default: break;
    }

    DESTRUCTOR(int, right);
    DESTRUCTOR(int, left);
    DESTRUCTOR(int, buf);
}

int main() {
    STACK(char)* expr = CONSTRUCTOR(char);
    STACK(char)* scob = CONSTRUCTOR(char);
    char c;

    while (true) {
        c = getchar();

        if (c == '\n')
            break;

        if (c == ' ')
            continue;

        if (!isSymvol(c)) {
            printf("%s",ERROR);
            DESTRUCTOR(char, expr);
            DESTRUCTOR(char, scob);
            return 0;
        }

        PUSH(char, expr, c);

        if (c == '(')
            PUSH(char, scob, c);
        else if (c == ')')
            POP(char, scob);
    }

    if(!ISEMPTY(char, scob)) {
        printf("%s", ERROR);
        DESTRUCTOR(char, expr);
        DESTRUCTOR(char, scob);
        return 0;
    }

    STACK(int)* set = CONSTRUCTOR(int);
    STACK(char)* operations = CONSTRUCTOR(char);
    DESTRUCTOR(char, scob);
    size_t setSize = 0;
    for (size_t i = 0; i < SIZE(char, expr); i++) {
        if (expr->root[i] == '(') {
            PUSH(char, operations, expr->root[i]);
        } else if (expr->root[i] == ')') {
            while (GETTOP(char, operations) != '(') {
                Process(set, POP(char, operations));
            }
            POP(char, operations);
        } else if (isOperation(expr->root[i])) {
            while (!ISEMPTY(char, operations) &&
                   (expr->root[i] >= 0 && priority(GETTOP(char, operations) >= priority(expr->root[i])))) {
                Process(set, POP(char, operations));
            }
            PUSH(char, operations, expr->root[i]);
        } else if (isdigit(expr->root[i])) {
            setSize++;
            STACK(char)* values = CONSTRUCTOR(char);

            while (i < SIZE(char, expr) && isdigit(expr->root[i])) {
                PUSH(char, values, expr->root[i++]);
            }
            PUSH(char, values, '\0');

            if (isdigit(values->root[0])) {
                char* tmp = values->root;
                PUSH(int, set, atoi(tmp));
            }
            if (expr->root[i] == ']') {
                PUSH(int, set, setSize);
                setSize = 0;
            }

            DESTRUCTOR(char, values);

        } else if (expr->root[i] == '[' && expr->root[i + 1] == ']') {
            PUSH(int, set, 0);
            i++;
        }
    }

    while (!ISEMPTY(char, operations)) {
        Process(set, POP(char, operations));
    }

    POP(int, set);
    //TODO sort

    STACK(int)* result = CONSTRUCTOR(int);

    for (size_t i = 0; i < SIZE(int, set); i++) {
        if (!isIn(set->root[i], result))
            PUSH(int, result, set->root[i]);
    }
    printf("[");

    for (size_t i = 0; i < SIZE(int, result); i++) {
        printf("%d,", result->root[i]);
    }

    printf("]");

    DESTRUCTOR(int, result);
    DESTRUCTOR(int, set);
    DESTRUCTOR(char, operations);
    DESTRUCTOR(char, expr);

}
