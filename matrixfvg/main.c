#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "inout.h"
#include "mmuvelet.h"
#ifdef _WIN32
    #include <windows.h>
#endif

int main()
{
    #ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
    #endif
    MX A;
    MX B;
    char fajlA[] = "matrixA.txt";
    char fajlB[] = "matrixB.txt";

    printf("\nÜdvözöllek a mátrix-programban!\nSzámolj egy szabadon választott mátrixszal!\n");
    int menuvezer = 0;
    do{
        menuvezer = Menu();
        switch(menuvezer){
        case 1: BeolvasKiir(&A, fajlA); break;
        case 2: BeolvasKiir(&B, fajlB); break;
        case 3: BeolvasFajl(&A, fajlA); BeolvasFajl(&B, fajlB); Osszeadas(A, B); break;
        case 4: BeolvasFajl(&A, fajlA); BeolvasFajl(&B, fajlB); Kivonas(A, B); break;
        case 5: BeolvasFajl(&A, fajlA); SzorzasValossal(A); break;
        case 6: BeolvasFajl(&B, fajlB); SzorzasValossal(B); break;
        case 7: BeolvasFajl(&A, fajlA); BeolvasFajl(&B, fajlB); Szorzas(A,B); break;
        case 8: BeolvasFajl(&A, fajlA); BeolvasFajl(&B, fajlB); Szorzas(B,A); break;
        case 9: BeolvasFajl(&A, fajlA); Determinans(A); break;
        case 10: BeolvasFajl(&B, fajlB); Determinans(B); break;
        case 11: printf("\nKilépés\n"); break;
        default: printf("\nMég nincs megcsinálva\n"); break;
        }
    }while(menuvezer!=11);
}
