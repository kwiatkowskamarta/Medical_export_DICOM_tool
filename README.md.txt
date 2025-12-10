# System Eksportu i Walidacji Danych Medycznych

## Opis Projektu

Projekt STAT to aplikacja desktopowa stworzona w ramach przedmiotu *Systemy Informatyczne w Medycynie*. Służy do zarządzania bazą danych obrazów medycznych (zgodną z hierarchią DICOM), filtrowania pacjentów oraz eksportu danych do formatu XML wraz z automatyczną walidacją.

### Główne funkcje:
* Interfejs GUI (Qt): Przeglądanie i filtrowanie bazy pacjentów (wg wieku, płci, daty badania, modalności).
* Eksport XML: Generowanie raportów zgodnych ze schematem XSD.
* Walidacja: Dedykowane narzędzie CLI (`xml_validator`) weryfikujące poprawność struktury plików wyjściowych przy użyciu `libxml2`.
* Baza Danych: Konteneryzowana baza PostgreSQL ze strukturą *Patient -> Study -> Series -> Image*.

## Struktura Projektu

* `src/` - Kod źródłowy aplikacji głównej (C++/Qt).
* `tools/` - Kod narzędzia do walidacji XML oraz schemat `export.xsd`.
* `database/` - Skrypty SQL oraz pliki inicjalizacyjne bazy.
* `docker/` - Konfiguracja Docker Compose.

## Jak uruchomić?

### Wymagania
* CMake >= 3.5
* Kompilator C++ (GCC/Clang/MSVC)
* Qt5 lub Qt6 (moduły: Widgets, Sql)
* LibXml2 (do kompilacji walidatora)
* Docker (opcjonalnie, do bazy danych)

### Krok 1: Uruchomienie Bazy Danych
Zalecamy użycie Dockera do postawienia bazy PostgreSQL:

```bash
cd docker
docker-compose up -d