#include <iostream>
#include "tubes.h"

using namespace std;

int main() {
    List L;
    Stack undoStack = createStack();
    Stack redoStack = createStack();
    address P;
    string data;
    int pilihan;

    // Membuat List baru
    createList(L);

    // Menampilkan menu
    do {
        cout << " +=============================+";
        cout << "\n |           M E N U           |";
        cout << "\n +=============================+";
        cout << "\n | 1 |  Tambah Kalimat         |";
        cout << "\n | 2 |  Cari Kata              |";
        cout << "\n | 3 |  Hapus Kata             |";
        cout << "\n | 4 |  Sisipkan Kata          |";
        cout << "\n | 5 |  Menampilkan teks       |";
        cout << "\n | 6 |  Undo                   |";
        cout << "\n | 7 |  Redo                   |";
        cout << "\n | 0 |  Exit                   |";
        cout << "\n +=============================+";
        cout << "\n  >> Pilih menu: ";
        cin >> pilihan;

        if (pilihan == 1) {
            // Tambah kalimat
            cout << "  | Masukkan kalimat (akhiri dengan titik '.'):" << endl;
            cout << "  >> ";
            do {
                cin >> data;
                P = createNewElement(data);
                insertLast(L, P);

                // Simpan tindakan ke undo stack
                push(undoStack, createAction("insert", data, listSize(L) - 1));

                // Kosongkan redo stack karena ada tindakan baru
                redoStack = createStack();
            } while (data[data.length() - 1] != '.');
        }

        else if (pilihan == 2) {
            // Cari kata
            cout << "  | Masukkan kata yang ingin dicari: ";
            cout << "\n  >> ";
            cin >> data;
            address found = searchCharacter(L, data);
            if (found != NIL) {
                cout << "  | Kata '" << data << "' ditemukan." << endl;
            } else {
                cout << "  | Kata '" << data << "' tidak ditemukan." << endl;
            }
        }

        else if (pilihan == 3) {
            cout << "  | Masukkan kata yang ingin dihapus: ";
            cout << "\n  >> ";
            cin >> data;
            address found = searchCharacter(L, data);
            if (found != NIL) {
                int pos = 0;
                address current = L.first;

                // Menentukan posisi elemen yang dihapus
                while (current != found) {
                    pos++;
                    current = current->next;
                }

                deleteAt(L, pos);

                // Simpan aksi ke undo stack
                push(undoStack, createAction("delete", data, pos));
                redoStack = createStack();  // Kosongkan redo stack

                cout << "  | Kata '" << data << "' telah dihapus dari posisi " << pos << "." << endl;
            } else {
                cout << "  | Kata '" << data << "' tidak ditemukan." << endl;
            }
        }

        else if (pilihan == 4) {
            // Tambahkan kata di posisi tertentu
            cout << "  | Masukkan kata yang ingin ditambahkan: ";
            cout << "\n  >> ";
            cin >> data;
            cout << "  | Masukkan posisi (1 untuk awal, 2 untuk akhir, ";
            cout << "\n  | atau ketik lainnya untuk sisipkan setelah kata):";
            cout << "\n  >> ";
            int pos;
            cin >> pos;

            P = createNewElement(data);
            if (pos == 1) {
                insertFirst(L, P);
            } else if (pos == 2) {
                insertLast(L, P);
            } else {
                cout << "  | Tambahkan setelah kata apa? ";
                cout << "\n  >> ";
                string predData;
                cin >> predData;
                address pred = searchCharacter(L, predData);
                if (pred != NIL) {
                    insertAfter(L, P, pred);
                } else {
                    cout << "  | Kata '" << predData << "' tidak ditemukan. Tambah ke akhir." << endl;
                    insertLast(L, P);
                }
            }

            // Simpan tindakan ke undo stack
            Action action = createAction("insert", data, listSize(L));
            push(undoStack, action);

            // Kosongkan redo stack
            redoStack = createStack();
        }

        else if (pilihan == 5) {
            // Menampilkan teks
            cout << "  | Tampilan Teks:";
            cout << "\n  >> ";
            display(L);
        }

        else if (pilihan == 6) {
            // Undo
            performUndo(undoStack, redoStack, L);
            cout << "  | Tampilan Teks:";
            cout << "\n  >> ";
            display(L);

            // Simpan tindakan ke undo stack
            //Action action = createAction("insert", data, listSize(L));
            //push(undoStack, action);
        }

        else if (pilihan == 7) {
            // Redo
            performRedo(redoStack, undoStack, L);
            cout << "  | Tampilan Teks:";
            cout << "\n  >> ";
            display(L);
        }

        else if (pilihan == 0) {
            // Keluar
            cout << " +=============================+";
            cout << "\n |.......Keluar  Program.......|";
            cout << "\n +=============================+";
        }

        else {
            cout << "  | Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);

    return 0;
}
