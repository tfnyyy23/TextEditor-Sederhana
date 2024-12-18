#ifndef TUBES_H_INCLUDED
#define TUBES_H_INCLUDED
#define NIL NULL
#include <iostream>

using namespace std;

typedef string infotype;
typedef struct Elmlist *address;

struct Elmlist {
    infotype info;
    address next;
    address prev;
};

struct List {
    address first;
    address last;
};

struct Stack {
    address top;
};

struct Action {
    string type;    // Jenis tindakan: "delete", "insert"
    string data;    // Data yang dihapus/dipulihkan
    int position;   // Posisi elemen pada list
};

void createList(List &L);
address createNewElement(infotype data);
void insertFirst(List &L, address P);
void insertLast(List &L, address P);
void insertAfter(List &L, address P, address pred);
void deleteFirst(List &L, address &P);
void deleteLast(List &L, address &P);
void deleteAfter(List &L, address &P, address pred);
Stack createStack();
bool isEmpty(Stack s);
Action createAction(string type, string data, int position);
void push(Stack &s, Action action);
Action pop(Stack &s);
Action peek(Stack s);
void display(List L);
address searchCharacter(List L, infotype karakter);
void performUndo(Stack &undoStack, Stack &redoStack, List &L);
void performRedo(Stack &redoStack, Stack &undoStack, List &L);
int listSize(List L);
address getElementAt(List &L, int position);
void deleteAt(List &L, int position);
void insertAt(List &L, address P, int position);
void deleteFirstWithUndo(List &L, Stack &undoStack);

#endif // TUBES_H_INCLUDED
