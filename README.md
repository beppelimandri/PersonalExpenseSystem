# Personal Expense System

Sistema di gestione delle spese personali sviluppato in **C++**, basato su **interfaccia console** e **database SQLite**.

Il progetto consente a un singolo utente di registrare le proprie spese quotidiane, organizzarle per categoria, definire budget mensili e visualizzare report riepilogativi tramite interrogazioni SQL.

---

##  Funzionalità Principali

- Gestione delle categorie di spesa
- Inserimento delle spese giornaliere
- Definizione di budget mensili per categoria
- Visualizzazione di report riepilogativi:
  - Totale spese per categoria
  - Elenco completo delle spese ordinate per data
- Validazione degli input utente
- Persistenza dei dati tramite database relazionale SQLite

---

##  Tecnologie Utilizzate

- **Linguaggio:** C++
- **Database:** SQLite
- **Interfaccia:** Console testuale
- **Compilatore:** g++
- **Persistenza dati:** Database SQLite (`spese.db`)

---

##  Struttura del Repository

PersonalExpenseSystem/
│
├── main.cpp
│
├── database.sql
│
└── README.md

---

##  Database

Il database è composto dalle seguenti tabelle:

- **categorie**
- **spese**
- **budget**

Sono definiti esplicitamente i seguenti vincoli di integrità:
- `PRIMARY KEY`
- `FOREIGN KEY`
- `CHECK`
- `UNIQUE`
- `NOT NULL`

Lo script SQL completo è disponibile nel file:


sql/database.sql

---

##  Compilazione ed Esecuzione

### Requisiti
- Compilatore **g++**
- **SQLite**
- Sistema operativo Windows / Linux / macOS

---

##  Demo del Funzionamento

Il video dimostrativo del funzionamento del programma è disponibile al seguente link Google Drive:

https://drive.google.com/file/d/1_s6A5pPQnmLf65B57M0HotYmOY1-DLwz/view?usp=sharing
