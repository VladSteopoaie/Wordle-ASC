#include <iostream>
#include <string.h>
#include <vector>
#include <windows.h>
#include <fstream>
using namespace std;

ifstream f("../cuvinte_wordle.txt");

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
            cout << guess_word[i], Sleep(60);
        }
        else if(state[i] == 'Y'){
            SetConsoleTextAttribute(h, 14);     //setez culoarea galbena [...]
            cout << guess_word[i], Sleep(60);
        }
        else{
            SetConsoleTextAttribute(h, 12);     //setez culoarea rosie   [...]
            cout << guess_word[i], Sleep(60);
        }
    }
    SetConsoleTextAttribute(h, 7);              // setez culoarea default
    if(cnt == 5){                              // verific daca toate literele se potrivesc
        verif = true;
    }
}
void screen1(){
    cout << '\n';
    string str1 = "Introdu un cuvant: ";
    for(char c : str1){
        cout << c, Sleep(60);
    }
}
void screen2(){
    cout << '\n';
    string str2 = "Ai ghicit!";
    for(char c : str2){
        cout << c, Sleep(60);
    }
}
void screen3(){
    cout << '\n';
    string str3 = "Mai incearca: ";
    for(char c : str3){
        cout << c, Sleep(60);
    }
}
int main(){
    
    string word = "trist";
    string ex;
    bool solved = false;
    screen1();
    while(solved == false){
        cin >> ex;
        word_verifier(word, ex, solved);
        if(solved) screen2();
        else screen3();
    }
    return 0;
}
