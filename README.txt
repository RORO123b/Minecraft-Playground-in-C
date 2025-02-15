Rezolvare task 1:
- pentru a rezolva acest task verific daca pozitia unde se doreste a adauga blockul se afla in limitele chunkului
- daca da il plasez, daca nu nu-l plasez
- returnez chunkul

Rezolvare task 2:
- verific daca x0, y0, z0, sunt mai mici decat x1, y1, respectiv z1, daca nu le schimb
- adaug blockurile in limitele impuse de cerinta
- returnez chunkul

Rezolvare task 3:
- pentru rezolvarea acestui task ma folosesc de formula matematica de a determina 
lungimea dintre 2 puncte si vad daca este mai mica decat raza
- verific astfel, parcurgand intreg chunkul, daca blockul respectiv se afla in sfera si il adaug daca da
- returnez chunkul

Rezolvare task 4:
- ma folosesc de 3 vectori de directie pentru a ma deplasa mai usor in jurul blockului
- retin intr-un vector de blockuri pozitiile target_blockurilor
- parcurg acest vector si adaug shellul fiecarui target_block
- returnez chunkul

Rezolvare task 5:
- aplic algoritmul fill 2D recursiv
- verific daca blockul la care sunt este de tipul respectiv
- daca da il inlocuiesc si parcurg recursiv in continuare forma 2D
- returnez chunkul

Rezolvare task 6:
- aplic fill 3D recursiv
- verific daca blockul la care sunt este de tipul respectiv
- daca da il inlocuiesc si parcurg recursiv in continuare forma 3D
- returnez chunkul

Rezolvare task 7:
- creez un nou chunk (cchunk) care are noile dimensiuni
- ii atribui valorile lui cchunk in pozitiile corespunzatoare (cchunk[x][y][z] = chunk[z][y][depth - x - 1])
- returnez cchunkul

Rezolvare task 8:
- voi numi obiect, corpul conform definitiei din cerinta
- voi numi mega obiect mai multe obiecte lipite intre ele indiferent de tipul blockului din care sunt formate
- mai multe mega obiecte pot forma un mega obiect
- pentru a rezolva acest task am decis sa-l impart in doua sub-taskuri
- Subtask 1: caderea mega obiectelor
- Subtask 2: caderea obiectelor
- pentru rezolvarea lor ma voi folosi in permanenta de o copie a chunkului (cchunk)
- de asemenea m-am inspirat de la algoritmul de sortare Bubble Sort care presupune la fiecare iteratie ca vectorul este sortat si
gaseste un contra exemplu in vector ca sa verifice inca o data vectorul
- asa am procedat si eu, am presupus ca toate obiectele/mega obiectele au cazut de tot si gaseam de fiecare data un contra exemplu,
pana cand cad toate

    Subtask 1:
    * parcurg chunkul, iar daca gasesc un mega obiect il marchez si ii pun toate pozitiile blockurilor intr-un vector de blockuri
    * avand pozitiile fiecarui block verific daca poate sa cada
    * ma folosesc de observatia ca orice mega obiect poate sa cada doar daca nu se afla deja pe pamant, deoarece, cum am spus si mai sus,
    daca 2 mega obiecte se lipesc ele formeaza alt mega obiect, astfel daca un mega obiect cade pe alt mega obiect care se afla deja pe pamant,
    ele vor forma un mega obiect care nu mai are cum sa cada
    * daca poate sa cada mut fiecare block din mega obiect cu un y mai jos

    Subtask 2:
    * pentru acest task este similar, doar ca se aplica obiectelor, nu mega obiectelor
    * singura diferenta considerabila este faptul ca obiectul poate sa cada doar daca fiecare block din el are ori block de aer sub el, ori
    block de acelasi tip ca al lui (interiorul obiectului practic)

- returnez chunkul

Rezolvare task 9:
- pentru rezolvarea acestui task ma folosesc de o metoda tip greedy, in sensul ca parcurg chunkul si masor lungimea fiecarui run, pe care-l pun
intr-un vector de unsigned chars numit code
- imi impart aceasta problema in mai multe case-uri
- in codul meu variabila c il reprezinta pe n din cerinta

    Small case (pentru n < 32)
    * marchez primii doi biti cu blockul respectiv
    * urmatorii biti sunt bitii lui n

    Medium case (pentru 32 <= n < 4096)
    * marchez primii doi biti cu blockul respectiv
    * pun cifra 1 pe al 3 lea bit
    * urmatorii biti sunt cei 5 cei mai semnificativi biti (daca q = 12, il shiftez la dreapta pe n cu 8 biti)
    * maresc lungimea vectorului
    * in urmatorul "compartiment" pun pe c fara primii 5 cei mai semnificativi biti
    * reusesc acest lucru prin transformarea primilor 5 cei mai semnificativi biti in 00000 (fac sau logic intre n si 2^8 - 1)

    Big case (pentru n >= 4096)
    * pana cand n < 4096 eu pun valoarea blockului, pe al 3 lea bit pun 1, si umplu restul cu 1 si urmatorul "compartiment" si repet procesul
    * dupa ce n devine < 4096 putem intra in small case sau in medium case

- returnez codeul

Rezolvare task 10:
- se observa pentru la decodificare medium case si big case pot fi imbinate pentru o scriere mai usoara
- pentru a diferentia small case ul si noul case putem verifica al 3 lea bit
- daca este 0 este small case, daca este 1 este noul case
- daca este small case adaug in chunk blockul codificat in primii 2 cei mai semnificativi biti, de cate ori scrie in numarul codificat in
ultimii 5 biti (sau 6 pentru ca acel bit este 0 si nu face diferenta)
- daca este noul case ma uit la "compartimentul" curent si la urmatorul
- plasez blockul codificat in primii 2 biti de cate ori scrie in numarul format din lipirea bitilor dintre primul (doar ultimii 4 biti) si
al doilea "compartiment"
- repet procesul in continuare pana ating inaltimea maxima a chunkului (nu ni se da lungimea codului si asa m-am gandit eu ca ar trebui sa procedez)
