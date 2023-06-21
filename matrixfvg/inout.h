#ifndef INOUT_H_INCLUDED
#define INOUT_H_INCLUDED

typedef struct MX{
        double **matrix;
        int sor, oszlop;
    }MX;

void Felszabadit(MX *matrix);
int Menu();
void BeolvasKiir(MX *matrix, char *fajlNev);
void BeolvasFajl(MX *matrix, char *fajlNev);
double SzamBeolvas(int min, int max, bool van_tart, char *utast1_szoveg,char *utast2_szoveg);

#endif // INOUT_H_INCLUDED
