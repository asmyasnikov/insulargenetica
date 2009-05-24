#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QTimer>
#include <QtCore/QLibrary>
#include <QtCore/QThread>
#include <../../idl/IFitness.h>
#include <../../include/CPopulation.h>
#include <../../idl/ICancelService.h>

namespace Ui
{
    class DialogClass;
}

class CalcThread;

class Dialog : public QDialog,
               public InsularGenetica::ICancelService
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    bool isCanceled();

    friend class CalcThread;

public slots:
    void calc(bool);
    void setMaxResults();
    void updateTime();
    void updateResults();

private:
    Ui::DialogClass *ui;
    CalcThread*calc_thread;
    QTimer timer;
    QList<InsularGenetica::IFitness*> m_fitness_modules;
    QList<QLibrary*> m_fitness_libraries;
};

class CalcThread : public QThread
{
    Q_OBJECT
public :
    CalcThread(Dialog*_d);
    ~CalcThread();
    void updateResults();
private:
    void run();
private:
    Dialog*d;
    InsularGenetica::IFitness*f;
    InsularGenetica::CPopulation*best;
};

#endif // DIALOG_H
