// Úkol_Frőmel.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>

//SEMESTRÁLNÍ PŘÍKLAD
// Máte k dispozici 12 zdrojových unaků s danou pravděpodobnsotí výskytu.(Tyto pravděpodobnsoti si zvolte náhodně).
const char abeceda[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' };
const int size = 12;  // velikost abecedy
const int count = 50; // délka generovaného řetězce 
// 1) Určete množství informace, které nese každý znak této zdrojové abecedy.
void entrop_pro_znaky(const double letterProbability[]) {
    std::cout << "Mnozstvi informace pro kazdy znak (v bitech):\n";
    for (int i = 0; i < size; i++) {
        if (letterProbability[i] > 0) {
            double information = -log2(letterProbability[i]);
            std::cout << "Pismeno:" << (char)('A'+ i)
                      << " | Mnozstvi informace: " << information << " bitu" << std::endl;
        }
    }
}
// 2) Vypočítejte průměrné množství informace, které nese 1 znak zdrojové abecedy.
    void entrop_pro_znak(const double letterProbability[]) {
        double entropy = 0;
        for (int i = 0; i < size; i++) {
            if (letterProbability[i] > 0) {
                entropy += letterProbability[i] * log2(letterProbability[i]);
            }
        }
        entropy = -entropy;
        std::cout << " Mnozstvi informace na jeden znak : " << entropy << " bitu\n" << std::endl;
    }

// 3) Sestrojte binární kód pro tuto zdrojovou abecedu dle metody Shannon-Fanovy. Určete průměrnou délku kódového slova a efektivnost kódu. Pro výsledný kód sestrojte binární strom. 
// Generování náhodného vstupního řetězce
void generateRandomData(char inputData[], int length) {
    srand(time(0));  // inincializace generátoru náhodných čísel
    for (int i = 0; i < length; ++i) {
        inputData[i] = abeceda[rand() % size];  // náhodně vybraný znak z abecedy
    }
    inputData[length] = '\0'; // zakončení řetězce 
}
// Výpočet Shannonova kódování
void calculateShannon(double letterProbability[], int letterIndexes[], char codedLetters[][10], int length) {
    int start[50], end[50];
    int groupCount = 1;
    start[0] = 0;
    end[0] = length;

    while (groupCount > 0) {
        groupCount--;
        int startingIndex = start[groupCount];
        int endingIndex = end[groupCount];
        if (endingIndex - startingIndex <= 1)
            continue;

        // Výpočet průměrné hodnoty (rozdělení podle pravděpodobností)
        double averageNumber = 0;
        for (int i = startingIndex; i < endingIndex; i++) {
            averageNumber += letterProbability[i];

        }
        averageNumber = averageNumber / 2;


        double calculatingNumber = letterProbability[startingIndex];
        int line = startingIndex;

        //Výpočet průměrné hodnoty (rozdělení podle pravděpodobností)
        while (line + 1 < endingIndex && fabs(calculatingNumber - averageNumber) > fabs(calculatingNumber + letterProbability[line + 1] - averageNumber)) {
            line++;
            calculatingNumber += letterProbability[line];
        }

        // Přiřazení bitu '0' nebo '1' podle umístění ve skupině
        for (int i = startingIndex; i < endingIndex; i++) {
            if (i <= line) {

                int len = strlen(codedLetters[i]);
                codedLetters[i][len] = '0';
                codedLetters[i][len + 1] = '\0';
            }
            else {

                int len = strlen(codedLetters[i]);
                codedLetters[i][len] = '1';
                codedLetters[i][len + 1] = '\0';
            }
        }

        // Přidání nových skupin pro další dělení
        if (line + 1 - startingIndex > 1) {
            start[groupCount] = startingIndex;
            end[groupCount] = line + 1;
            groupCount++;
        }

        if (endingIndex - (line + 1) > 1) {
            start[groupCount] = line + 1;
            end[groupCount] = endingIndex;
            groupCount++;
        }
    }
}
// 5) Z výsledného kódu vytvořte kód zabezpečený. Použíte zabezpečení pomocí sudé parity pro Shannon - Fanův kód.
void Zabez_kod(char codedLetters1[][10], char codedLetters[][10], int a, int letterIndexes[]) {
    for (int i = 0; i < a; i++) {
        int delka = strlen(codedLetters[i]);  // Spočítáme délku kódu
        int ones = 0;
        int nulls = 0;
        for (int j = 0; j < delka; j++) {
            if (codedLetters[i][j] == '1') {
                ones++;
            }
            else if(codedLetters[i][j] == '0'){
                nulls++;
            }
        }
        char parity;
        if (ones % 2 == 0) {
            parity = '0';
        }
        else {
            parity = '1';
        }
        for (int j = 0; j < delka; j++) {
            codedLetters1[i][j] = codedLetters[i][j];
        } 
        codedLetters1[i][delka] = parity;
        codedLetters1[i][delka + 1] = '\0';

        
        char letter1 = 'A' + letterIndexes[i];
        std::cout << "Pismeno: " << letter1 <<" = "<< codedLetters1[i] << std::endl;
        

    }
}

int main() {
    char inputData[count + 1];  // Vstupní náhodný řetězec
    int counts[size] = { 0 };    // Četnost jednotlivých znaků
    double letterProbability[size] = { 0 };  // Pravděpodobnosti výskytu znaků
    int letterIndexes[size];  // Indexy znaků v abecedě
    char codedLetters[size][10] = {};  // Kódy jednotlivých znaků
    char codedLetters1[size][10] = {};

    generateRandomData(inputData, count);
    std::cout << "Vygenerovana veta: " << inputData << std::endl;

    // Spočítej četnosti výskytu každého znaku
    for (int i = 0; i < count; ++i) {
        counts[inputData[i] - 'A']++;
    }

    // Spočítej pravděpodobnosti výskytu
    for (int i = 0; i < size; i++) {
        if (counts[i] != 0) {
            letterProbability[i] = static_cast<double>(counts[i]) / count;
        }
    }

    std::cout << " 1) Urcete mnozstvi informace, ktere nese kazdy znak teto zdrojove abecedy. " << std::endl;
    entrop_pro_znaky(letterProbability);

    std::cout << " 2) Vypocitejte prumerne mnozstvi informace, ktere nese 1 znak zdrojove abecedy. " << std::endl;
    entrop_pro_znak(letterProbability);

    // Filtruj znaky s nenulovou pravděpodobností
    int a = 0;
    for (int i = 0; i < size; i++) {
        if (letterProbability[i] != 0) {
            letterIndexes[a] = i;
            a++;
        }
    }

    // Seřaď znaky podle pravděpodobnosti sestupně
    for (int i = 0; i < a - 1; i++) {
        for (int j = 0; j < a - 1 - i; j++) {
            if (letterProbability[letterIndexes[j]] < letterProbability[letterIndexes[j + 1]]) {
                int temp = letterIndexes[j];
                letterIndexes[j] = letterIndexes[j + 1];
                letterIndexes[j + 1] = temp;
            }
        }
    }

    // Vytvoř novou kopii pravděpodobností v seřazeném pořadí
    double letterProbability_[size];
    for (int i = 0;i < a;i++) {
        letterProbability_[i] = letterProbability[letterIndexes[i]];
    }

    // Vypočítej Shannonovy kódy
    calculateShannon(letterProbability_, letterIndexes, codedLetters, a);
    // Výpis Shannonových kódů
    std::cout << "\n3) Shannon kody:\n";
    for (int i = 0; i < a; i++) {
        if (letterIndexes[i] >= 0 && letterIndexes[i] < sizeof(codedLetters) / sizeof(codedLetters[0]) && letterIndexes[i] < sizeof(counts) / sizeof(counts[0])) {
            char letter = 'A' + letterIndexes[i];
            std::cout << letter << " = " << codedLetters[i]
                << " Cetnsot vyskytu: " << counts[letterIndexes[i]]
                << " * " << letterProbability[letterIndexes[i]]
                << " = " << (double)counts[letterIndexes[i]] * letterProbability[letterIndexes[i]]
                << std::endl;
        }
        else {
            std::cerr << "Index out of bounds: " << letterIndexes[i] << std::endl;
        }
    }

    std::cout << "3) Prumerna delka kodoveho slova " << std::endl;
    double Delka = 0.0; 
    for (int i = 0; i < a; i++) {
        int delka = strlen(codedLetters[i]);
        Delka += delka * letterProbability[letterIndexes[i]];
    }

    std::cout << "Velikost je: " << Delka << std::endl;

    std::cout << "3) Efektivnost kodu " << std::endl;
    double entropy = 0.0;

    for (int i = 0; i < a; i++) {
        entropy += letterProbability[letterIndexes[i]] * log2(letterProbability[letterIndexes[i]]);
    }
    entropy = -entropy;

    double efektivnost_kodu = (entropy / Delka) * 100.0;
    std::cout << "Efektivnost kodu je: " << efektivnost_kodu << " %" << std::endl;

    // 5) Z výsledného kódu vytvořte kód zabezpečený. Použíte zabezpečení pomocí sudé parity pro Shannon - Fanův kód.
    std::cout << "5) Z vysledneho kodu vytvorte kod zabezpeceny. Pouzite zabezpeceni pomoci sude parity pro Shannon - Fanuv kod.:" << std::endl;
    Zabez_kod(codedLetters1, codedLetters, a, letterIndexes);


    // Zakóduj vstupní řetězec pomocí získaných kódů
    std::cout << "\nZakodovany retezec:\n";
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < a; j++) {
            if (inputData[i] - 'A' == letterIndexes[j]) {
                std::cout << codedLetters[j] << " ";
                break;
            }
        }
    }
    return 0;
}


// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
