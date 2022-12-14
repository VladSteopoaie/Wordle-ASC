#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <string.h>

using namespace std;

#define FIFO "guess"

ifstream f("../cuvinte_wordle.txt");

bool in_menu = true, in_game = false, in_solver = false, verif = false, ok = false, ok_guess = true, ok_num = false; /// pentru a vedea in ce stadiu este jocul
char state[6];
int contor;
vector<string> dictionar;
string line;

/// pentru textul din meniu si instructiuni va fi folosita functia usleep, pentru a nu afisa un wall of text instant.

// Intializare dictionarului de cuvinte
void init_dictionar()
{
    int total_lines = 0;
    while (getline(f, line))
    {
        dictionar.push_back(line);
        total_lines++;
    }
}

// Functie care citeste din fifo
int Read(char *c)
{
    // deschidem file descriptor pentru citire
    int fdr = open("guess", O_RDONLY);

    if (fdr == -1)
    {
        printf("Eroare la deschiderea fdr! => Read");
        return -1;
    }

    // citim din fifo cuvantul cu cea mai mare entropie
    if (read(fdr, c, sizeof(char) * 5) == -1)
    {
        // in caz de eroare la deschiderea fisierului fifo se printeaza acest mesaj
        printf("Eroare la citirea din fifo");
        return -1;
    }
    close(fdr);

    // Afisam numarul citit din fifo
    for (int i = 0; i < 5; ++i)
    {
        cout << c[i];
    }
    return 0;
}

// Functia care scrie in fifo
int Write(char *v)
{
    // deschidem file descriptor pentru scriere
    int fdw = open("guess", O_WRONLY);

    // verificam daca programul intampina erori
    if (fdw == -1)
    {
        perror("Eroare la deschiderea fdw! =>Write"); // intreb pe vlad
        return -1;
    }

    // scriem in fifo informatia cuvantului citit
    if (write(fdw, v, sizeof(char) * 5) == -1)
    {
        perror("Eroare la write!");
        return -1;
    }

    close(fdw);
    return 0;
}

void menu_text() /// introducerea jocului
{
    string text;
    text = "Salut, hai sa jucam jocul Wordle!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Pentru a incepe jocul, scrie cuvantul 'Joaca'!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Daca vrei ca jocul sa se rezolve singur, scrie cuvantul 'Rezolva'!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "In cazul in care nu stii cum se joaca acest joc, scrie 'Ajutor' si vor aparea instructiuni pe ecran!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
}

void instructions() /// cum se joca jocul
{
    string text;
    cout << '\n';
    text = "In acest joc, scopul este simplu: Incearca sa ghicesti un cuvant de 5 litere! ";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Insa, incercarile trebuie sa fie numai cuvinte de 5 litere existente in dictionarul limbii romane!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "De fiecare data cand introduci un cuvant din Dictionar, vei vedea ca sub el vor aparea aceste literele R,Y,G";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "1) Daca o litera de pe pozitia x din cuvantul introdus de tine se afla in cuvantul care trebuie ghicit, pe a x-a pozitie se va afla litera 'Y' ";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "2) Daca litera, in plus, se afla pe aceeasi pozitie ca in cuvantul care trebuie ghicit, pe a x-a pozitie va aparea litera 'G'";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "3) Daca litera nu apartine deloc acelui cuvant, atunci pe a x-a pozitie va aparea litera 'R'";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Nota: jocul nu este case sensitive, asa ca literele mici vor fi tratate ca si cum ar fi litere mari";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Exemplu de inputuri corecte: Sapte, paine, CRUCE, fOaMe";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
}

void beginning_text()
{
    string text;
    cout << '\n';
    text = "Ce vrei sa faci? Scrie aici: ";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
}

void uppercase(string &s) /// inputurile vor fi transformate in cuvinte cu litere mari, ca sa nu fie case-sensitive jocul
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
    }
}

void menu_checker(string s) /// verificarile inputurilor din meniu, daca vrem sa jucam, sa rezolvam sau instructiuni
{
    string text;
    if (s == "JOACA")
        in_menu = false, in_game = true;
    else if (s == "REZOLVA")
        in_menu = false, in_solver = true;
    else if (s == "AJUTOR")
        instructions();
    else // in cazul in care alta comanda este introdusa
    {
        text = "Aceasta nu este o comanda valida!";
        for (int i = 0; i < text.size(); i++)
            cout << text[i] /*, usleep(30000)*/;
        cout << '\n';
    }
}

// Aici se alege cuvantul random
void word_getter(string &cuv)
{
    int n;
    for (int i = 1; i <= 10; i++)
    {
        n = rand() % 11453; /// un for micut inainte ca sa genereze mai multe numere pseudo-random, ca sa adauge putin la randomness
    }
    cuv = dictionar[n];
    // cout<<cuv<<'\n'; //cheat code: scapa de primele slash-uri pentru a afisa pe ecran cuvantul care trebuie ghicit
    cout << '\n';
}

void in_game_text() /// textul care afiseaza numarul de incercari si te indeamna sa mai incerci
{
    string text;
    text = "Ai incercat sa ghicesti cuvantul de ";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << contor;
    text = " ori";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Scrie un cuvant: ";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
}

// Verifica daca cuvantul introdus manual apartine fisierului de cuvinte
int list_verifier(string word)
{
    for (int i = 0; i < 11454; ++i)
    {
        if (word == dictionar[i])
            return 1; /// daca word este in dictionar va returna 1/true, altfel 0/false
    }
    return 0;
}

// Verifica daca cuvantul este eligibil pentru a fi utilizat ca si incercare
int check_validity(string word)
{
    string text;
    if (word.size() != 5) /// daca inputul nu are lungimea 5
    {
        text = "Cuvantul trebuie sa fie format din 5 litere!";
        for (int i = 0; i < text.size(); i++)
            cout << text[i] /*, usleep(30000)*/;
        cout << '\n'
             << '\n';
        return 0;
    }
    for (int i = 0; i < 5; i++)                  // pentru fiecare caracter in parte
        if (!(word[i] >= 'A' && word[i] <= 'Z')) /// daca acel caracter nu este litera mare
        {                                        /// reminder: toate literele mici sunt transformate in litere mari in functia uppercase

            text = "Cuvantul trebuie sa contina doar litere!";
            for (int i = 0; i < text.size(); i++)
                cout << text[i] /*, usleep(30000)*/;
            cout << '\n'
                 << '\n';
            ok_num = true;
            return 0;
        }
    if (list_verifier(word) == 0)
    {
        text = "Cuvantul trebuie sa apartina Dictionarului Limbii Romane!";
        for (int i = 0; i < text.size(); i++)
            cout << text[i] /*, usleep(30000)*/;
        cout << '\n'
             << '\n';
        return 0;
    }
    return 1; /// returneaza 1 daca respecta conditiile if-urilor, altfel returneaza 0
}

// Aici se stabileste informatia cuvantului
void word_verifier(string correct_word, string guess_word, bool &verif)
{
    int cnt = 0;
    for (int i = 0; i < 5; ++i)
        if (correct_word[i] == guess_word[i]) /// verific daca am litere care deja se potrivesc
            state[i] = 'G', cnt++;            /// pun in state[i] G si retin faptul ca am inca o litera pe pozitia buna
        else
            state[i] = '_';

    for (int i = 0; i < 5; ++i)
    {
        bool ok = false;
        if (state[i] == '_')
        { /// daca litera nu se potriveste pe pozitie identica, o caut pe restul pozitiilor
            for (int j = 0; j < 5; ++j)
            {
                if (correct_word[j] == guess_word[i])
                { /// daca la un moment dat gasesc o litera din cuvantul introdus in cel corect, dar nu pe aceeasi pozitie, pun in state[i] 'Y'
                    state[i] = 'Y';
                    ok = true;
                    break;
                }
            }
            if (ok == false)
                state[i] = 'R'; /// pun in state[i] 'R' daca nu gasesc o litera din cuvantul introdus in celalalt
        }
    }
    for (int i = 0; i < 5; i++)
        cout << state[i];
    cout << '\n'
         << '\n';
    if (cnt == 5)
    { /// verific daca toate literele se potrivesc
        verif = true;
    }
}

void word_guessed_text()
{
    string guess, text;
    ok_guess = true;
    text = "Felicitari, ai gasi cuvantul in ";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << contor;
    text = " incercari!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    text = "Vrei sa mai joci o data? DA/NU";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
    cout << '\n';
    while (ok_guess) /// aici va verifica daca userul mai vrea sa joace din nou sau nu
    {
        cin >> guess;
        uppercase(guess);
        if (guess == "NU") /// daca jucatorul scrie NU
            in_game = false, ok_guess = false;
        else if (guess == "DA") /// daca jucatorul scrie DA
        {
            in_game = true, ok_guess = false, ok = false, verif = false, contor = 0; /// resetarea valorilor pentru ca jocul sa o ia de la capat
            text = "Jocul o ia de la capat!";
            for (int i = 0; i < text.size(); i++)
                cout << text[i] /*, usleep(30000)*/;
            cout << '\n';
        }
        else /// in caz ca scrie altceva
        {
            text = "Te rog, scrie DA/NU!";
            for (int i = 0; i < text.size(); i++)
                cout << text[i] /*, usleep(30000)*/;
            cout << '\n';
        }
    }
}

void game_over()
{
    string text;
    text = "Joc terminat!";
    for (int i = 0; i < text.size(); i++)
        cout << text[i] /*, usleep(30000)*/;
}

/// MAIN///

int main()
{
    string guess, cuv;
    srand(time(NULL));
    init_dictionar();
    menu_text();

    while (in_menu)
    {
        beginning_text();
        cin >> guess;
        uppercase(guess);
        menu_checker(guess);
    }

    while (in_game) /// aici va incepe jocul jucat de o persoana
    {
        if (ok == false)                 /// daca nu a fost ales un cuvant
            word_getter(cuv), ok = true; /// alegerea cuvantului
        in_game_text();
        cin >> guess;
        uppercase(guess);
        if (!check_validity(guess)) /// daca nu este valid cuvantul(nu e format din 5 caractere, doar litere sau nu apartine Dictionarului Limbii Romane)
            continue;
        contor++;                         /// daca este valid, inseamna ca s-a produs o incercare buna si se tine minte
        word_verifier(cuv, guess, verif); /// verifica apartenenta literelor prin functia word_verifier. bineinteles si guess-ul va fi case insensitive
        if (verif == true)                /// daca a gasit cuvantul potrivit
            word_guessed_text();
    }

    if (in_solver) // daca programul a ajuns in solver
    {
        // impartim procesul in doua
        int id = fork();
        if (id == -1) // verificam erori
        {
            perror("Fork: ");
            return -1;
        }
        if (id == 0) // in procesul copil supra scriem procesul programului de solve
        {
            if (execl("solve", "solve", NULL) == -1)
            {
                perror("Executie: ");
                return -1;
            }
        }
        else // in procesul parinte continuam executia programului curent
        {
            // Creeaza fisierul pentru IPC
            if (mkfifo(FIFO, 0777) != 0)
            {
                perror("FIFO: ");
                return -1;
            }
            while (in_solver) // aici va incepe jocul sa se rezolve singur
            {
                if (ok == false)
                {
                    // aici o sa lasam userul sa aleaga optiunea pe care o vrea la solver
                    cout << "Alege cum vrei sa rezolve: \n";
                    cout << "- Alege tu un cuvant din lista de cuvinte => 1\n";
                    cout << "- Lasa jocul sa aleaga un cuvant aleator din lista de cuvinte => 2\n";
                    char raspuns;
                    bool gresit = false;
                    do // daca nu da comanda buna repetam procesul
                    {
                        cin >> raspuns;
                        while(cin.get() != '\n'){;}
                        if (raspuns == '1')
                        {
                            // daca intra aici inseamna ca o sa aleaga un cuvant din lista
                            gresit = false;
                            bool corect = true;
                            cout << "Alege un cuvant din lista de cuvinte: ";
                            do // cat timp cuvantul nu apare in lista il punem sa mai aleaga odata
                            {
                                cin >> cuv;
                                uppercase(cuv);
                                cout << endl;
                                corect = list_verifier(cuv);
                                if (!corect)
                                    cout << "Cuvantul nu apartine de lista, mai alege odata: ";
                            } while (!corect);
                        }
                        else if (raspuns == '2')
                        {
                            //daca a ajuns aici inseamna ca programul nostru trebuie 
                            //sa aleaga un cuvant random
                            gresit = false;
                            word_getter(cuv);
                        }
                        else
                        {
                            //nu a bagat comanda buna
                            gresit = true;
                        }
                        if (gresit)
                        {
                            cout << "Codul nu este corect mai introdu odata (1 sau 2): \n";
                        }
                    } while (gresit);
                    cout << '\n';
                    ok = true; // punem ok=true ca sa nu ne mai intre in acest if
                }

                char c[6];
                if (Read(c) != 0) // citim guess-ul
                {
                    return -1;
                }

                cout << "\n";
                contor++;

                word_verifier(cuv, c, verif); //verificam si setam state 

                if (Write(state) != 0) // scriem state pentru celalalt program sa citeasca
                {
                    return -1;
                }

                if (verif)
                {
                    printf("Ai gasit cuvantul in %d incercari\n", contor);
                    break;
                }
            }

            remove(FIFO); // stergem fifo-ul
        }
    }
    game_over();
    return 0;
}
