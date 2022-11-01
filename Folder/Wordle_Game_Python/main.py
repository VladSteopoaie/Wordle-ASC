import random
file=open("ListaCuvinte.txt","r")  #deschide fisierul de cuvinte
data=file.read() #citeste fisierul
lista_cuv=data.replace('\n'," ").split(" ") #aici fac lista de cuvinte, inlocuind randul liber cu spatiu pt a imparti bine
lista_cuv.pop()#ramane un element vid la final, pe care il scot
#print(data_list) #pentru verificare
# aparent ZURRR este un cuvant?

ans=random.choice(lista_cuv) #cuvantul care trebuie gasit
print(ans) #sa fie mai usoara verificarea
#print(ord('a')) #ord arata codul ascii al unei litere, valoare int!
tries=0
found=False
g_counter=0 #asa verific daca toate literele sunt la locul lor
while found==False: #cat timp cuvantul nu a fost gasit
    guess=input("Please type in a word: ")
    if guess.isalpha() is False:
        print("Your answer must have letters only!") #inputul are chestii care nu sunt litere
    elif len(guess)!=5:
        print("Your answer must be 5 letters long!") #inputul are lungimea diferita de 5
    elif guess.upper() not in lista_cuv:
        print("Your answer must be a word!") #inputul nu e din dictionar
    else:
        fv=[0 for i in range(26)] #initializez vectorul de frecventa
        for i in range(5):
            fv[ord(guess[i].upper())-ord('A')]+=1 #actualizez vectorul de frecventa cu aparitia literelor in cuvant(ce usor era in C++...)
            #^^^^scriu asa ca sa am vectorul de frecventa doar cu 26 elemente, adica de la 0 la 25. Altfel ar fi trebuit sa aiba peste 100 elemente
        print(fv)
        for i in range(5):
            if guess[i]==ans[i]: #daca litera din cuvantul incercat se afla pe pozitia corecta
                fv[ord(guess[i].upper())-ord('A')]-=1 #ii scad aparitia
                print("G",end=" ")
                g_counter=g_counter+1 #tin minte ca am o litera pe pozitia buna
        for i in range(5):
            if guess[i] in ans: #daca litera se afla in cuvant
                if fv[ord(guess[i].upper())-ord('A')]>0: #daca litera respectiva mai apare cel putin inca o data in cuvant:
                    fv[ord(guess[i].upper())-ord('A')]-=1 #ii scad aparitia
                    print("Y",end=" ")
            else: #daca litera aceea nu apare deloc sau a aparut deja de cate ori e nevoie de ea(de ex daca un cuvant are un singur i si inputul are 2 i)
                print("N",end=" ")
        tries=tries+1
        print()
        print(f'You have attempted to guess the word {tries} times!')
        if g_counter==5:
            found=True
            print(f'Well done, the word is {ans}')
        else:
            g_counter=0


#De facut: de reparat cazul cand o litera