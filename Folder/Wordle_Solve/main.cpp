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
    //Deschide fisierele pentru a le sterege
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


///MAIN///

int main()
{
    InitFiles();
    return 0;
}