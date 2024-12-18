#include <iostream>
#include "tubes.h"

using namespace std;

void createList(List &L) {
    L.first = NIL;
    L.last = NIL;
}

address createNewElement(infotype data) {
    address P = new Elmlist;
    P->info = data;
    P->next = NIL;
    P->prev = NIL;
    return P;
}

void insertFirst(List &L, address P) {
    if (L.first != NIL) {
        P->next = L.first;
        L.first->prev = P;
        L.first = P;
    } else {
        L.first = P;
        L.last = P;
    }
}

void insertLast(List &L, address P) {
    if (L.first != NIL) {
        P->prev = L.last;
        L.last->next = P;
        L.last = P;
    } else {
        L.first = P;
        L.last = P;
    }
}

void insertAfter(List &L, address P, address pred) {
    if (pred != NIL) {
        P->next = pred->next;
        P->prev = pred;
        if (pred->next != NIL) {
            pred->next->prev = P;
        } else {
            L.last = P;
        }
        pred->next = P;
    }
}

void deleteFirst(List &L, address &P) {
    if (L.first != NIL) {
        P = L.first;
        L.first = P->next;
        if (L.first != NIL) {
            L.first->prev = NIL;
        } else {
            L.last = NIL;
        }
        P->next = NIL;
        P->prev = NIL;
    }
}

void deleteLast(List &L, address &P) {
    if (L.last != NIL) {
        P = L.last;
        L.last = P->prev;
        if (L.last != NIL) {
            L.last->next = NIL;
        } else {
            L.first = NIL;
        }
        P->prev = NIL;
        P->next = NIL;
    }
}

void deleteAfter(List &L, address &P, address pred) {
    if (pred != NIL && pred->next != NIL) {
        P = pred->next;
        pred->next = P->next;
        if (P->next != NIL) {
            P->next->prev = pred;
        } else {
            L.last = pred;
        }
        P->next = NIL;
        P->prev = NIL;
    }
}

Stack createStack() {
    Stack s;
    s.top = NIL;
    return s;
}

bool isEmpty(Stack s) {
    return s.top == NIL;
}

void push(Stack &s, Action action) {
    address P = createNewElement(action.data);
    P->info = action.type + "|" + action.data + "|" + to_string(action.position);
    P->next = s.top;
    s.top = P;
}

Action pop(Stack &s) {
    Action action = {"", "", -1};
    if (s.top != NIL) {
        address P = s.top;
        s.top = s.top->next;
        string temp = P->info;
        size_t delim1 = temp.find("|");
        size_t delim2 = temp.rfind("|");
        action.type = temp.substr(0, delim1);
        action.data = temp.substr(delim1 + 1, delim2 - delim1 - 1);
        action.position = stoi(temp.substr(delim2 + 1));
        delete P;
    }
    return action;
}

Action peek(Stack s) {
    Action action = {"", "", -1};
    if (s.top != NIL) {
        string temp = s.top->info;
        size_t delim1 = temp.find("|");
        size_t delim2 = temp.rfind("|");
        action.type = temp.substr(0, delim1);
        action.data = temp.substr(delim1 + 1, delim2 - delim1 - 1);
        action.position = stoi(temp.substr(delim2 + 1));
    }
    return action;
}

void display(List L) {
    address P = L.first;
    while (P != NIL) {
        cout << P->info << " ";
        P = P->next;
    }
    cout << endl;
}

address searchCharacter(List L, infotype karakter) {
    address current = L.first;
    while (current != NIL) {
        if (current->info == karakter) {
            return current;
        }
        current = current->next;
    }
    return NIL;
}

Action createAction(string type, string data, int position) {
    Action newAction;
    newAction.type = type;
    newAction.data = data;
    newAction.position = position;
    return newAction;
}

void performUndo(Stack &undoStack, Stack &redoStack, List &L) {
    if (isEmpty(undoStack)) {
        cout << "  | Tidak ada aksi untuk di-undo." << endl;
        return;
    }

    // Ambil aksi terakhir dari undoStack
    Action lastAction = pop(undoStack);
    Action redoAction;

    if (lastAction.type == "insert") {
        // Jika aksi terakhir adalah insert, hapus elemen
        deleteAt(L, lastAction.position);

        // Simpan kebalikan ke redo stack
        redoAction = createAction("delete", lastAction.data, lastAction.position);
        push(redoStack, redoAction);
        cout << "  | Undo berhasil: elemen '" << lastAction.data
             << "' dihapus dari posisi " << lastAction.position << "." << endl;
    } else if (lastAction.type == "delete") {
        // Jika aksi terakhir adalah delete, kembalikan elemen ke posisi semula
        address P = createNewElement(lastAction.data);
        insertAt(L, P, lastAction.position);

        // Simpan kebalikan ke redo stack
        redoAction = createAction("insert", lastAction.data, lastAction.position);
        push(redoStack, redoAction);
        cout << "  | Undo berhasil: elemen '" << lastAction.data
             << "' dikembalikan ke posisi " << lastAction.position << "." << endl;
    }
}

void performRedo(Stack &redoStack, Stack &undoStack, List &L) {
    if (isEmpty(redoStack)) {
        cout << "  | Tidak ada aksi untuk di-redo." << endl;
        return;
    }

    Action action = pop(redoStack);
    if (action.type == "insert") {
        address target = getElementAt(L, action.position);
        deleteAt(L, action.position);
        push(undoStack, {"insert", action.data, action.position});
    } else if (action.type == "delete") {
        address newElement = createNewElement(action.data);
        insertAt(L, newElement, action.position);
        push(undoStack, {"delete", action.data, action.position});
    }
    cout << "  | Redo berhasil dilakukan." << endl;
}

int listSize(List L) {
    int count = 0;
    address P = L.first;
    while (P != NIL) {
        count++;
        P = P->next;
    }
    return count;
}

address getElementAt(List &L, int position) {
    address P = L.first;
    int idx = 0;
    while (P != NIL && idx < position) {
        P = P->next;
        idx++;
    }
    return P;
}

void deleteAt(List &L, int position) {
    if (L.first == NIL) {
        cout << "  | List kosong!" << endl;
        return;
    }

    if (position == 0) {
        // Hapus elemen pertama jika posisi adalah 0
        address P;
        deleteFirst(L, P);
        delete P;
    } else {
        address current = L.first;
        int idx = 0;

        // Traversal ke posisi sebelum elemen yang dihapus
        while (current != NIL && idx < position - 1) {
            current = current->next;
            idx++;
        }

        if (current != NIL && current->next != NIL) {
            // Hapus elemen setelah current
            address P;
            deleteAfter(L, P, current);
            delete P;
        } else {
            cout << "  | Posisi tidak valid!" << endl;
        }
    }
}

void insertAt(List &L, address P, int position) {
    if (L.first == NIL || position == 0) {
        // Jika list kosong atau posisi 0, tambahkan di awal
        insertFirst(L, P);
    } else {
        address current = L.first;
        int idx = 0;

        // Traversal ke posisi yang diinginkan
        while (current != NIL && idx < position - 1) {
            current = current->next;
            idx++;
        }

        if (current == NIL) {
            // Jika posisi melebihi jumlah elemen, tambahkan di akhir
            insertLast(L, P);
        } else {
            // Menyisipkan elemen setelah posisi yang ditemukan
            insertAfter(L, P, current);
        }
    }
}

// Fungsi untuk menghapus elemen pertama dengan dukungan undo
void deleteFirstWithUndo(List &L, Stack &undoStack) {
    if (L.first == NIL) { // Jika list kosong
        cout << "  | List kosong!" << endl;
        return;
    }

    address P = L.first; // Ambil elemen pertama
    string deletedData = P->info; // Simpan informasi elemen
    int deletedPos = 0; // Posisi elemen pertama selalu 0

    deleteFirst(L, P); // Hapus elemen pertama dari list

    // Buat aksi delete dan simpan ke undo stack
    Action action = createAction("delete", deletedData, deletedPos);
    push(undoStack, action);

    delete P; // Hapus memori elemen
}
