#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextStream>

// Konstruktor klasy - okno aplikacji
MainWindow::MainWindow(QWidget *parent,
                       QString database_name,
                       QString user_name,
                       QString database_port,
                       QString user_password)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    database_name(database_name),
    user_name(user_name),
    database_port(database_port),
    user_password(user_password)
{
    // Uruchomienie interfejsu i informacja zwrotna
    ui->setupUi(this);
    appendToStatusWindow("Uruchomiono program");

    // Konstruowanie wewnątrzprogramowej bazy danych
    db = QSqlDatabase::addDatabase("QPSQL", "main_connection");
    db.setHostName("localhost");
    db.setPort(database_port.toInt());
    db.setDatabaseName(database_name);
    db.setUserName(user_name);
    db.setPassword(user_password);

    qDebug() << "Database Name:" << this->database_name;
    qDebug() << "User Name:" << this->user_name;
    qDebug() << "Database Port:" << this->database_port;
    qDebug() << "User Password:" << this->user_password;

    // Stan początkowy przycisków funkcyjnych
    ui->ResetButton->setEnabled(false);
    ui->prevButton->setEnabled(false);
    ui->ExportToXML->setEnabled(false);

    // Weryfikacja połączenia
    if (!db.open()) {
        QMessageBox::critical(this, "Błąd", "Nie można połączyć się z bazą: " + db.lastError().text());
        return;
    }
    else {
        appendToStatusWindow("Polaczono z baza danych");
        qDebug() << "Polaczaono z baza danych: " << db.databaseName();
    }

    // Wyznaczanie wartości początkowych wyszukiwania
    initializeBaseRanges();

    // Ustawienie wartości początkowych wyszukiwania
    ui->dataStart->setDate(baseSTART);
    ui->dataEnd->setDate(baseEND);

    ui->wiekMIN->setValue(baseAgeMIN);
    ui->wiekMAX->setValue(baseAgeMAX);

    // Tworzenie modelu wyświetlania
    model = new QSqlQueryModel(this);

    // Dostosowanie funkcjonalnosci do zawartosci bazy danych (płeć oraz modalności)
    updateAvailableFilters(db);

    // Wyświetlenie aktualnie wybranych danych
    loadPage();

    // Przyciski zmiany wyświetlanych danych
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::nextPage);
    connect(ui->prevButton, &QPushButton::clicked, this, &MainWindow::previousPage);

    // Działanie przycisku filtracji
    connect(ui->filterButton, &QPushButton::clicked, this, [this]() {
        offset = 0;

        // filtracja bazy danych
        currentFilterCondition = filterData();
        loadPage();
        ui->ResetButton->setEnabled(true);
        ui->ExportToXML->setEnabled(true);

    });

    // Działanie przycisku resetu filtracji
    connect(ui->ResetButton, &QPushButton::clicked, this, [this]() {
        offset = 0;

        const QString connName = "filtered_connection";
        {
            QString tmpConnName = connName + "_removal";
            {
                QSqlDatabase dbRef = QSqlDatabase::database(connName, false);
                if (dbRef.isValid()) {
                    dbRef = QSqlDatabase();
                }
            }
            QSqlDatabase::removeDatabase(connName);
        }

        // resetowanie zalorzen filtracji i aktualizacja wyświetlania
        currentFilterCondition = baseFilter;
        resetFilters();
        loadPage();
        ui->PacjenciNr->display(totalPatientRecords);
        ui->ResetButton->setEnabled(false);
    });

    // Działanie przycisku do procesowania eksportu
    connect(ui->ExportToXML, &QPushButton::clicked, this, [this]() {

        // eksport bazy danych po filtracji
        exportXML(db, currentFilterCondition);
    });

    // Działanie przycisku do zamykania programu
    connect(ui->ExitButton, &QPushButton::clicked, this, &MainWindow::closeProgram);
}

// Wyznaczenie maksimow i minimow daty i wieku
void MainWindow::initializeBaseRanges() {

    // kontrola połączenia z bazą
    if (!db.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta.";
        return;
    }

    // wyznaczanie ograniczeń wieku
    QSqlQuery ageQuery(db);
    if (ageQuery.exec("SELECT MIN(age), MAX(age) FROM patients")) {
        if (ageQuery.next()) {
            baseAgeMIN = ageQuery.value(0).toInt();
            baseAgeMAX = ageQuery.value(1).toInt();
        } else {
            qDebug() << "Brak danych o wieku.";
            return;
        }
    } else {
        qDebug() << "Błąd zapytania o wiek:" << ageQuery.lastError().text();
        return;
    }

    // wyznaczanie ograniczeń daty badań
    QSqlQuery dateQuery(db);
    if (dateQuery.exec("SELECT MIN(study_date), MAX(study_date) FROM studies")) {
        if (dateQuery.next()) {
            baseSTART = dateQuery.value(0).toDate();
            baseEND = dateQuery.value(1).toDate();
        } else {
            qDebug() << "Brak danych o datach.";
            return;
        }
    } else {
        qDebug() << "Błąd zapytania o daty:" << dateQuery.lastError().text();
        return;
    }

    // przypisanie do odpowiednich zmiennych
    baseAgeCondition = QString("p.age BETWEEN %1 AND %2").arg(baseAgeMIN).arg(baseAgeMAX);
    baseDateCondition = QString("s.study_date BETWEEN '%1' AND '%2'").arg(baseSTART.toString("yyyy-MM-dd")).arg(baseEND.toString("yyyy-MM-dd"));
}

// Funkcja - aktualizacja dostępnych filtrów (blokada filtracji po nie występujących wartościach)
void MainWindow::updateAvailableFilters(const QSqlDatabase &db)
{
    // kontrola połączenia z bazą
    if (!db.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta.";
        return;
    }

    QSqlQuery query(db);

    // walidacja dostępnych płci
    struct SexCheckbox {
        QString sexTag;
        QCheckBox* checkbox;
    };

    QVector<SexCheckbox> sexCheckboxes = {
        {"F", ui->plecK},
        {"M", ui->plecM},
        {"O", ui->plecO}
    };

    QStringList selectedSexes;

    for (const SexCheckbox& sc : sexCheckboxes) {
        query.prepare("SELECT EXISTS (SELECT 1 FROM patients WHERE sex = :sex LIMIT 1)");
        query.bindValue(":sex", sc.sexTag);
        if (query.exec()) {
            if (query.next() && query.value(0).toBool()) {
                sc.checkbox->setEnabled(true);
                sc.checkbox->setChecked(true);
                selectedSexes.append("'" + sc.sexTag + "'");
            } else {
                sc.checkbox->setEnabled(false);
                sc.checkbox->setChecked(false);
            }
        } else {
            qDebug() << "Błąd zapytania o płeć:" << query.lastError().text();
        }
    }

    if (!selectedSexes.isEmpty()) {
        baseSexCondition = "p.sex IN (" + selectedSexes.join(", ") + ")";
    } else {
        appendToStatusWindow("Nie znaleziono wystąpień żadnej z dostępnych płci.");
        return;
    }

    // walidacja dostępnych modalności
    struct ModalCheckbox {
        QString modalTag;
        QCheckBox* checkbox;
    };

    QVector<ModalCheckbox> modalCheckboxes = {
        {"NM", ui->modaNM},
        {"CT", ui->modaCT},
        {"MRI", ui->modaMRI},
        {"RTG", ui->modaRTG},
        {"USG", ui->modaUSG}
    };

    QStringList selectedModalities;

    for (const ModalCheckbox& mc : modalCheckboxes) {
        query.prepare("SELECT EXISTS (SELECT 1 FROM series WHERE modality = :modal LIMIT 1)");
        query.bindValue(":modal", mc.modalTag);
        if (query.exec()) {
            if (query.next() && query.value(0).toBool()) {
                mc.checkbox->setEnabled(true);
                mc.checkbox->setChecked(true);
                selectedModalities.append("'" + mc.modalTag + "'");
            } else {
                mc.checkbox->setEnabled(false);
                mc.checkbox->setChecked(false);
            }
        } else {
            qDebug() << "Błąd zapytania o modalność:" << query.lastError().text();
        }
    }

    if (!selectedModalities.isEmpty()) {
        baseModalCondition = "se.modality IN (" + selectedModalities.join(", ") + ")";
    } else {
        appendToStatusWindow("Nie znaleziono wystąpień żadnej z dostępnych modalności.");
        return;
    }

    // tworzenie zapytania bazowego filtracji
    QStringList conditions = {baseSexCondition, baseModalCondition, baseAgeCondition, baseDateCondition};
    baseCombinedCondition = conditions.join(" AND ");
}

// Funkcja - eksport do XML
void MainWindow::exportXML(const QSqlDatabase &database, const QString &currentFilterCondition) {

    // kontrola połączenia z bazą
    if (!database.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta!";
        appendToStatusWindow("Baza danych nie jest otwarta!");
        return;
    }

    // podpięcie pliku z indtrukcją SQL
    QString sqlFilePath = "export_xml_query.sql";
    QFile sqlFile(sqlFilePath);
    if (!sqlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku SQL:" << sqlFilePath;
        appendToStatusWindow("Nie można otworzyć pliku SQL z zapytaniem.");
        return;
    }

    QString sqlQuery = sqlFile.readAll();
    sqlFile.close();

    if (sqlQuery.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Eksport", "Zapytanie SQL jest puste.");
        return;
    }

    // tworzenie odpowiednich zapytań na podstawie założeń filtracji
    if (currentFilterCondition == baseFilter || currentFilterCondition.isEmpty()) {
        QString filterPatients = " AND " + baseSexCondition + " AND " + baseAgeCondition;
        QString filterStudies = " AND " + baseDateCondition;
        QString filterSeries = " AND " + baseModalCondition;

        // zastąpienie znaczników w SQL
        sqlQuery.replace("/* FILTER_PATIENTS */", filterPatients);
        sqlQuery.replace("/* FILTER_STUDIES */", filterStudies);
        sqlQuery.replace("/* FILTER_SERIES */", filterSeries);
    } else {
        QString filterPatients = " AND " + sexCondition + " AND " + ageCondition;
        QString filterStudies = " AND " + dateCondition;
        QString filterSeries = " AND " + modalCondition;

        // zastąpienie znaczników w SQL
        sqlQuery.replace("/* FILTER_PATIENTS */", filterPatients);
        sqlQuery.replace("/* FILTER_STUDIES */", filterStudies);
        sqlQuery.replace("/* FILTER_SERIES */", filterSeries);
    }

    // wykonanie zapytania
    QSqlQuery query(database);
    if (!query.exec(sqlQuery)) {
        qDebug() << "Błąd zapytania:" << query.lastError().text();
        QMessageBox::critical(this, "Eksport XML", "Nie udało się wykonać zapytania XML:\n" + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString xmlResult = query.value(0).toString();

        if (xmlResult.trimmed().isEmpty()) {
            appendToStatusWindow("Brak danych do eksportu (pusty wynik).");
            return;
        }

        // walidacja nazwy pliku wprowadzonej przez użytkownika
        QString filename = ui->nameEdit->text().trimmed();
        QRegularExpression validName("^[A-Za-z0-9_-]+$");

        if (filename.isEmpty()) {
            appendToStatusWindow("Przed eksportem podaj nazwę");
            return;
        } else if (!validName.match(filename).hasMatch()) {
            appendToStatusWindow("Nazwa może zawierać tylko litery, cyfry, _ i -");
            return;
        }
        if (filename.length() > 50) {
            appendToStatusWindow("Nazwa pliku jest zbyt długa. Maksymalna długość to 50 znaków.");
            return;
        }

        // Zapis do pliku XML o podanej nazwie
        QString filenameXML = "exports/" + filename + ".xml";
        QFile outFile(filenameXML);

        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Nie można zapisać pliku .xml";
            appendToStatusWindow("Nie można zapisać pliku XML.");
            return;
        }

        QTextStream out(&outFile);
        out << xmlResult;
        outFile.close();

        // informacja zwrotna i wyswietlenie w konsoli pełnej nazwy pliku XML
        QString outMessage = "Wygenerowano plik XML: " + filenameXML;
        qDebug() << filenameXML;
        appendToStatusWindow(outMessage);
        ui->nameEdit->clear();
    }
}

// Funkcja - aktualizacja numeru strony wyswietlania
void MainWindow::updatePageNumber()
{
    int currentPage = (offset / limit) + 1;
    ui->PageNumber->setText(QString("%1 / %2").arg(currentPage).arg(totalPages));
}

// Funkcja - wyswietlanie wybranych rekordów w tabeli
void MainWindow::loadPage() {
    QSqlDatabase database = db;

    // kontrola połączenia z bazą
    if (!database.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta!";
        return;
    }

    // sprawdzanie warunków filtracji
    if (currentFilterCondition.isEmpty()) {
        currentFilterCondition = baseCombinedCondition;
    }
    // Jeśli nadal jest pusto (bo np. baza nie odpowiedziała poprawnie na filtry),
    // ustawiamy warunek "1=1" (zawsze prawda), żeby zapytanie SQL było poprawne.
    if (currentFilterCondition.trimmed().isEmpty()) {
        currentFilterCondition = "1=1";
    }

    // liczenie liczby pacjentów
    QSqlQuery countPatientsQuery(database);
    QString countPatientsSQL = QString(R"(
        SELECT COUNT(DISTINCT p.id) AS patient_count
        FROM patients p
        JOIN studies s ON s.patient_id = p.id
        JOIN series se ON se.study_id = s.id
        WHERE %1
    )").arg(currentFilterCondition);

    if (countPatientsQuery.exec(countPatientsSQL)) {
        if (countPatientsQuery.next()) {
            totalPatientRecords = countPatientsQuery.value("patient_count").toInt();
        }
    } else {
        qDebug() << "Błąd zliczania pacjentów:" << countPatientsQuery.lastError().text();
        return;
    }

    // liczenie liczby badań
    QSqlQuery countStudiesQuery(database);
    QString countStudiesSQL = QString(R"(
        SELECT COUNT(DISTINCT s.id) AS study_count
        FROM patients p
        JOIN studies s ON s.patient_id = p.id
        JOIN series se ON se.study_id = s.id
        WHERE %1
    )").arg(currentFilterCondition);

    if (countStudiesQuery.exec(countStudiesSQL)) {
        if (countStudiesQuery.next()) {
            totalStudyRecords = countStudiesQuery.value("study_count").toInt();
        }
    } else {
        qDebug() << "Błąd zliczania badań:" << countStudiesQuery.lastError().text();
        return;
    }

    // obliczanie liczby stron
    totalPages = (totalPatientRecords + limit - 1) / limit;

    // przygotowanie zapytania SQL do wyświetlania danych
    QString queryStr = QString(R"(
        SELECT DISTINCT p.id, p.patient_id,
            split_part(p.name, '^', 1) AS lastname,
            split_part(p.name, '^', 2) AS firstname,
            p.age, p.sex
        FROM patients p
        JOIN studies s ON s.patient_id = p.id
        JOIN series se ON se.study_id = s.id
        WHERE %1
        ORDER BY p.id
        LIMIT %2 OFFSET %3
    )").arg(currentFilterCondition)
                           .arg(limit)
                           .arg(offset);

    // ustawienie modelu danych
    model->setQuery(queryStr, database);
    if (model->lastError().isValid()) {
        qDebug() << "Błąd modelu danych:" << model->lastError().text();
        return;
    }

    // sprawdzenie, czy tabela ma dane
    if (model->rowCount() == 0) {
        appendToStatusWindow("Brak danych do wyświetlenia dla podanych filtrów.");
    }

    // ustawienie nagłówków i layoutu tabeli
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ID pacjenta");
    model->setHeaderData(2, Qt::Horizontal, "Nazwisko");
    model->setHeaderData(3, Qt::Horizontal, "Imię");
    model->setHeaderData(4, Qt::Horizontal, "Wiek");
    model->setHeaderData(5, Qt::Horizontal, "Płeć");

    ui->DB_table->setModel(model);
    ui->DB_table->setColumnWidth(0, 50);
    ui->DB_table->setColumnWidth(1, 150);
    ui->DB_table->setColumnWidth(2, 165);
    ui->DB_table->setColumnWidth(3, 150);
    ui->DB_table->setColumnWidth(4, 50);
    ui->DB_table->setColumnWidth(5, 50);
    ui->DB_table->verticalScrollBar()->setValue(0);

    // wyświetlenie liczby pacjentów i badań
    ui->PacjenciNr->display(totalPatientRecords);
    ui->BadaniaNr->display(totalStudyRecords);

    // ustawienia przycisków nawigacji
    ui->nextButton->setEnabled(offset + limit < totalPatientRecords);
    ui->prevButton->setEnabled(offset > 0);

    // aktualizacja numeru strony
    updatePageNumber();
}

// Funkcja - następna strona
void MainWindow::nextPage()
{
    ui->nextButton->setEnabled(false);

    if (offset + limit < totalPatientRecords) {
        offset += limit;
        loadPage();
        ui->nextButton->setEnabled(true);
    }

    ui->prevButton->setEnabled(true);
}

// Funkcja - poprzednia strona
void MainWindow::previousPage()
{
    offset = (offset - limit) < 0 ? 0 : (offset - limit);
    loadPage();
    if (offset == 0) {
        ui->prevButton->setEnabled(false);
    }
    ui->nextButton->setEnabled(true);
}

// Funkcja - filtracja danych
QString MainWindow::filterData() {

    // pobranie wartości z UI
    QString start_date = ui->dataStart->date().toString("yyyy-MM-dd");
    QString end_date = ui->dataEnd->date().toString("yyyy-MM-dd");

    int min_age = ui->wiekMIN->value();
    int max_age = ui->wiekMAX->value();

    // filtr płci
    QStringList selectedSexes;
    if (ui->plecK->isChecked()) selectedSexes << "'F'";
    if (ui->plecM->isChecked()) selectedSexes << "'M'";
    if (ui->plecO->isChecked()) selectedSexes << "'O'";


    if (!selectedSexes.isEmpty()) {
        sexCondition = "p.sex IN (" + selectedSexes.join(", ") + ")";
    } else {
        appendToStatusWindow("Brak wybranej płci. Proszę wybrać co najmniej jedną opcję.");
        return QString();
    }

    // filtr modalności
    QStringList selectedModal;
    if (ui->modaNM->isChecked()) selectedModal << "'NM'";
    if (ui->modaCT->isChecked()) selectedModal << "'CT'";
    if (ui->modaMRI->isChecked()) selectedModal << "'MRI'";
    if (ui->modaRTG->isChecked()) selectedModal << "'RTG'";
    if (ui->modaUSG->isChecked()) selectedModal << "'USG'";


    if (!selectedModal.isEmpty()) {
        modalCondition = "se.modality IN (" + selectedModal.join(", ") + ")";
    } else {
        appendToStatusWindow("Brak wybranej modalności. Proszę wybrać co najmniej jedną opcję.");
        return QString();
    }

    // filtr wieku
    ageCondition = QString("p.age BETWEEN %1 AND %2").arg(min_age).arg(max_age);

    // filtr daty
    dateCondition = QString("s.study_date BETWEEN '%1' AND '%2'").arg(start_date).arg(end_date);

    // tworzenie pełnego zapytania warunkowego w SQL
    QStringList conditions = {sexCondition, modalCondition, ageCondition, dateCondition};
    QString combinedCondition = conditions.join(" AND ");

    // informacja zwrotna
    appendToStatusWindow("Filtracja zakończona sukcesem");
    ui->ResetButton->setEnabled(true);

    return combinedCondition;
}

// Funkcja - resetowanie filtracji
void MainWindow::resetFilters(){

    // ustawienie wszystkich filtrów na domyślne
    ui->dataStart->setDate(baseSTART);
    ui->dataEnd->setDate(baseEND);

    ui->wiekMIN->setValue(baseAgeMIN);
    ui->wiekMAX->setValue(baseAgeMAX);

    if (ui->plecK->isEnabled()) ui->plecK->setChecked(true);
    if (ui->plecM->isEnabled()) ui->plecM->setChecked(true);
    if (ui->plecO->isEnabled()) ui->plecO->setChecked(true);

    if (ui->modaNM->isEnabled()) ui->modaNM->setChecked(true);
    if (ui->modaCT->isEnabled())  ui->modaCT->setChecked(true);
    if (ui->modaMRI->isEnabled()) ui->modaMRI->setChecked(true);
    if (ui->modaRTG->isEnabled()) ui->modaRTG->setChecked(true);
    if (ui->modaUSG->isEnabled()) ui->modaUSG->setChecked(true);

    currentFilterCondition = baseFilter;

    loadPage();
    appendToStatusWindow("Zresetowano warunki filtrowania");
    ui->ResetButton->setEnabled(false);
}

// Funkcja - obsługa wyświetlania w oknie statusu
void MainWindow::appendToStatusWindow(const QString& text) {
    ui->statusWindow->append(text);

    // automatyczne przewijanie po nowym komunikacie
    QTextCursor cursor = ui->statusWindow->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->statusWindow->setTextCursor(cursor);
}

// Funkcja - zamykanie programu
void MainWindow::closeProgram(){
    QApplication::quit();
}

// Dekonstruktor klasy okno aplikacji
MainWindow::~MainWindow()
{
    if(db.isOpen())db.close();
    delete model;
    delete ui;
}
