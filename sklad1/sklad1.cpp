// sklad1.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#include <string>
#include <ctime>
#include <iostream>
#include <limits>

using namespace std;

class Record {
public:
    int id;
    string title;
    unsigned int ks;
    double price;
    string category;
    time_t datum_posledniho_naskladneni;
    Record* left;
    Record* right;

    Record(int id, string title, unsigned int ks, double price, string category)
        : id(id), title(title), ks(ks), price(price), category(category), left(nullptr), right(nullptr) {
        time(&datum_posledniho_naskladneni);
    }
};

class Sklad {
private:
    Record* root;

    Record* vloz(Record* node, int id, string title, unsigned int ks, double price, string category) {
        if (node == nullptr) {
            return new Record(id, title, ks, price, category);
        }
        if (id < node->id) {
            node->left = vloz(node->left, id, title, ks, price, category);
        }
        else if (id > node->id) {
            node->right = vloz(node->right, id, title, ks, price, category);
        }
        else {
            node->ks += ks; // Aktualizace počtu kusů
            time(&node->datum_posledniho_naskladneni); // Aktualizace data
        }
        return node;
    }

    void vypis(Record* node) {
        if (node != nullptr) {
            vypis(node->left);

            // Použití ctime_s pro bezpečný převod času na řetězec
            char buffer[26];
            ctime_s(buffer, sizeof(buffer), &node->datum_posledniho_naskladneni);

            cout << "ID: " << node->id << ", Název: " << node->title << ", Kategorie: " << node->category
                << ", Počet kusů: " << node->ks << ", Cena: " << node->price
                << ", Datum naskladnění: " << buffer;
            vypis(node->right);
        }
    }

public:
    Sklad() : root(nullptr) {}

    void vloz_na_sklad() {
        int id;
        string title, category;
        unsigned int ks;
        double price;

        cout << "Zadejte ID výrobku: ";
        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Zadejte název výrobku: ";
        getline(cin, title);

        cout << "Zadejte počet kusů na skladě: ";
        cin >> ks;

        cout << "Zadejte cenu výrobku: ";
        cin >> price;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Zadejte kategorii výrobku: ";
        getline(cin, category);

        root = vloz(root, id, title, ks, price, category);
        cout << "Položka byla přidána na sklad.\n";
    }

    void vypis() {
        if (root == nullptr) {
            cout << "Sklad je prázdný.\n";
        }
        else {
            vypis(root);
        }
    }
};

int main() {
    Sklad mujSklad;
    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Přidat nové zboží\n";
        cout << "2. Výpis všech položek\n";
        cout << "3. Konec\n";
        cout << "=================\n";
        cout << "Vyberte možnost (1-3): ";
        cin >> choice;

        switch (choice) {
        case 1:
            mujSklad.vloz_na_sklad();
            break;
        case 2:
            mujSklad.vypis();
            break;
        case 3:
            cout << "Ukončuji program.\n";
            break;
        default:
            cout << "Neplatná volba. Zkuste to znovu.\n";
            break;
        }
    } while (choice != 3);

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
