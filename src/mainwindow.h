#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDate>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,
                        QString database_name = "",
                        QString user_name = "",
                        QString database_port = "",
                        QString user_password = "");
    ~MainWindow();

private slots:
    void exportXML(const QSqlDatabase &database, const QString &sqlQuery);
    void loadPage();
    void nextPage();
    void previousPage();
    void resetFilters();
    QString filterData();
    void initializeBaseRanges();
    static void closeProgram();

private:

    Ui::MainWindow *ui;
    QSqlQueryModel* model;
    QSqlDatabase db;

    QString database_name;
    QString user_name;
    QString user_password;
    QString database_port;

    QString filteredConnectionName;
    QString baseFilter = "";
    QString sqlQuery;

    QString baseAgeCondition;
    QString baseDateCondition;
    QString baseModalCondition;
    QString baseSexCondition;
    QString baseCombinedCondition;

    QString sexCondition;
    QString modalCondition;
    QString ageCondition;
    QString dateCondition;
    QString currentFilterCondition = baseFilter;

    int limit = 13;
    int offset = 0;
    int totalPatientRecords = 0;
    int totalStudyRecords = 0;
    int totalPages = 0;

    QDate baseSTART;
    QDate baseEND;

    int baseAgeMIN;
    int baseAgeMAX;

    void updatePageNumber();
    void updateAvailableFilters(const QSqlDatabase &db);
    void appendToStatusWindow(const QString& text);

};
#endif // MAINWINDOW_H
