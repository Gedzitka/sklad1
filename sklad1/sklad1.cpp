#include <locale>
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

            char buffer[26];
            ctime_s(buffer, sizeof(buffer), &node->datum_posledniho_naskladneni);

            cout << "ID: " << node->id << ", Název: " << node->title << ", Kategorie: " << node->category
                << ", Počet kusů: " << node->ks << ", Cena: " << node->price
                << ", Datum naskladnění: " << buffer;
            vypis(node->right);
        }
    }

    void vyhledat_kategorii(Record* node, const string& category) {
        if (node != nullptr) {
            vyhledat_kategorii(node->left, category);
            if (node->category == category) {
                char buffer[26];
                ctime_s(buffer, sizeof(buffer), &node->datum_posledniho_naskladneni);

                cout << "ID: " << node->id << ", Název: " << node->title << ", Počet kusů: " << node->ks
                    << ", Cena: " << node->price << ", Datum naskladnění: " << buffer;
            }
            vyhledat_kategorii(node->right, category);
        }
    }

    Record* vyhledat_id(Record* node, int id) {
        if (node == nullptr || node->id == id)
            return node;

        if (id < node->id)
            return vyhledat_id(node->left, id);

        return vyhledat_id(node->right, id);
    }

    Record* odstranit(Record* node, int id) {
        if (node == nullptr) return nullptr;

        if (id < node->id) {
            node->left = odstranit(node->left, id);
        }
        else if (id > node->id) {
            node->right = odstranit(node->right, id);
        }
        else {
            if (node->left == nullptr) {
                Record* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Record* temp = node->left;
                delete node;
                return temp;
            }
            else {
                Record* temp = min_value_node(node->right);
                node->id = temp->id;
                node->title = temp->title;
                node->ks = temp->ks;
                node->price = temp->price;
                node->category = temp->category;
                node->datum_posledniho_naskladneni = temp->datum_posledniho_naskladneni;
                node->right = odstranit(node->right, temp->id);
            }
        }
        return node;
    }

    Record* min_value_node(Record* node) {
        Record* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
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

    void hledat_podle_kategorie() {
        string category;
        cout << "Zadejte kategorii: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, category);

        cout << "Výsledky vyhledávání:\n";
        vyhledat_kategorii(root, category);
    }

    void hledat_podle_id() {
        int id;
        cout << "Zadejte ID: ";
        cin >> id;

        Record* result = vyhledat_id(root, id);
        if (result) {
            char buffer[26];
            ctime_s(buffer, sizeof(buffer), &result->datum_posledniho_naskladneni);

            cout << "ID: " << result->id << ", Název: " << result->title << ", Kategorie: " << result->category
                << ", Počet kusů: " << result->ks << ", Cena: " << result->price
                << ", Datum naskladnění: " << buffer;
        }
        else {
            cout << "Položka nebyla nalezena.\n";
        }
    }

    void odstranit_polozku() {
        int id;
        cout << "Zadejte ID položky, kterou chcete odstranit: ";
        cin >> id;

        root = odstranit(root, id);
        cout << "Položka byla odstraněna (pokud existovala).\n";
    }
};

int main() {
    
    Sklad mujSklad;
    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Přidat nové zboží\n";
        cout << "2. Výpis všech položek\n";
        cout << "3. Hledat podle kategorie\n";
        cout << "4. Hledat podle ID\n";
        cout << "5. Odstranit položku\n";
        cout << "6. Konec\n";
        cout << "=================\n";
        cout << "Vyberte možnost (1-6): ";
        cin >> choice;

        switch (choice) {
        case 1:
            mujSklad.vloz_na_sklad();
            break;
        case 2:
            mujSklad.vypis();
            break;
        case 3:
            mujSklad.hledat_podle_kategorie();
            break;
        case 4:
            mujSklad.hledat_podle_id();
            break;
        case 5:
            mujSklad.odstranit_polozku();
            break;
        case 6:
            cout << "Ukončuji program.\n";
            break;
        default:
            cout << "Neplatná volba. Zkuste to znovu.\n";
            break;
        }
    } while (choice != 6);

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
