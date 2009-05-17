#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QTimer>
#include <QtCore/QLibrary>
#include <../../idl/IFitness.h>
#include <../../idl/ICancelService.h>

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog,
               public InsularGenetica::ICancelService
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    bool isCanceled();

public slots:
    void calc(bool);
    void setMaxResults();
    void updateTime();

private:
    Ui::DialogClass *ui;
    QTimer timer;
    QList<InsularGenetica::IFitness*> m_fitness_modules;
    QList<QLibrary*> m_fitness_libraries;
};

#endif // DIALOG_H
