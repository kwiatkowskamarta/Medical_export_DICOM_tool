#include <iostream>
#include <filesystem>
#include <sql.h>
#include <sqlext.h>
#include <dicomhero6/dicomhero.h>
#include <locale>
#include <codecvt>
#include <ctime>
#include <unordered_map>

using namespace std;
namespace fs = std::filesystem;
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

std::unordered_map<string, string> name_to_auto_id;
int auto_patient_counter = 1;

void extract_error(string fn, SQLHANDLE handle, SQLSMALLINT type) {
    SQLINTEGER i = 0, native;
    SQLCHAR state[7], text[256];
    SQLSMALLINT len;
    SQLRETURN ret;
    printf("\nThe driver reported the following diagnostics whilst running %s\n\n", fn.c_str());
    do {
        ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len);
        if (SQL_SUCCEEDED(ret))
            printf("%s:%ld:%ld:%s\n", state, (long)i, (long)native, text);
    } while (ret == SQL_SUCCESS);
}

void clearHandle(SQLHSTMT hstmt, SQLHDBC hdbc, SQLHENV henv) {
    if (hstmt != SQL_NULL_HSTMT) SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    if (hdbc != SQL_NULL_HDBC) {
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    }
    if (henv != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

int calculateAgeFromBirthYear(const string& birth_date_str) {
    if (birth_date_str.length() < 4) return -1;
    int birth_year = stoi(birth_date_str.substr(0, 4));
    
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int current_year = now->tm_year + 1900;
    
    int age = current_year - birth_year;
    if (age == current_year) age = -1;
    return age;
}

void insert_image_to_db(SQLHDBC hdbc, const string& series_uid, const dicomhero::DataSet& dcm) {
    int rows = -1, columns = -1, bits_allocated = -1;

    try {
        rows = dcm.getUint16(dicomhero::TagId(dicomhero::tagId_t::Rows_0028_0010), 0);
    } catch (...) {}
    try {
        columns = dcm.getUint16(dicomhero::TagId(dicomhero::tagId_t::Columns_0028_0011), 0);
    } catch (...) {}
    try {
        bits_allocated = dcm.getUint16(dicomhero::TagId(dicomhero::tagId_t::BitsAllocated_0028_0100), 0);
    } catch (...) {}
    int instance_number = -1;
    try {
        instance_number = dcm.getInt32(dicomhero::TagId(dicomhero::tagId_t::InstanceNumber_0020_0013), 0);
    } catch (...) {}

    string image_type = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::ImageType_0008_0008), 0));


    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    string sql = "INSERT INTO images (series_id, rows, columns, bits_allocated, instance_number, image_type) "
             "VALUES ((SELECT id FROM series WHERE series_uid = ?), ?, ?, ?, ?, ?);";

    SQLPrepare(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
    SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)series_uid.c_str(), 0, NULL);
    SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &rows, 0, NULL);
    SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &columns, 0, NULL);
    SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bits_allocated, 0, NULL);
    SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &instance_number, 0, NULL);
    SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)image_type.c_str(), 0, NULL);


    SQLExecute(hstmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

void insert_series_to_db(SQLHDBC hdbc, const string& study_uid, const dicomhero::DataSet& dcm) {
    string series_uid = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::SeriesInstanceUID_0020_000E), 0));
    string modality = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::Modality_0008_0060), 0));
    string description = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::SeriesDescription_0008_103E), 0));
    
    int series_number = -1;
    try {
        series_number = dcm.getInt32(dicomhero::TagId(dicomhero::tagId_t::SeriesNumber_0020_0011), 0);
    } catch (...) {}


    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    string check_query = "SELECT id FROM series WHERE series_uid = ?;";
    SQLPrepare(hstmt, (SQLCHAR*)check_query.c_str(), SQL_NTS);
    SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)series_uid.c_str(), 0, NULL);

    bool series_exists = false;
    if (SQLExecute(hstmt) == SQL_SUCCESS && SQLFetch(hstmt) == SQL_SUCCESS) {
        series_exists = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

    if (!series_exists) {
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        string sql = "INSERT INTO series (study_id, series_uid, modality, description, series_number) VALUES ((SELECT id FROM studies WHERE study_uid = ?), ?, ?, ?, ?);";
        SQLPrepare(hstmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
        SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)study_uid.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)series_uid.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)modality.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)description.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &series_number, 0, NULL);
        SQLExecute(hstmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
    insert_image_to_db(hdbc, series_uid, dcm);
}

void insert_study_to_db(SQLHDBC hdbc, const string& patient_id, const dicomhero::DataSet& dcm) {
    string study_uid = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::StudyInstanceUID_0020_000D), 0));
    string description = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::StudyDescription_0008_1030), 0));
    string referring_physician = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::ReferringPhysicianName_0008_0090), 0));
    string study_date_str = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::StudyDate_0008_0020), 0));
    string study_date = (study_date_str.length() >= 8) ? 
        (study_date_str.substr(0,4) + "-" + study_date_str.substr(4,2) + "-" + study_date_str.substr(6,2)) 
        : "YYYY-MM-DD";
        
    string accession_number = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::AccessionNumber_0008_0050), 0));
    string study_time_str = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::StudyTime_0008_0030), 0));
    string study_time = (study_time_str.length() >= 6) ? study_time_str.substr(0,2) + ":" + study_time_str.substr(2,2) + ":" + study_time_str.substr(4,2) : "";

    string institution_name = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::InstitutionName_0008_0080), 0));
    string institution_address = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::InstitutionAddress_0008_0081), 0));


    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    string check_query = "SELECT id FROM studies WHERE study_uid = ?;";
    SQLPrepare(hstmt, (SQLCHAR*)check_query.c_str(), SQL_NTS);
    SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)study_uid.c_str(), 0, NULL);

    bool study_exists = false;
    if (SQLExecute(hstmt) == SQL_SUCCESS && SQLFetch(hstmt) == SQL_SUCCESS) {
        study_exists = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

    if (!study_exists) {
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        string insert_query = "INSERT INTO studies (patient_id, study_uid, study_date, study_time, description, referring_physician, accession_number, institution_name, institution_address) "
                      "VALUES ((SELECT id FROM patients WHERE patient_id = ?), ?, ?, ?, ?, ?, ?, ?, ?);";

        SQLPrepare(hstmt, (SQLCHAR*)insert_query.c_str(), SQL_NTS);
        SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)patient_id.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)study_uid.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)study_date.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)study_time.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)description.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)referring_physician.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)accession_number.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)institution_name.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)institution_address.c_str(), 0, NULL);

        SQLExecute(hstmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }

    insert_series_to_db(hdbc, study_uid, dcm);
}


void insert_dicom_to_db(SQLHDBC hdbc, const string& filepath) {
    dicomhero::DataSet dcm(dicomhero::CodecFactory::load(filepath));
    
    string patient_id = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::PatientID_0010_0020), 0));
    string name = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::PatientName_0010_0010), 0));
    
        if (patient_id.empty()) {
    auto it = name_to_auto_id.find(name);
    if (it != name_to_auto_id.end()) {
        patient_id = it->second;
    } else {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "AUTO_%04d", auto_patient_counter++);
        patient_id = buffer;
        name_to_auto_id[name] = patient_id;
    }
}
    string sex = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::PatientSex_0010_0040), 0));

    int age = -1;
    try {
        dicomhero::Age age_dcm = dcm.getAge(dicomhero::TagId(dicomhero::tagId_t::PatientAge_0010_1010), 0);
        age = age_dcm.getYears();
    } catch (...) {
        string birth_date = converter.to_bytes(dcm.getUnicodeString(dicomhero::TagId(dicomhero::tagId_t::PatientBirthDate_0010_0030), 0));
        age = calculateAgeFromBirthYear(birth_date);
    }

    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    string check_query = "SELECT id FROM patients WHERE patient_id = ?;";
    SQLPrepare(hstmt, (SQLCHAR*)check_query.c_str(), SQL_NTS);
    SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)patient_id.c_str(), 0, NULL);

    bool patient_exists = false;
    if (SQLExecute(hstmt) == SQL_SUCCESS && SQLFetch(hstmt) == SQL_SUCCESS) {
        patient_exists = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

    if (!patient_exists) {
        SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        string insert_query = "INSERT INTO patients (patient_id, name, age, sex) VALUES (?, ?, ?, ?);";
        SQLPrepare(hstmt, (SQLCHAR*)insert_query.c_str(), SQL_NTS);
        SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)patient_id.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)name.c_str(), 0, NULL);
        SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &age, 0, NULL);
        SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, (SQLPOINTER)sex.c_str(), 0, NULL);
        SQLExecute(hstmt);
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }

    insert_study_to_db(hdbc, patient_id, dcm);
}

void load_dicom_directory_to_db(const string& dicom_dir_path) {
    SQLHENV henv = SQL_NULL_HENV;
    SQLHDBC hdbc = SQL_NULL_HDBC;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    SQLRETURN retcode;

    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (retcode != SQL_SUCCESS) return;
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
    if (retcode != SQL_SUCCESS) return;
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (retcode != SQL_SUCCESS) return;
    retcode = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
    if (retcode != SQL_SUCCESS) return;

    retcode = SQLConnect(hdbc, (SQLCHAR*)"SIM_DB", SQL_NTS, (SQLCHAR*)"Weronika", SQL_NTS, (SQLCHAR*)"haslomaslo123", SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        extract_error("SQLConnect", hdbc, SQL_HANDLE_DBC);
        clearHandle(hstmt, hdbc, henv);
        return;
    }

    for (const auto& entry : fs::recursive_directory_iterator(dicom_dir_path)) {
        if (entry.path().extension() == ".dcm") {
            insert_dicom_to_db(hdbc, entry.path().string());
        }
    }

    clearHandle(hstmt, hdbc, henv);
}

int main() {
    string path = "../SIM1";
    load_dicom_directory_to_db(path);
    return 0;
}

