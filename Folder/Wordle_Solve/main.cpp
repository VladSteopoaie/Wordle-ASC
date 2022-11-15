#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

//Constante
const int NR_CUV = 11454; //Numarul de cuvinte din fisier 
const int LG_CUV = 5; //Lungimea unui cuvant
const int NR_LITERE =  NR_CUV * LG_CUV; //Numarul de litere din fisier


///FUNCTII///


//Initializeaza fisierele text
void InitFiles()
{
    std::ifstream f_original("../cuvinte_wordle.txt");
    //Deschide fisierele pentru a le sterege continutul
    std::ofstream f1("cuvinte1.txt");
    std::ofstream f2("cuvinte2.txt");

    char s[6];
    //Initializam f1 cu cuvintele din original
    while(f_original >> s)
    {
        f1 << s << '\n';
    }

    f_original.close();
    f1.close();
    f2.close();
}


//Functie de cautare a unei litere in cuvant
bool find(char litera, char *cuvant)
{
    bool ok = false;
    for(int i = 0; i < LG_CUV; i++)
        if(cuvant[i] == litera){
            ok = true;
            break;
        }
    return ok;
}


//Functie care verifica daca cuv_curent si cuv_baza sunt echivalente dupa regula reg
bool VerificareReg(int *reg, char *cuv_baza, char *cuv_curent)
{
    bool ok = true; //presupunem ca sunt echivalente

    for(int i = 0; i < LG_CUV && ok; i++)
    {
        switch(reg[i])
        {
            case 0: //gri (nu se afla in cuvant)
                if(find(cuv_baza[i], cuv_curent))
                    ok = false;
                break;
            case 1: //galben (se afla undeva in cuvant dar nu pe pozitia respectiva)
                if(!find(cuv_baza[i], cuv_curent) || cuv_baza[i] == cuv_curent[i]) //Ba 100% la 100% corect, n-are cum da-o naibii
                    ok = false;
                break;
            case 2: //verde (se afla pe pozitia respectiva)
                if(cuv_baza[i] != cuv_curent[i])
                    ok = false;
                break;
        }
    }

    return ok;
}

//Backtraking
void Back(int k, int *x, char *cuv, double &entropie)
{
    //Conventie: 
    //0 - gri (nu se afla in cuvant)
    //1 - galben (se afla undeva in cuvant dar nu pe pozitia respectiva)
    //2 - verde (se afla pe pozitia respectiva)
    if (k > 4)
    {
        /*
            -> 1. DESCHIDEM FISIER X
            -> 2. CAUT CUVINTELE CARE RESPECTA REGULA DIN X SI SA LE NUMAR X
            -> 3. CALA = (NR CUV NUMARATE / NR CUV TOTAL)
            -> 4. CALCULAM SI INFORMATIA = LOG2(1 / PROBABILITATE)
            -> 5. SUMA DE PROBABILITATI (ENTROPIA) = SUM(PROBABILITATE * INFORMATIE)
        */
        //1
        std::ifstream f1("cuvinte1.txt");
        char s[6];
        int contor = 0;
        //2
        while(f1 >> s)
        {
            if(VerificareReg(x, cuv, s))
                contor ++;
        }
        //3
        double probabilitate = (double)contor / NR_CUV;
        //4
        if(probabilitate > 0){
            float informatie = -log2(probabilitate);
            //5
            entropie += probabilitate * informatie;
        }
        f1.close();
        

    }
    else
    {
        //backtraking normal de produs cartezian
        x[k] = -1;
        while (x[k] < 2)
        {
            x[k]++;
            Back(k + 1, x, cuv, entropie);
        }
    }
}


///MAIN///

int main()
{

    // Initializam fisierele inainte sa inceapa programul
    InitFiles();

    // Deschidem fisierul si vrem sa calculam entropia pentru toate cuvintele din fisier
    std::fstream f1("cuvinte1.txt", std::ios::in | std::ios::out);
    std::fstream f2("cuvinte2.txt", std::ios::in | std::ios::out);
    char cuv[6];
    int i = 1;
    // while(f1 >> cuv)
    // {
    //     int x[5] = {0, 0, 0, 0, 0};
    //     double entropie = 0.0f;
    //     Back(0, x, cuv, entropie);
    //     f2 << cuv << " " << entropie << '\n';
    //     printf("Gata: %d\n", i++);
    // }
    
    char cuv_MAX[6]="TAREI";
    int x[5]={0,1,0,0,1};
    while(f1>>cuv)
    {
        if(VerificareReg(x,cuv_MAX,cuv))
        {
            f2<<cuv<<"\n";
        }
    }
    f1.close();
    f2.close();
    f2.open("cuvinte2.txt");
    f1.open("cuvinte1.txt",std::ios::out |std::ios::trunc);
    while(f2>>cuv)
    {
        f1<<cuv<<"\n";
    }
    f1.close();
    f2.close();
    f2.open("cuvinte2.txt", std::ios::out | std::ios::trunc);
    return 0;
}



