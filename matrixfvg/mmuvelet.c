#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "inout.h"
#include "mmuvelet.h"

/*Eldonti, hogy feltoltesre kerult-e a parameterkent kapott matrix
 *@parametere stukturalt tipusu matrix
 *@Visszateresi erteke logikai IGAZ, ha feltoltesre kerult
*/
static bool feltoltottMX(MX matrix){
    if(matrix.matrix != NULL)
        return true;
    return false;
}


/*Eldonti, hogy a parameterkent kapott matrixok azonos meretuek-e
 *@parameterei az ellenorizendo matrixok
 *@Visszateresi erteke logikai IGAZ, ha azonosak meretuek.
*/
static bool azonosTipusuak(MX matrixA, MX matrixB){
    if(matrixA.sor == matrixB.sor && matrixA.oszlop == matrixB.oszlop)
        return true;
    return false;
}


/*Megjeleniti a parameterkent kapott matrixot a kepernyore
 *@Parametere egy strukturalt matrix
*/
static void megjelenit(MX matrix){
    printf("\nA mátrix %dsoros %doszlopos, értékei:\n", matrix.sor, matrix.oszlop);
    for(int i = 0; i<matrix.sor; ++i){
        for(int j = 0; j<matrix.oszlop; ++j){
            printf("%g",matrix.matrix[i][j]);
            if(j!=matrix.oszlop-1)
                printf("\t");
        }
        printf("\n");
    }
}


/*Osszeadja a legutobb feltoltott ket matrixot
 *@parameterei stukturalt tipusu matrixok
*/
void Osszeadas(MX matrixA, MX matrixB){
    bool felszabaditott = false;
    if(feltoltottMX(matrixA) && feltoltottMX(matrixB)){
        if(azonosTipusuak(matrixA, matrixB)){
            MX osszeg;
            osszeg.sor = matrixA.sor;
            osszeg.oszlop = matrixA.oszlop;
            osszeg.matrix = (double**)malloc(osszeg.sor*sizeof(double*));
            for(int x = 0; x<osszeg.sor; ++x){
                osszeg.matrix[x] = (double*)malloc(osszeg.oszlop*sizeof(double));
                for(int y = 0; y<osszeg.oszlop; ++y)
                    osszeg.matrix[x][y] = matrixA.matrix[x][y] + matrixB.matrix[x][y];
            }
            megjelenit(osszeg);
            Felszabadit(&osszeg);
        }else
            printf("\nNem adhatóak össze.\n");
        Felszabadit(&matrixA);
        Felszabadit(&matrixB);
        felszabaditott = true;
    }else
        printf("\nNincsenek feltöltve a mátrixok.\n");
    if((feltoltottMX(matrixA) || feltoltottMX(matrixB)) && !felszabaditott){
        if(feltoltottMX(matrixA))
            Felszabadit(&matrixA);
        if(feltoltottMX(matrixB))
            Felszabadit(&matrixB);
    }
}


/*Kivonja a legutobb feltoltott ket matrixot egymasbol
 *@parameterei stukturalt tipusu matrixok
*/
void Kivonas(MX matrixA, MX matrixB){

    bool felszabaditott = false;
    if(feltoltottMX(matrixA) && feltoltottMX(matrixB)){
        if(azonosTipusuak(matrixA, matrixB)){
            MX osszeg;
            osszeg.sor = matrixA.sor;
            osszeg.oszlop = matrixA.oszlop;
            osszeg.matrix = (double**)malloc(osszeg.sor*sizeof(double*));
            for(int x = 0; x<osszeg.sor; ++x){
                osszeg.matrix[x] = (double*)malloc(osszeg.oszlop*sizeof(double));
                for(int y = 0; y<osszeg.oszlop; ++y)
                    osszeg.matrix[x][y] = matrixA.matrix[x][y] - matrixB.matrix[x][y];
            }
            megjelenit(osszeg);
            Felszabadit(&osszeg);
        }else
            printf("\nNem vonhatóak ki egymásból.\n");
        Felszabadit(&matrixA);
        Felszabadit(&matrixB);
        felszabaditott = true;
    }else
        printf("\nNincsenek feltöltve a mátrixok.\n");
    if((feltoltottMX(matrixA) || feltoltottMX(matrixB)) && !felszabaditott){
        if(feltoltottMX(matrixA))
            Felszabadit(&matrixA);
        if(feltoltottMX(matrixB))
            Felszabadit(&matrixB);
    }
}


/*Beszorozza egy matrix elemeit egy a bemeneten kapott valos szammal
 *@parametere strukturalt tipusu matrix
*/
void SzorzasValossal(MX matrix){

    if(feltoltottMX(matrix)){
        double lambda = SzamBeolvas(0,0,false,"\nAdj meg egy valós számot!: ","");
        for(int x = 0; x<matrix.sor; ++x){
            for(int y = 0; y<matrix.oszlop; ++y)
                matrix.matrix[x][y] *= lambda;
        }
        megjelenit(matrix);
        Felszabadit(&matrix);
    }else
        printf("\nNincs feltöltve a mátrix.\n");
}


/*Ket matrix szorzasanal kiszamolja az egy elem erteket a parameterknek megfeleloen
 *@parameterkent varja az adott szorzat-matrix oszlop indexet, mely a B strukturalt matrix hasznal fel majd
 *@tovabba felhasznalja az elso szorzattenyezo (matrix) adott x.-edik soraban levo valos ertekeket
 *@tovabba felhasznalja a masodik szorzattenyezot (matrixot), hogy megallapitsa adott k.-adik soraban és j-edik oszlopaban tarolt elem erteket
 *@visszateresi erteke egy valos szam, mely a szorzat-matrix egy elemet jelenti.
*/
static double szorzatMatrixElem(int j, double *sorA, MX B){

    double ertek = 0;
    for(int k = 0; k<B.sor; ++k){
        ertek += sorA[k] * B.matrix[k][j];
    }
    return ertek;
}


/*Osszeszoroz ket matrixot egymassal
 *@parameterei stukturalt tipusu matrixok
*/
void Szorzas(MX matrixA, MX matrixB){
    bool felszabaditott = false;
    if(feltoltottMX(matrixA) && feltoltottMX(matrixB)){
        if(matrixA.oszlop == matrixB.sor){

            //Feltoltes - helyfoglalas
            MX szorzatMatrix;
            szorzatMatrix.sor = matrixA.sor;
            szorzatMatrix.oszlop = matrixB.oszlop;
            szorzatMatrix.matrix = (double**)malloc(matrixA.sor * sizeof(double*));
            for(int x = 0; x<matrixA.sor; ++x){
                szorzatMatrix.matrix[x] = (double*)malloc(matrixB.oszlop*sizeof(double));
                //Elemei kiszamitasa - A MATRIX SZORZAS
                for(int y = 0; y<matrixB.oszlop; ++y)
                    szorzatMatrix.matrix[x][y] = szorzatMatrixElem(y, matrixA.matrix[x], matrixB);
            }
            megjelenit(szorzatMatrix);
            Felszabadit(&szorzatMatrix);
        }
        else
            printf("\nNem szorozhatóak össze.\n");
        Felszabadit(&matrixA);
        Felszabadit(&matrixB);
        felszabaditott = true;
    }else
        printf("\nNincsenek feltöltve a mátrixok.\n");
    if((feltoltottMX(matrixA) || feltoltottMX(matrixB)) && !felszabaditott){
        if(feltoltottMX(matrixA))
            Felszabadit(&matrixA);
        if(feltoltottMX(matrixB))
            Felszabadit(&matrixB);
    }
}

/*A 'Determinans' fuggvenyhez nagyban hozzajarult a BSz hivatalos jegyzeteben (105.oldalon) talalhato pszeudokod*/

/*Parameterkent kapott sort leoszt a sor foatlojaban talalhato szammal vezeregyest kepezve
 *@parametere egy strukturalt matrix cime es a leosztando sor
*/
void sorOsztas(MX *matrix, int adottSor){
    double foatlo = matrix->matrix[adottSor][adottSor];
    for(int i = 0; i<matrix->oszlop; ++i)
        matrix->matrix[adottSor][i] /= foatlo;
}


/*Parameterkent kapott sor alatt levo sorokkal kivonast vegezve a vezeregyes alatti elemeket kinullazza
 *@parametere egy strukturalt matrix cime es a leosztando sor
*/
void sorKivonas(MX *matrix, int adottSor){
    for(int x = adottSor+1; x<matrix->sor; ++x){
        double xElsoElem = matrix->matrix[x][adottSor];
        for(int y = adottSor; y<matrix->oszlop; ++y)
            matrix->matrix[x][y] += -1*xElsoElem*matrix->matrix[adottSor][y];
    }
}


/*Ellenorzi, hogy a parameterkent kapott foatlo oszlopaban talalhato-e nullatol kulonb elem
 *@a fuggveny erteke logikai IGAZ, ha talhato, es ekkor...
 *@...visszateriti cim szerint a sor indexet a harmadik parameternek
*/
bool vanNemNullaElem(MX matrix, int adottSor, int *nemNullaSor){
    for(int x = adottSor+1; x<matrix.sor; ++x){
        if(matrix.matrix[x][adottSor]!=0){
            *nemNullaSor = x;
            return true;
        }
    }
    return false;
}


/*Sor cseret hajt vegre egy parameterkent kapott matrixban.
 *@tovabbi parameterei a felcserelendo matrix sorainak indexei
 *@a sor csere utan visszaadja cim szerint a modositott matrixot parameter listaban
*/
void sorCsere(MX *matrix, int adottSor, int nemNullaSor){
    double temp[matrix->sor];
    for(int i=0; i<matrix->oszlop; ++i)
        temp[i] = matrix->matrix[adottSor][i];
    for(int i=0; i<matrix->oszlop; ++i)
        matrix->matrix[adottSor][i] = matrix->matrix[nemNullaSor][i];
    for(int i=0; i<matrix->oszlop; ++i)
        matrix->matrix[nemNullaSor][i] = temp[i];
}


/*Kiszamolja a parameterkent kapott matrix determinansanak erteket
 *@a fuggveny soran kepernyore kerul az eredmeny
*/
void Determinans(MX matrix){

    if(feltoltottMX(matrix)){
        if(matrix.sor == matrix.oszlop){                                //Ha negyzetes
            bool foatlobanNincsNulla = true;
            int sorI = 0;
            double det = 1;
            while(foatlobanNincsNulla){                                 //Ameddig nem talaltam nullat, vagy nem irtam ki a det erteket
                if(matrix.matrix[sorI][sorI]!=0){                       //Ha egy foatlo nem nulla...
                    det *= matrix.matrix[sorI][sorI];                   //det = det * foatlo
                    sorOsztas(&matrix, sorI);                           //sort elosztjuk ugy, hogy a foatloban 1-es legyen
                    if(sorI<matrix.sor-1){                              //vizsgalat mindig az utolso elotti sorig
                        sorKivonas(&matrix, sorI);                      //az adott sorI.-edik sor ALATT LEVO sorokbol kivonom a [-1*(foatlo_alatt_levo_adott_sorbeli_szam/foatlobeli)]-szereset
                        sorI = sorI + 1;
                    }else{                                              //Ha nincs tobb sor...
                        printf("\nA determináns értéke: %lg\n", det);   //Determinans ertekenek kiirasa
                        foatlobanNincsNulla = false;
                    }
                }else{                                                  //Ha egy foatlo nulla, megnezzuk, hogy nincs-e abban az oszlopban nem nulla, majd sor csere
                    int nemNullaElemSorI=0;                             //És van alatta (kicserelheto) sor
                    if(sorI<matrix.sor-1 && vanNemNullaElem(matrix, sorI, &nemNullaElemSorI)){
                        sorCsere(&matrix, sorI, nemNullaElemSorI);      //Sor csere
                        det *= -1;                                      //Sor csere miatt detA ertekenek (-1)-szerese
                    }else{                                              //Ha nincs kicserelheto sor
                        printf("\nA determináns értéke nulla\n");       //A foatloban nulla marad, vagyis det erteke nulla.
                        foatlobanNincsNulla = false;
                    }
                }
            }
        }
        else
            printf("\nNem számítható determináns.\n");
        Felszabadit(&matrix);
    }else
        printf("\nNincs feltöltött mátrix.\n");
}
