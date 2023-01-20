#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

#define MAX 500

int A[250000], IA[250000], JA[250000];
int A1[250000], IA1[250000], JA1[250000];
int A2[250000], IA2[250000], JA2[250000];



void create(int m, int n, int mat[m][n], int proc_min, int proc_max)
{
    /* KREIRANJE MATRICE GDJE UZIMAMO MINIMALNU VRIJEDNOST NULA I MAKSIMALNU VRIJEDNOST
        U PROCENTIMA.PRVO UBACUJEMO NULE U MATRICI KAKO BI SE ZADOVOLJIO MINIMALNI PROCENAT.
        POSLIJE PRELASKA GRANICE MINIMALNE VRIJEDNOSTI UBACUJEMO NASUMICNO NULU ILI JEDINICU U MATRICU
        DO KRAJA MATRICE, NULE UBACUJEMO DO KRAJA PROC_MAX
        POSLIJE KREIRANJA MATRICE VRSIMO MJESANJE ELEMENATA U MATRICI KAKO BI MATRICA BILA PRIRODNIJA.
    */
    int i,j,k,l,y,indikator;
    int ostatak = (proc_max-proc_min);

    y = rand() % ostatak + -1;
    indikator = rand() % 10 + 0;
    if (indikator%2==0)
    {
        y = 0;
    }

    int brojac=0;
    int novi_brojac=0;
    for (i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            int t;
            if (brojac<=proc_min + y)
            {
                while(1)
                {
                    t = rand() % 2 + 0;
                    if (t==0)
                    {
                        brojac++;
                        mat[i][j] = t;
                        break;
                    }
                }
            }
            if (brojac==proc_min + y)
            {
                novi_brojac = brojac;
            }
            int indikator=0;
            if (novi_brojac>=proc_min + y)
            {
                if (novi_brojac<proc_max)
                {
                    indikator = rand() % 2 + 0;
                    novi_brojac++;
                }
            }
            if (novi_brojac>(proc_min + y) && indikator==1)
            {
                mat[i][j] = 0;
            }
            if (novi_brojac>(proc_min + y) && indikator==0)
            {
                mat[i][j] = 1;
            }





        }
    }
    //MJESANJE ELEMENATA MATRICE

    for (i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            int k = rand() % (m * n);
            int l = rand() % (m * n);
            int temp = mat[i][j];
            mat[i][j] = mat[k/n][k%n];
            mat[k/n][k%n] = temp;
        }
    }





}

void transformToCSR(int m, int n,int l, int mat[m][n], int A[], int IA[], int JA[])
{
    /* U OVOJ FUNKCIJI VRSIMO CSR ZAPIS MATRICE.
        KORISTIMO A,IA,JA NIZOVE. U A NIZ UBACUJEMO ELEMENTE IZ MATRICE KOJI NISU NULE
        U IA NIZ UBACUJEMO BROJ INDEKSA ELEMENTA KOJI JE RAZLICIT OD NULE KOJI SE KRECE PO "i"
        U JA NIZ UBACUJEMO BROJ INDEKSA ELEMENTA KOJI JE RAZLICIT OD NULE KOJI SE KRECE PO "j"
    */
    int i,j;
    int k=0;
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            if (mat[i][j]!=0)
            {
                A[k] = mat[i][j];
                k++;
            }
        }
    }



    k=0;
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            if (mat[i][j] != 0)
            {
                IA[k] = i;
                k++;
            }
        }
    }

    k=0;
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            if (mat[i][j] != 0)
            {
                JA[k] = j;
                k++;
            }
        }
    }


}

void sum(int m, int n, int A[m][n], int B[m][n], int C[m][n])
{
    //VRSIMO SABIRANJE NASUMICNO GENERISANE MATRICE A I MATRICE B
    int i,j;

    LARGE_INTEGER tu_freq, tu_start, tu_end; //POTREBNE FUNKCIJE ZA MJERENJE VREMENA
    __int64 t_ns;
    QueryPerformanceFrequency(&tu_freq);
    QueryPerformanceCounter(&tu_start);

    for(i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    QueryPerformanceCounter(&tu_end);
    t_ns = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;

    printf("VRIJEME TRAJANJA OPERACIJE SABIRANJA MATRICA:(%llu)[ms]  (%llu)[ns]\n",t_ns/1000000,t_ns);




}

void product(int m, int n, int p, int A[m][n], int B[n][p], int C[m][p])
{
    int i,j,k;
    /*U OVOJ FUNKCIJI VRSIMO MNOZENJE MATRICA A I B. PRI TOME OVA FUNKCIJA CE SE SAMO POZVATI KADA
        JE MOGUCE IZVRSENJE OVE OPERACIJE.
    */
    LARGE_INTEGER tu_freq, tu_start, tu_end; //POTREBNE FUNKCIJE ZA MJERENJE VREMENA
    __int64 t_ns;
    QueryPerformanceFrequency(&tu_freq);
    QueryPerformanceCounter(&tu_start);

    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
        {
            C[i][j] = 0;
            for (k=0; k<p; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    QueryPerformanceCounter(&tu_end);
    t_ns = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;

    printf("VRIJEME TRAJANJA OPERACIJE MNOZENJA MATRICA:(%llu)[ms]  (%llu)[ns]\n", t_ns/1000000,t_ns);
}

void summ(int u,int l, int A1[], int IA1[], int JA1[],int o, int A2[], int IA2[], int JA2[],int A[], int IA[], int JA[])
{
    /* FUNKCIJA ZA SABIRANJE CSR ZAPISA MATRICE. SVAKI NIZ SE SABIRA SA SVOJIM ODGOVARAJUCIM
    I VRIJEDNOST SE UPISUJE U KONACNO A,IA,JA STO PREDSTAVLJA SUMU 2 CSR ZAPISANE MATRICE.
    */
    int i, j, k;
    int nnz1 = l, nnz2 = o; //nnz - NON-ZERO ELEMENTI

    i = j = k = 0;


    LARGE_INTEGER tu_freq, tu_start, tu_end; //POTREBNE FUNKCIJE ZA MJERENJE VREMENA
    __int64 t_ns;
    QueryPerformanceFrequency(&tu_freq);
    QueryPerformanceCounter(&tu_start);

    while(i < nnz1 && j < nnz2)
    {
        int vrsta1 = IA1[i];
        int vrsta2 = IA2[j];
        if(vrsta1 == vrsta2)
        {
            int kolona1 = JA1[i];
            int kolona2 = JA2[j];
            if(kolona1 == kolona2)
            {
                A[k] = A1[i] + A2[j];
                IA[k] = vrsta1;
                JA[k] = kolona1;
                k++;
                i++;
                j++;
            }
            else if(kolona1 < kolona2)
            {
                A[k] = A1[i];
                IA[k] = vrsta1;
                JA[k] = kolona1;
                k++;
                i++;
            }
            else
            {
                A[k] = A2[j];
                IA[k] = vrsta2;
                JA[k] = kolona2;
                k++;
                j++;
            }
        }
        else if(vrsta1 < vrsta2)
        {
            A[k] = A1[i];
            IA[k] = vrsta1;
            JA[k] = JA1[i];
            k++;
            i++;
        }
        else
        {
            A[k] = A2[j];
            IA[k] = vrsta2;
            JA[k] = JA2[j];
            k++;
            j++;
        }
    }


    while(i < nnz1)
    {
        A[k] = A1[i];
        IA[k] = IA1[i];
        JA[k] = JA1[i];
        i++;
        k++;

    }

    while(j < nnz2)
    {
        A[k] = A2[j];
        IA[k] = IA2[j];
        JA[k] = JA2[j];
        j++;
        k++;

    }
    QueryPerformanceCounter(&tu_end);
    t_ns = 1000000000ULL * (tu_end.QuadPart - tu_start.QuadPart) / tu_freq.QuadPart;

    printf("VRIJEME TRAJANJA OPERACIJE SABIRANJA CSR MATRICA:(%llu)[ms]  (%llu)[ns]\n", t_ns/1000000,t_ns);

}

int nule(int m,int n, int mat[m][n])
{
    /* FUNKCIJA KOJA BROJI NULE IZ MATRICE.
       OVA FUNKCIJA OMOGUCAVA DA NA KRAJU PROGRAMA ISPISE KOLIKO NULA SE NALAZI
       U MATRICI I TIME DA SE IZRACUNA BROJ NULA U MATRICI.
    */
    int brojac=0,i,j;
    for (i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            if (mat[i][j] == 0)
            {
                brojac++;
            }


        }
    }

    return brojac;
}

int jedinice(int m,int n, int mat[m][n])
{
    // FUNKCIJA KOJA BROJI JEDINICE IZ MATRICE.


    int jedinice=0,i,j;
    for (i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            if (mat[i][j] != 0)
            {
                jedinice++;
            }


        }
    }

    return jedinice;
}










int main()
{

    /* SEMINARSKI RAD KOJI JE NA TEMU KREIRANJE NASUMICNIH MATRICA KORISTECI ODREDJENE PARAMETRE.
       POTREBNO JE NAPRAVITI CSR ZAPIS MATRICE U NIZOVIMA, NAPRAVITI TRECU MATRICU KOJA PREDSTAVLJA
       SUMU/PROIZVOD PRVE 2 MATRICE, IZRACUNATI SUMU CSR ZAPISANIH MATRICA, REZULTATE ISPISATI NA PROGRAMU.
       U OBZIR UZETI DA SE VRIJEME RACUNA SAMO OD TRENUTKA KADA SE KRENE VRSITI OPERACIJA DO TRENUTKA KADA
       SE ZAVRSI.


    */



    srand(time(0));

    //DIMENZIJA MATRICA
    int m=500,n=500,p=m;
    int *A_mat = (int *)malloc(MAX * MAX * sizeof(int));
    int *B_mat = (int *)malloc(MAX * MAX * sizeof(int));
    int *C = (int *)malloc(MAX * MAX * sizeof(int));


    printf("MATRICA DIMENZIJA %d x %d\n\n", m,n);

    //PROCENAT NULA KOJI MOZEMO MIJENJATI
    int minimalno=90, maksimalno=98;
    int proc_min = ((m*n) * minimalno) / 100;
    int proc_max = ((m*n) * maksimalno)/ 100;
    printf("MINIMALNO NULA: %d --- %d POSTO\nMAKSIMALNO NULA: %d --- %d POSTO\n\n", proc_min,minimalno,proc_max,maksimalno);


    //KREIRANJE MATRICA
    create(m,n,A_mat,proc_min,proc_max);
    create(m,n,B_mat,proc_min,proc_max);




    //SUMA A+B
    sum(m,n,A_mat,B_mat,C);
    int u = jedinice(m,n,C);



    //PROIZVOD A*B
    if (n!=p)
    {
        printf("MATRICE NIJE MOGUCE POMNOZITI!\n\n");
    }
    else
    {
        product(m,n,p,A_mat,B_mat,C);
    }




    //NIZ A,IA,JA
    int l = jedinice(m,n,A_mat);
    int o = jedinice(m,n,B_mat);

    transformToCSR(m,n,l,A_mat,A1,IA1,JA1);
    transformToCSR(m,n,o,B_mat,A2,IA2,JA2);



    //SUMA NIZOVA
    summ(u,l,A1,IA1,JA1,o,A2,IA2,JA2,A,IA,JA);








    printf("\n");
    printf("Ukupno nula u matrici A: %d\n", nule(m,n,A_mat));
    printf("Ukupno nula u matrici B: %d\n", nule(m,n,B_mat));
    printf("\n");




    free(A_mat);
    free(B_mat);
    free(C);

    return 0;

}
