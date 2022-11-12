#include <iostream>
#include <fstream>

//Constante
const int NR_CUV = 11454; //Numarul de cuvinte din fisier 
const int NR_LITERE =  NR_CUV * 5; //Numarul de litere din fisier


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

//Backtraking
float Back(int k, int *x, char *cuv)
{
    //Conventie: 
    //0 - gri (nu se afla in cuvant)
    //1 - galben (se afla undeva in cuvant dar nu pe pozitia respectiva)
    //2 - verde (se afla pe pozitia respectiva)
    int i;
    float entropie = 0.0f;
    if (k == 5)
    {
        /*
            -> FISIER 
            -> CAUT CUVINTELE CARE RESPECTA REGULA DIN X SI SA LE NUMAR
            -> CALCULAM PROBABILITATEA = (NR CUV NUMARATE / NR CUV TOTAL)
            -> CALCULAM SI INFORMATIA = LOG2(1 / PROBABILITATE)
            ->SUMA DE PROBABILITATI (ENTROPIA) = SUM(PROBABILITATE * INFORMATIE)
        */


    }
    else
    {
        x[k] = -1;
        while (x[k] < 2)
        {
            x[k]++;
            Back(k + 1, x, cuv);
        }
    }
}


///MAIN///

int main()
{
    InitFiles();

    std::ifstream f1("cuvinte1.txt");
    char cuv[6];
    while(f1 >> cuv)
    {
        int x[5] = {0, 0, 0, 0, 0};
        float entropie = Back(0, x, cuv);
    }


    return 0;
}



