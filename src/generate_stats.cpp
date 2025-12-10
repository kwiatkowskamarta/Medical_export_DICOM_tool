#include "generate_stats.h"

//sprawdzenie czy wystapil jakis blad sql
void checkSqlError(const QSqlQuery& query, const QString& context) {
    if (query.lastError().isValid()) {
        qCritical() << "Błąd SQL w " << context << ":" << query.lastError().text();
        // Nie wychodzimy z programu, tylko informujemy i zwracamy
        // exit(EXIT_FAILURE);
    }
}

//compute stats przyjmuje polaczenie do bazy danych db i warunek filtrowania (where condition)
Stats computeStats(const QSqlDatabase& db, const QString& whereCondition) {
    if (!db.isOpen()) { //sprawdzenie czy baza jest otwarta
        qCritical() << "Połączenie z bazą danych jest zamknięte";
        return Stats{}; // Zwróć pustą strukturę, nie exit
    }

    //left join pozwala policzyc pacjenta nawet jesli nie ma badan
    //coalesce- jak nie ma danych o rozmiarze, to wstaw 0
    //count liczy ilosc
    //distinct liczy unikalnych pacjentow
    QString statsSQL = QString(R"(
        SELECT
            COUNT(DISTINCT p.id) AS patient_count,
            COUNT(DISTINCT s.id) AS study_count,
            COUNT(DISTINCT se.id) AS series_count,
            COUNT(im.id) AS image_count,
            COALESCE(SUM(im.file_size_bytes), 0) AS total_size_bytes
        FROM patients p
        LEFT JOIN studies s ON s.patient_id = p.id 
        LEFT JOIN series se ON se.study_id = s.id
        LEFT JOIN images im ON im.series_id = se.id
        WHERE %1
    )").arg(whereCondition);

    //wysylanie zapytania do bazy
    QSqlQuery query(db);
    if (!query.exec(statsSQL)) {
        checkSqlError(query, "computeStats");
        return Stats{};
    }

    Stats s = {};
    if (query.next()) { //pobiera pierwszy wiersz wynikow
        s.patientCount = query.value("patient_count").toInt();
        s.studyCount = query.value("study_count").toInt();
        s.seriesCount = query.value("series_count").toInt();
        s.imageCount = query.value("image_count").toInt();
        s.totalSizeBytes = query.value("total_size_bytes").toLongLong();
    } else {
        qCritical() << "Brak danych zwróconych przez zapytanie statystyczne.";
    }

    return s; //zwracamy strukture z wynikami
}

QString exeFuncSTATS(const QString& whereCondition, const QString& nazwaBazy, const QString& uzytkownik, const QString& haslo, const QString& port ) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "stats_connection"); //tworzenie polaczenia z baza
    //ustawienie danych do logowania
    db.setHostName("localhost");
    db.setPort(port.toInt());
    db.setDatabaseName(nazwaBazy);
    db.setUserName(uzytkownik);
    db.setPassword(haslo);

    //prosba polaczenia z baza
    if (!db.open()) {
        return QString("Błąd połączenia z bazą danych: %1").arg(db.lastError().text());
    }

    //liczenie statystyk za pomoca wczesniej opisanej funkcji
    Stats s = computeStats(db, whereCondition);

    //zamkniecie polczenia z baza
    db.close();
    QSqlDatabase::removeDatabase("stats_connection");

    // Formatowanie tekstowe statystyk:
    QString result;
    result += "Statystyki:\n";
    result += QString("Liczba pacjentów: %1\n").arg(s.patientCount);
    result += QString("Liczba badań: %1\n").arg(s.studyCount);
    result += QString("Liczba serii: %1\n").arg(s.seriesCount);
    result += QString("Liczba obrazów: %1\n").arg(s.imageCount);
    result += QString("Łączny rozmiar (bajty): %1\n").arg(s.totalSizeBytes);

    return result;
}

