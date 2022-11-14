#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <vector>

using namespace std;

ifstream f("../cuvinte_wordle.txt");

bool in_menu=true,in_game=false,in_solver=false,verif=false,ok=false; //pentru a vedea in ce stadiu este jocul
char text[500];
//pentru textul din meniu si instructiuni va fi folosita functia sleep, pentru a nu afisa un wall of text instant.
//de adaugat lista de cuvinte prin citire de fisier

void word_getter(char cuv[])
{int n;
    for (int i=1;i<=10;i++)
    {
        n=rand()%11451+1; //un for micut inainte ca sa genereze mai multe numere random si sa nu fie neaparat in ordine increasing
    }
    while(n)
    {
        f>>cuv;
        n--;
    }
    cout<<cuv; //pentru a verifica faptul ca a ales un cuvant
}


void menu_text() //introducerea jocului
{
    strcpy(text,"Salut, hai sa jucam jocul Wordle!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],Sleep(30);
    cout<<'\n';
    strcpy(text,"Pentru a incepe jocul, scrie cuvantul 'Joaca'!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],Sleep(30);
    cout<<'\n';
    strcpy(text,"Daca vrei ca jocul sa se rezolve singur, scrie cuvantul 'Rezolva'!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],Sleep(30);
    cout<<'\n';
    strcpy(text,"In cazul in care nu stii cum se joaca acest joc, scrie 'Ajutor' si vor aparea instructiuni pe ecran!");
    for (int i=0;i<strlen(text);i++)
        cout<<text[i],Sleep(30);
    cout<<'\n';

}

void instructions() //cum se joca jocul. de modificat in functie de cum vor fi evidentiate literele corecte in joc
{
    cout<<"In acest joc, scopul este simplu: Incearca sa ghicesti un cuvant de 5 litere!"<<'\n';
    cout<<"Insa, incercarile trebuie sa fie numai cuvinte de 5 litere existente in dictionarul limbii romane!"<<'\n';
    cout<<"De fiecare data cand introduci un cuvant, ti se vor da urmatoarele informatii:"<<'\n';
    cout<<"1) Daca o litera din cuvantul introdus de tine se afla in cuvantul care trebuie ghicit, aceasta va fi evidentiata cu galben"<<'\n';
    //Sleep(1000)
    cout<<"2) Daca litera, in plus, se afla pe aceeasi pozitie ca in cuvantul care trebuie ghicit, aceasta va fi evidentiata cu verde"<<'\n';
    //Sleep(1000)
    cout<<"3) Daca litera nu apartine deloc acelui cuvant, atunci culoarea ei ramane neschimbata"<<'\n';
    cout<<"Nota: jocul nu este case sensitive, asa ca literele mici vor fi tratate ca si cum ar fi litere mari"<<'\n';
    cout<<"Exemplu de inputuri corecte: Sapte, paine, CRUCE, fOaMe"<<'\n';

}

void uppercase(char s[]) //inputurile vor fi transformate in cuvinte cu litere mari, ca sa nu fie case-sensitive jocul
{
    for (int i=0;i<strlen(s);i++)
    {
        if (s[i]>='a' && s[i]<='z')
            s[i]-=32;
    }
}

void menu_checker(char s[]) //verificarile inputurilor din meniu, daca vrem sa jucam, sa rezolvam sau instructiuni
{
    if (strcmp(s,"JOACA")==0)
        in_menu=false,in_game=true;
    else if (strcmp(s,"REZOLVA")==0)
        in_menu=false,in_solver=true;
    else if (strcmp(s,"AJUTOR")==0)
        instructions();
    else //in cazul in care alta comanda este introdusa
        cout<<"Aceasta nu este o comanda valida!"<<'\n';

}


HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);    // pentru culori

void word_verifier(string correct_word, string guess_word, bool &verif){
    char state[6] = {'_', '_', '_', '_', '_'};        // starea fiecarei litere din cuvantul introdus
    for(int i = 0; i < 5; ++i){
        if(correct_word[i] == guess_word[i]){         // verific daca am litere care deja se potrivesc
            state[i] = 'G';
        }
    }
    for(int i = 0; i < 5; ++i){
        bool ok = false;
        if(state[i] == '_'){                   //daca litera nu se potriveste pe pozitie identica, o caut pe restul pozitiilor
            for(int j = 0; j < 5; ++j){
                if(correct_word[j] == guess_word[i]){   // daca la un moment dat gasesc o litera din cuvantul introdus in cel corect, dar nu pe aceeasi pozitie, o colorez in galben
                    state[i] = 'Y';
                    ok = true;
                    break;
                }
            }
            if(ok == false) state[i] = 'R';             // marchez cu rosu daca nu gasesc o litera din cuvantul introdus in celalalt
        }
    }
    int cnt = 0;
    for(int i = 0; i < 5; ++i){
        if(state[i] == 'G'){
            cnt ++;                             // numar cate pozitii din cuvinte au aceeasi litera
            SetConsoleTextAttribute(h, 10);     //setez culoarea verde la afisare
            cout << guess_word[i];
        }
        else if(state[i] == 'Y'){
            SetConsoleTextAttribute(h, 14);     //setez culoarea galbena [...]
            cout << guess_word[i];
        }
        else{
            SetConsoleTextAttribute(h, 12);     //setez culoarea rosie   [...]
            cout << guess_word[i];
        }
    }
    SetConsoleTextAttribute(h, 7);              // setez culoarea default
    if(cnt == 5){                              // verific daca toate literele se potrivesc
        verif = true;
    }
}


int main()
{
    srand(time(NULL));
    menu_text();
    char s[25],cuv[25],guess[25]; //eventual va fi alocat dinamic. acest s e folosit PENTRU MENU INPUTS.
    while(in_menu)
    {
        cin.getline(s,25);
        uppercase(s);
        menu_checker(s);

    }
    while(in_game) //aici va incepe jocul jucat de o persoana
    {
        if (ok==false)
            word_getter(cuv),cout<<'\n',ok=true;
        strcpy(text,"Scrie un cuvant: ");
        for (int i=0;i<strlen(text);i++)
            cout<<text[i],Sleep(30);
        cout<<'\n';
        cin>>guess;
        uppercase(guess);
        word_verifier(cuv,guess,verif);
        cout<<'\n';
     //de facut functiile pentru verificarea validitatii cuvantului si verificarea literelor
        if (verif==true)
        {
            strcpy(text,"Felicitari, ai gasi cuvantul!");
            for (int i=0;i<strlen(text);i++)
                cout<<text[i],Sleep(30);
            in_game=false;
        }

    }
    while(in_solver) //aici sa se desfasoare activitatea solverului(de modificat in urma sfatuirilor! :) )"
    {
        cout<<"De implementat"<<'\n';
    }
}
