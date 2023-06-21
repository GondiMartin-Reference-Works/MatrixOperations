#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "inout.h"
#include "mmuvelet.h"

/*Lefoglalt memoriateruletet szabadit fel
 *@parametere egy strukturalt matrix címe, melynek egyik mezotagjat (matrix), egy ketdimenzios tombot szabadit fel
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
            printf("\nHIBA: Nem számot adtál meg!\n");
            scanf("%*[^\n]");
            printf("%s",utast2_szoveg);
        }else if (van_tart && (szam > max || szam < min))
            printf("\nHIBA: A megadott szám nem volt megfelelo! (min: %d, max: %d)\n", min, max);
        else
            helyes = true;

    }while(!helyes);
    return szam;
}


/*Kilistazza a menupontokat a bemeneti erteknek megfeleloen
 *@visszateresi erteke a helyes menupont, amit a felhasznalo megadott
*/
int Menu(){
    char menu_szoveg[] = {"\nVálassz az alábbiak közül kérlek!\n"
       "1.  ~ 'A' mátrix beolvasása, kiíratása fájlba\n"
       "2.  ~ 'B' mátrix beolvasása, kiíratása fájlba\n"
       "3.  ~ Mátrixok összeadása\n"
       "4.  ~ Mátrixok kivonása\n"
       "5.  ~ 'A' mátrix szorzása egy valós számmal\n"
       "6.  ~ 'B' mátrix szorzása egy valós számmal\n"
       "7.  ~ Mátrixok összeszorzása (A*B)\n"
       "8.  ~ Mátrixok összeszorzása (B*A)\n"
       "9.  ~ 'A' mátrix determinánsának meghatározása\n"
       "10. ~ 'B' mátrix determinánsának meghatározása\n"
       "11. ~ Kilépés\n"
       "\nMenüpont: "};
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
            printf("Mátrix %d.sor %d.oszlop eleme: ",x+1,y+1);
            uj[x][y] = SzamBeolvas(0,0,false,"","Add meg helyesen az elem értékét!: ");
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
        perror("\nNem sikerült megnyitni.\n");
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
        printf("Helytelen bemenet! Nem lehet üres mátrixot létrehozni! (sor: %d, oszlop: %d)\n", x, y);
        return true;
    }
    return false;
}


/*Beolvas es kiir egy matrixot
 *@parameterei egy struct tipusu matrixra mutato pointer es a .txt nevere mutato pointer
*/
void BeolvasKiir(MX *matrix, char *fajlNev){
    int sor, oszlop;
    char bekersor_szoveg[] = {"\nAdd meg a mátrix sorainak számát!: "};
    char bekeroszlop_szoveg[] = {"Add meg a mátrix oszlopainak számát!: "};

    //matrix feltoltese
    do{
        sor = (int)SzamBeolvas(0,0,false,bekersor_szoveg,"");
        oszlop = (int)SzamBeolvas(0,0,false,bekeroszlop_szoveg,"");
    }while(nullVektor(sor, oszlop));
    matrix->matrix = feltolt(sor, oszlop);
    matrix->sor = sor;
    matrix->oszlop = oszlop;
    //matrix kiiratasa
    printf("%s\n", kiir(matrix, fajlNev) ? "\nA fájl létrejött." : "A fájl létrehozása sikertelen.");
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
            //Többoszlopos
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
