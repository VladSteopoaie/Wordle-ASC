#include <iostream>
#include <windows.h>
#include <cstring>
using namespace std;

bool in_menu=true,in_game=false,in_solver=false; //pentru a vedea in ce stadiu este jocul

//pentru textul din meniu si instructiuni va fi folosita functia delay, pentru a nu afisa un wall of text instant.
//de adaugat lista de cuvinte prin citire de fisier

void menu_text() //introducerea jocului
{char text[500];
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
    cout<<"Exemplu de inputuri corecte: Sapte paine CRUCE fOaMe"<<'\n';

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


int main()
{

    menu_text();
    char s[25]; //eventual va fi alocat dinamic. acest s e folosit PENTRU MENU INPUTS.
    while(in_menu)
    {
        cin.getline(s,25);
        uppercase(s);
        //cout<<s<<'\n';
        menu_checker(s);

    }
    while(in_game) //aici va incepe jocul jucat de o persoana
    {
        //de facut functiile pentru verificarea validitatii cuvantului si verificarea literelor
        in_game=false;
    }
    while(in_solver) //aici sa se desfasoare activitatea solverului(de modificat in urma sfatuirilor! :) )"
    {
        cout<<"De implementat"<<'\n';
    }
}
