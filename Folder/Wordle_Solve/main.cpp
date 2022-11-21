#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

// VARIABILE GLOBALE
int nr_cuv = 11454; //Numarul de cuvinte din fisier 

// CONSTANTE
#define FIFO "guess"
#define F1 "cuvinte1.txt"
#define F2 "cuvinte2.txt"
const int LG_CUV = 5; //Lungimea unui cuvant
const int NR_LITERE =  nr_cuv * LG_CUV; //Numarul de litere din fisier


///FUNCTII///


//Initializeaza fisierele text
int InitFiles()
{
    //Creeaza fisierul pentru IPC
    if(mkfifo(FIFO, 0777) != 0) return -1;

    std::ifstream f_original("../cuvinte_wordle.txt");
    //Deschide fisierele pentru a le sterege continutul
    std::ofstream f1(F1);
    std::ofstream f2(F2);

    char s[6];
    //Initializam f1 cu cuvintele din original
    while(f_original >> s)
    {
        f1 << s << '\n';
    }

    f_original.close();
    f1.close();
    f2.close();
    return 0;
}


//Functie de cautare a unei litere in cuvant
bool find(char litera, std::string cuvant)
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
bool VerificareReg(int *reg, std::string cuv_baza, std::string cuv_curent)
{
    bool ok = true; //presupunem ca sunt echivalente

    for(int i = 0; i < LG_CUV && ok; i++)
    {
        switch(reg[i])
        {
            case 0: //rosu (nu se afla in cuvant)
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

//Backtraking (produs cartezian)
void Back(int k, int *x, std::string cuv, double &entropie)
{
    //Conventie: 
    //0 - rosu (R) (nu se afla in cuvant)
    //1 - galben (Y) (se afla undeva in cuvant dar nu pe pozitia respectiva)
    //2 - verde (G) (se afla pe pozitia respectiva)
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
        std::ifstream f1(F1);
        char s[6];
        int contor = 0;
        //2
        while(f1 >> s)
        {
            if(VerificareReg(x, cuv, s))
                contor ++;
        }
        //3
        double probabilitate = (double)contor / nr_cuv;
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

//Functie care citeste din fifo
int Read(int *v)
{
    //deschidem file descriptor pentru citire
    int fdr = open(FIFO, O_RDONLY);

    if(fdr == -1)
    {
        printf("Eroare la deschidere fifo => Read");
        return -1;
    }
    char cuv[LG_CUV];
    //citim din fifo
    if(read(fdr, cuv, sizeof(char) * LG_CUV) == -1)
    {
        //luam in calcul eroarea
        printf("Eroare la citirea din fifo!");
        return -1;
    }
    //facem conversie de la output la conventia din backtraking
    for(int i = 0; i < LG_CUV; i ++)
    {
        switch(cuv[i])
        {
            case 'R':
                v[i] = 0;
                break;
            case 'Y':
                v[i] = 1;
                break;
            case 'G':
                v[i] = 2;
                break;
        }
    }

    close(fdr);
    return 0;
}

//Functie care scrie in fifo
int Write(std::string str)
{

    //deschidem fifo pentru afisare
    int fdw = open(FIFO, O_WRONLY);

    //verificam erori
    if(fdw == -1)
    {
        printf("Eroare la deschidere fifo => Write");
        return -1;
    }
    //scriem in fifo
    char ch[5];
    for(int i = 0; i < LG_CUV; i++)
        ch[i] = str[i];
    if(write(fdw, ch, sizeof(char) * LG_CUV) == -1)
    {
        printf("Eroare la afisare in fifo!");
        return -1;
    }

    close(fdw);
    return 0;
}

///MAIN///

int main()
{

    
    
    //Intre cele doua lini va fi unit programul final
    /////////////////////////////////////////////////////////////

    // Initializam fisierele inainte sa inceapa programul
    if(InitFiles() != 0)
    {
        std::cout << "A aparut o eroare la fisiere!";
        return -1;
    }
    

    // Poate niste citiri inainte... daca nu dam direct primul guess "TAREI"
    bool ghicit = false;
    std::string guess = "TAREI";
    if(Write(guess) != 0)
    {
        return -1;
    }
    
    // Cat timp nu am ghicit cuvantul
    while(ghicit == false)
    {
        int rezultat[LG_CUV];
        // Citim ce rezultat obtinem 
        if(Read(rezultat) != 0)
        {
            return -1;
        }

        // Presupunem ca am gasit cuvantul
        ghicit = true;
        for(int i = 0; i < LG_CUV; i ++)
            if(rezultat[i] != 2)
                ghicit = false; //verificam daca presupunerea e adevarata
        if(ghicit == true) // daca am ghicit iesim din while
            break;

        // Verificam si pastram doar cuvintele care respecta regula
        
        std::ifstream f1_in(F1);
        std::ofstream f2_out(F2);

        std::string cuv; //variabila pentru citirea din fisiere
        nr_cuv = 0;



        while(f1_in >> cuv) //citeste cuvintele din primul fisier
        {
            // verifica care cuvinte respecta regula din rezultat si le pune in al doilea fisier
            if(VerificareReg(rezultat, guess, cuv)) 
            {
                f2_out << cuv << "\n";
                nr_cuv ++;
            }
            
        }

        if (nr_cuv == 1) //Daca ne-a mai ramas un cuvant in lista atunci el este guess-ul nostru
            guess = cuv;

        f1_in.close();
        f2_out.close();
        
        //Deschidem fisiere pentru a copia continutul din F2 in F1
        std::ifstream f2_in(F2);
        std::ofstream f1_out(F1);

        while(f2_in >> cuv)
        {
            f1_out << cuv << "\n";
        }

        f2_in.close();
        f1_out.close();
        //Sterge ce e in F2
        f2_out.open(F2);
        f2_out.close();
        
        // Actualizam entropia fiacarui cuvant

        f1_in.open(F1);

        double entropie_max = 0.0f;
        while(f1_in >> cuv && nr_cuv != 1) //cuv l-am declarat mai sus
        {
            int x[5] = {0, 0, 0, 0, 0};
            double entropie = 0.0f;
            Back(0, x, cuv, entropie);
            if(entropie > entropie_max){
                entropie_max = entropie;
                guess = cuv;
            }
        }
        f1_in.close();
        // Dam ca si urmator guess cuvantul cu entropia cea mai mare
        if(Write(guess) != 0) return -1;
    }

    // Afisam cuvantul ghicit
    std::cout << guess << std::endl;
    //OBSERVATII
        // trebuie sa tinem minte numarul de guess-uri
        // trebuie sa tinem minte toate guess-urile pana la cuvantul ghicit

    remove("guess");
    /////////////////////////////////////////////////////////////

  
    return 0;
}



