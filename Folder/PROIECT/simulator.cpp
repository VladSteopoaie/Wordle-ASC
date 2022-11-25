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
#include <sys/wait.h>

using namespace std;

#define FIFO "guess"

ifstream f("../cuvinte_wordle.txt");
ofstream fo("cuvinte_gasite.txt");

bool in_solver = true;
char state[6];
string line;

/// pentru textul din meniu si instructiuni va fi folosita functia usleep, pentru a nu afisa un wall of text instant.

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

    if (cnt == 5)
    { /// verific daca toate literele se potrivesc
        verif = true;
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
    string cuv;
    int nrCuv = 0;
    if (mkfifo(FIFO, 0777) != 0)
    {
        perror("FIFO: ");
        return -1;
    }
    while (f >> cuv)
    {
        nrCuv++;
        fo << nrCuv << ". ";
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
            fo << cuv << ": ";

            int contor = 0;
            while (in_solver) // aici va incepe jocul sa se rezolve singur
            {
                if (contor > 100) // in caz ca avem loop infinit
                {
                    printf("loop infinit in while");
                    return -1;
                }
                char c[6];
                if (Read(c) != 0)
                {
                    return -1;
                }
                fo << c << ' ';
                contor++;

                bool verif = false;
                word_verifier(cuv, c, verif);

                if (Write(state) != 0)
                {
                    return -1;
                }
                if (verif)
                {
                    fo << contor << '\n';
                    break;
                }
            }
            fo.flush();
            wait(NULL); // asteptam procesul copil
        }
    }
    remove(FIFO); // stergem fifo-ul
    game_over();
    return 0;
}
