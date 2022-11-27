Proiect realizat de:

Cuțuliga Razvan (grupa 144)
Neagoe Mihai (grupa 144)
Barbu Eduard (grupa 144)
Șteopoaie Vlad (grupa 144)


Descrierea proiectului:

Proiectul nostru permite unui utilizator atat sa joace jocul Wordle, fara a avea stresul limitei de incercari,
dar si sa foloseasca un algoritm bazat pe teoria informatiei, astfel incat nu utilizatorul, ci calculatorul, sa
fie cel care ghiceste cuvantul cat mai optim posibil. Programul principal este constituit din 3 parti

I) Jocul Wordle in sine, care permite utilizatorului sa introduca un cuvant de la tastatura, care va trece prin
urmatoarele verificari:
	1) Verificarea validitatii cuvantului: bineinteles, utilizatorul trebuie sa introduca un cuvant din 
	Dictionarul Limbii Romane, de 5 litere. Asa ca cifre si caractere speciale nu sunt permise, iar
	lungimea cuvantului va fi verificata
	2) Afisarea informatiei cuvantului: cand utilizatorul introduce un cuvant valid, trebuie sa stim
	despre fiecare litera in parte daca apartine cuvantului pe care acesta incearca sa il ghiceasca,
	se afla si pe aceeasi pozitie in cuvant, sau nu apartine deloc
Inainte de inceperea propriu-zisa a jocului, utilizatorul va putea alege o optiune care ii va explica
regulile jocului si cum va fi simbolizata informatia despre fiecare litera in parte. De asemenea, dupa ce
ghiceste cuvantul, poate sa aleaga sa primeasca un cuvant nou, reluand jocul. 

II) Algoritmul care rezolva jocul Wordle

Cum ghiceste?

-Programul trebuie sa ghiceasca un cuvant de 5 litere dintr-o lista de cuvinte.
-Acest lucru se realizeaza cu ajutorul unor state-uri pe care le primeste de la joc in sine.
-De exemplu, daca programul "solve" incearca cuvantul VREAU acesta v-a primi de la programul "game" un state (RRYRG, unde R - rosu, litera nu apare in cuvant; Y - galben, litera apare in cuvant dar nu este pe acea pozitie; G - verde, litera se afla in cuvant pe acea pozitie)
-In functie de state-ul primit programul calculeaza entropia tuturor cuvintelor care respecta acel state si alege cuvantul cu entropia cea mai mare pe care il propune ca urmator guess.

Cum calculeaza entropia?

-Programul face, pentru fiecare cuvant din lista de cuvinte, un backtraking de produs cartezian si trece prin toate state-urile posibile (RRRRR, RRRRY, RRRRG, RRRYR, ...).
-Pentru fiecare state din backtracking programul verfifica cate cuvinte din lista respecta acel state (ex: cuvantul VREAU si cuvantul TAREI sunt echivalente luand in considerare state-ul RYYYR).
-Numarul de cuvinte obtinut / numarul total de cuvinte din lista = probabilitatea ca un cuvant sa respecte acel state. 
-1 / probabilitate = informatia
-Iar entropia este media ponderata a informatiilor
-Dupa programul alege cuvantul cu entropia cea mai mare ca si urmator guess
-La final in lista de cuvinte se pun doar cuvintele care respecta state-ul primit


III) Comunicarea dintre programe

-Realizat prin named pipes (FIFO)
-Programul "game" creeaza un fisier fifo si un proces copil prin care se executa programul de "solve"
-Cele doua programe comunica prin fisierul fifo numit "guess"



