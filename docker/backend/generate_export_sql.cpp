#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string start_date, end_date, modality, sex;
    int min_age, max_age;

    // Pobieranie danych od użytkownika
    cout << "Podaj datę początkową (YYYY-MM-DD): ";
    cin >> start_date;
    cout << "Podaj datę końcową (YYYY-MM-DD): ";
    cin >> end_date;
    cout << "Podaj modalność (np. NM, CT): ";
    cin >> modality;
    cout << "Podaj płeć (M/F/O): ";
    cin >> sex;
    cout << "Podaj minimalny wiek pacjenta: ";
    cin >> min_age;
    cout << "Podaj maksymalny wiek pacjenta: ";
    cin >> max_age;

    // Tworzenie zapytania SQL z filtrami
    ofstream sqlFile("filtered_export.sql");
    if (!sqlFile.is_open()) {
        cerr << "Nie można utworzyć pliku SQL!" << endl;
        return 1;
    }

    sqlFile << R"(\o export.xml
SELECT xmlelement(name patients,
  xmlagg(
    xmlelement(name patient,
      xmlattributes(p.id as id, p.patient_id as patient_id),
      xmlforest(p.age as age, p.sex as sex, p.birth_date as birth_date),
      (
        SELECT xmlagg(
          xmlelement(name study,
            xmlattributes(s.study_uid as study_uid),
            xmlforest(s.study_date as study_date, s.study_type as study_type),
            (
              SELECT xmlagg(
                xmlelement(name series,
                  xmlattributes(se.series_uid as series_uid),
                  xmlforest(se.modality as modality, se.description as description),
                  (
                    SELECT xmlagg(
                      xmlelement(name image,
                        xmlforest(i.filename, i.filepath, i.file_size)
                      )
                    )
                    FROM images i
                    WHERE i.series_id = se.id
                  )
                )
              )
              FROM series se
              WHERE se.study_id = s.id
            )
          )
        )
        FROM studies s
        WHERE s.patient_id = p.id
      )
    )
  )
)

FROM patients p
WHERE p.sex = ')" << sex << R"('
AND p.age BETWEEN )" << min_age << " AND " << max_age << R"(;
\o
)";
    sqlFile.close();

    cout << "Zapisano zapytanie SQL do pliku: filtered_export.sql" << endl;

    // (Opcjonalnie) wykonanie zapytania w psql
    cout << "Czy chcesz wykonać to zapytanie za pomocą psql? (t/n): ";
    char choice;
    cin >> choice;

    if (choice == 't' || choice == 'T') {
        system("psql -d sim_db -U weronika -f filtered_export.sql");
    }

    return 0;
}
