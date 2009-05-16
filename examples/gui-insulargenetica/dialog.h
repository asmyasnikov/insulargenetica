#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QTimer>
#include <QtCore/QLibrary>
#include <../../idl/IFitness.h>

namespace Ui
{
    class DialogClass;
}

namespace InsularGenetica
{
    class CGeneticController;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void calc();
    void cancel();
    void setMaxResults();
    void updateTime();

private:
    Ui::DialogClass *ui;
    QTimer timer;
    bool calc_state;
    QList<InsularGenetica::IFitness*> m_fitness_modules;
    QList<QLibrary*> m_fitness_libraries;
    InsularGenetica::CGeneticController*calculator;
};

#endif // DIALOG_H
