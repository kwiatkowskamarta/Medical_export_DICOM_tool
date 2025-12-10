## Uruchamianie bazy danych PostgreSQL z danymi 


### ✅ Wymagania

* **Docker** (min. 20.10+)
* **docker-compose plugin**
  Sprawdź, czy masz:

  ```bash
  docker compose version
  ```

  Jeśli nie działa, zainstaluj:

  ```bash
  sudo apt install docker-compose-plugin
  ```

---

### 📁 Struktura katalogów

```
sim/
├── backend/
│   ├── dicom_to_DB.cpp      # kod importujący pliki DICOM
│   └── ...
├── docker/
│   └── sim_db.sql           # dump bazy danych w formacie .sql
├── docker-compose.yml       # konfiguracja Dockera
└── SIM1/                     # katalog z plikami DICOM (lokalnie)
```

---

### 🚀 Uruchamianie bazy danych z gotowymi danymi

1. **Sklonuj repozytorium:**

   ```bash
   git clone <adres_repozytorium>
   cd sim/
   ```

2. **Uruchom kontener - bazę danych z dumpem:**

   ```bash
   docker compose up --build -d
   ```

   🔸 Po pierwszym uruchomieniu, plik `docker/sim_db.sql` zostanie automatycznie załadowany do bazy.

---

### 🧪 Sprawdzenie działania

Po kilku sekundach baza powinna być gotowa.

Zaloguj się do niej:

```bash
docker exec -it sim-db psql -U Weronika -d Weronika
```

W terminalu:

```sql
\dt                          -- lista tabel
SELECT * FROM patients;      -- dane przykładowe
```
---

### 🌐 Dostęp po IP, bez stawiania kontenera

1. Potrzebne IP hosta, na którym działa kontener. Jeśli puścicie na swoim i chcecie sprawdzić z innego urządzenia,
to odczytujemy to jako:

   ```bash
   hostname -I
   ```

2. Korzystamy z portu `5433`.

3. Łączenie z innego urządzenia:

   ```bash
   psql -h <IP_Hosta> -p 5433 -U Weronika -d Weronika
   ```

Ja już zrobiłam tak na swoim, więc mogę wam podać mój IP żebyście się połączyły. Nie wiem tylko czy teraz nie lepiej 
żeby każda z nas miała swój kontener, aktualizowała go gdy coś doda i wrzucała do repo.
---


### 📌 Uwagi końcowe

* Login do bazy: `Weronika`
* Hasło: `haslomaslo123`
* Nazwa bazy: `Weronika`
* Port: `5433`


