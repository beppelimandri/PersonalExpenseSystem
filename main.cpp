#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

sqlite3* db;

/* ================= FUNZIONI DI SUPPORTO ================= */

void eseguiQuery(const string& sql) {
    char* errorMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMsg);
    if (rc != SQLITE_OK) {
        cout << "Errore SQL: " << errorMsg << endl;
        sqlite3_free(errorMsg);
    }
}

bool categoriaEsiste(const string& nome) {
    sqlite3_stmt* stmt;
    string sql = "SELECT id FROM categorie WHERE nome = ?";

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, nome.c_str(), -1, SQLITE_STATIC);

    bool esiste = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);

    return esiste;
}

/* ================= MODULO 1 ================= */
void gestioneCategorie() {
    string nome;
    cout << "Nome categoria: ";
    cin >> nome;

    if (nome.empty()) {
        cout << "Errore: nome categoria non valido.\n";
        return;
    }

    if (categoriaEsiste(nome)) {
        cout << "La categoria esiste già.\n";
        return;
    }

    string sql = "INSERT INTO categorie (nome) VALUES ('" + nome + "')";
    eseguiQuery(sql);
    cout << "Categoria inserita correttamente.\n";
}

/* ================= MODULO 2 ================= */
void inserisciSpesa() {
    string data, categoria, descrizione;
    double importo;

    cout << "Data (YYYY-MM-DD): ";
    cin >> data;

    cout << "Importo: ";
    cin >> importo;

    if (importo <= 0) {
        cout << "Errore: l'importo deve essere maggiore di zero.\n";
        return;
    }

    cout << "Categoria: ";
    cin >> categoria;

    if (!categoriaEsiste(categoria)) {
        cout << "Errore: la categoria non esiste.\n";
        return;
    }

    cout << "Descrizione (una parola): ";
    cin >> descrizione;

    string sql =
        "INSERT INTO spese (data, importo, categoria_id, descrizione) "
        "VALUES ('" + data + "', " + to_string(importo) +
        ", (SELECT id FROM categorie WHERE nome = '" + categoria + "'), '" +
        descrizione + "')";

    eseguiQuery(sql);
    cout << "Spesa inserita correttamente.\n";
}

/* ================= MODULO 3 ================= */
void definisciBudget() {
    string mese, categoria;
    double importo;

    cout << "Mese (YYYY-MM): ";
    cin >> mese;

    cout << "Categoria: ";
    cin >> categoria;

    if (!categoriaEsiste(categoria)) {
        cout << "Errore: categoria inesistente.\n";
        return;
    }

    cout << "Importo budget: ";
    cin >> importo;

    if (importo <= 0) {
        cout << "Errore: il budget deve essere positivo.\n";
        return;
    }

    string sql =
        "INSERT OR REPLACE INTO budget (mese, categoria_id, importo) "
        "VALUES ('" + mese + "', (SELECT id FROM categorie WHERE nome = '" +
        categoria + "'), " + to_string(importo) + ")";

    eseguiQuery(sql);
    cout << "Budget mensile salvato correttamente.\n";
}

/* ================= REPORT ================= */

int callbackReport(void* , int argc, char** argv, char**) {
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << "\t";
    }
    cout << endl;
    return 0;
}

void visualizzaReport() {
    int scelta;
    char* errorMsg = nullptr;

    do {
        cout << "\n------ REPORT ------\n";
        cout << "1. Totale spese per categoria\n";
        cout << "2. Elenco completo delle spese\n";
        cout << "3. Torna al menu principale\n";
        cout << "Scelta: ";
        cin >> scelta;

        switch (scelta) {
            case 1:
                cout << "\nCategoria\tTotale Speso\n";
                sqlite3_exec(
                    db,
                    "SELECT c.nome, SUM(s.importo) "
                    "FROM spese s "
                    "JOIN categorie c ON s.categoria_id = c.id "
                    "GROUP BY c.nome;",
                    callbackReport,
                    nullptr,
                    &errorMsg
                );
                break;

            case 2:
                cout << "\nData\t\tCategoria\tImporto\tDescrizione\n";
                sqlite3_exec(
                    db,
                    "SELECT s.data, c.nome, s.importo, s.descrizione "
                    "FROM spese s "
                    "JOIN categorie c ON s.categoria_id = c.id "
                    "ORDER BY s.data;",
                    callbackReport,
                    nullptr,
                    &errorMsg
                );
                break;

            case 3:
                break;

            default:
                cout << "Scelta non valida.\n";
        }
    } while (scelta != 3);
}

/* ================= MAIN ================= */
int main() {
    if (sqlite3_open("spese.db", &db)) {
        cout << "Errore nell'apertura del database.\n";
        return 1;
    }

    int scelta;
    do {
        cout << "\n-------------------------\n";
        cout << " SISTEMA SPESE PERSONALI\n";
        cout << "-------------------------\n";
        cout << "1. Gestione Categorie\n";
        cout << "2. Inserisci Spesa\n";
        cout << "3. Definisci Budget Mensile\n";
        cout << "4. Visualizza Report\n";
        cout << "5. Esci\n";
        cout << "Scelta: ";
        cin >> scelta;

        switch (scelta) {
            case 1:
                gestioneCategorie();
                break;
            case 2:
                inserisciSpesa();
                break;
            case 3:
                definisciBudget();
                break;
            case 4:
                visualizzaReport();
                break;
            case 5:
                cout << "Uscita dal programma.\n";
                break;
            default:
                cout << "Scelta non valida. Riprovare.\n";
        }

    } while (scelta != 5);

    sqlite3_close(db);
    return 0;
}