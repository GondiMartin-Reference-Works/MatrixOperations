#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "inout.h"
#include "mmuvelet.h"

/*Lefoglalt memoriateruletet szabadit fel
 *@parametere egy strukturalt matrix c�me, melynek egyik mezotagjat (matrix), egy ketdimenzios tombot szabadit fel
*/
void Felszabadit(MX *matrix){
    for(int i = 0; i<matrix->sor; ++i)
        free(matrix->matrix[i]);
    free(matrix->matrix);
}


/*Beolvas egy szamot hibakat kezelve
 *@parameter listaja a kovetkezo: sorrendben!
    - az elso ket parameter megmondja, hogy a szamnak milyen tartomany kozott kell lennie.
    - a harmadik parameter egy logika ertek, mely IGAZ, ha a bekerendo szamnak megszabtak a tartomanyat
    - negyedik parameter egy karaktertomb, ami az utasitasokat tartalmazza a felhasznalo fele
    - otodik parameter szinten egy karaktertomb, ami tovabbi utasitast tartalmaz a felhasznalo fele, ha
    az elsot se vette figyelembe
 *@visszateresi erteke a helyes, eltarolando valos szam
*/
double SzamBeolvas(int min, int max, bool van_tart, char *utast1_szoveg,char *utast2_szoveg){
    bool helyes = false;
    double szam;
    do{
        printf("%s",utast1_szoveg);
        if(scanf("%lf",&szam)!=1){
            printf("\nHIBA: Nem sz�mot adt�l meg!\n");
            scanf("%*[^\n]");
            printf("%s",utast2_szoveg);
        }else if (van_tart && (szam > max || szam < min))
            printf("\nHIBA: A megadott sz�m nem volt megfelelo! (min: %d, max: %d)\n", min, max);
        else
            helyes = true;

    }while(!helyes);
    return szam;
}


/*Kilistazza a menupontokat a bemeneti erteknek megfeleloen
 *@visszateresi erteke a helyes menupont, amit a felhasznalo megadott
*/
int Menu(){
    char menu_szoveg[] = {"\nV�lassz az al�bbiak k�z�l k�rlek!\n"
       "1.  ~ 'A' m�trix beolvas�sa, ki�rat�sa f�jlba\n"
       "2.  ~ 'B' m�trix beolvas�sa, ki�rat�sa f�jlba\n"
       "3.  ~ M�trixok �sszead�sa\n"
       "4.  ~ M�trixok kivon�sa\n"
       "5.  ~ 'A' m�trix szorz�sa egy val�s sz�mmal\n"
       "6.  ~ 'B' m�trix szorz�sa egy val�s sz�mmal\n"
       "7.  ~ M�trixok �sszeszorz�sa (A*B)\n"
       "8.  ~ M�trixok �sszeszorz�sa (B*A)\n"
       "9.  ~ 'A' m�trix determin�ns�nak meghat�roz�sa\n"
       "10. ~ 'B' m�trix determin�ns�nak meghat�roz�sa\n"
       "11. ~ Kil�p�s\n"
       "\nMen�pont: "};
    return (int)SzamBeolvas(1,11,true,menu_szoveg,"");
}


/*Feltolt egy matrixot elemekkel
 *@parameterei ketto egesz szam, mely a matrix mereteit adjak
 *@visszateresi erteke egy ketdimenzios tomb => matrix
*/
static double **feltolt(int szeles, int magas){
    double **uj;
    uj = (double**) malloc(szeles*sizeof(double*));
    for(int x = 0; x<szeles; ++x){
        uj[x] = (double*)malloc(magas*sizeof(double));
        for(int y = 0; y<magas; ++y){
            printf("M�trix %d.sor %d.oszlop eleme: ",x+1,y+1);
            uj[x][y] = SzamBeolvas(0,0,false,"","Add meg helyesen az elem �rt�k�t!: ");
        }
    }
    return uj;
}


/*Fajlba ir egy matrixot
 *@parameterei egy struct tipusu matrixra mutato pointer es a keletkezo fajl neve karaktertombkent
 *@visszateresi erteke logikai IGAZ, ha sikerult a fajlt letrehozni
*/
static bool kiir(MX *paramMX, char *fajlNev){
    bool sikeres = false;
    FILE* fajl = fopen(fajlNev,"wt");
    if(fajl!=NULL){
        fprintf(fajl,"%d,%d\n",paramMX->sor,paramMX->oszlop);
        for(int i = 0; i<paramMX->sor; ++i){
            for(int j = 0; j<paramMX->oszlop; ++j){
                fprintf(fajl,"%g",paramMX->matrix[i][j]);
                if(j!=paramMX->oszlop-1) fprintf(fajl, "\t");
            }
            fprintf(fajl,"\n");
        }
        sikeres = true;
    }
    else{
        perror("\nNem siker�lt megnyitni.\n");
        sikeres = false;
    }
    fclose(fajl);
    return sikeres;
}


/*Ellenorzi ket szamrol, hogy legalabb az egyikuk nulla-e
 *@parameterei ketto egesz szam, oket vizsgaljuk
 *@visszateresi erteke logika IGAZ, ha legalabb az egyik szam nulla volt
*/
static bool nullVektor(int x, int y){
    if(x==0 || y==0){
        printf("Helytelen bemenet! Nem lehet �res m�trixot l�trehozni! (sor: %d, oszlop: %d)\n", x, y);
        return true;
    }
    return false;
}


/*Beolvas es kiir egy matrixot
 *@parameterei egy struct tipusu matrixra mutato pointer es a .txt nevere mutato pointer
*/
void BeolvasKiir(MX *matrix, char *fajlNev){
    int sor, oszlop;
    char bekersor_szoveg[] = {"\nAdd meg a m�trix sorainak sz�m�t!: "};
    char bekeroszlop_szoveg[] = {"Add meg a m�trix oszlopainak sz�m�t!: "};

    //matrix feltoltese
    do{
        sor = (int)SzamBeolvas(0,0,false,bekersor_szoveg,"");
        oszlop = (int)SzamBeolvas(0,0,false,bekeroszlop_szoveg,"");
    }while(nullVektor(sor, oszlop));
    matrix->matrix = feltolt(sor, oszlop);
    matrix->sor = sor;
    matrix->oszlop = oszlop;
    //matrix kiiratasa
    printf("%s\n", kiir(matrix, fajlNev) ? "\nA f�jl l�trej�tt." : "A f�jl l�trehoz�sa sikertelen.");
    //matrix felszabaditasa
    Felszabadit(matrix);
}


/*Beolvas egy fajlt
 *@kezeli a parameterkent kapott fajlt
 *@parameterkent, pointert hasznalva visszaadja a matrix sor es oszlop szamat
 *@visszateresi erteke a fajlbol beolvasott adatokkal feltoltott ketdimenzios tomb => matrix
*/
static double **olvas(char *fajlNev, int *sor, int *oszlop){
    FILE* fajl = fopen(fajlNev, "rt");
    double** uj;
    if(fajl!=NULL){
        fscanf(fajl,"%d,%d",sor,oszlop);
        uj = (double**) malloc(*sor*sizeof(double*));
        for(int x = 0; x<*sor; ++x){
            uj[x] = (double*) malloc(*oszlop*sizeof(double));
            //ESETSZETVALASZTAS
            //1oszlopos
            if(*oszlop == 1)  fscanf(fajl,"%lg",uj[x]);
            //T�bboszlopos
            else
                for(int y = 0; y<*oszlop; ++y)
                    (y!=*oszlop-1) ? fscanf(fajl, "%lg[^\t]",&uj[x][y]) : fscanf(fajl, " %lg[^\n]",&uj[x][y]);
        }
    }else{
        return NULL;
    }
	fclose(fajl);
    return uj;
}


/*Fajlbol visszaolvas egy matrixot
 *@parameterei egy struct tipusu matrixra mutato pointer es a beolvasando .txt nevere mutato pointer
*/
void BeolvasFajl(MX *matrix, char *fajlNev){
    matrix->matrix = olvas(fajlNev, &matrix->sor, &matrix->oszlop);
}
