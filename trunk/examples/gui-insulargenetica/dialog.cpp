/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser GPL as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details.
You should have received a copy of the GNU Lesser GPL
in file LICENSE along with this program.  If not, see
<http://www.gnu.org/licenses/>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for № FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers / users,
no additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CDialog.h
 * @class   CDialog, CalcThread
 * @brief   Declaration of CDialog, CalcThread
 * @date    22/05/2009
 * @version 1.18
**/
#include "dialog.h"
#include "ui_dialog.h"
#include "../../include/export.h"
#include "../../include/CPopulation.h"
#include "../../include/CFitnessHelper.h"
#include "../../include/CChromosome.h"
#include "../../include/CGeneticController.h"
#include <QtCore/QDir>
#include <QtCore/QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogClass),
      calc_thread(NULL),
      timer(this)
{
    ui->setupUi(this);
    ui->dte_max_datetime->setDateTime(QDateTime::currentDateTime().addSecs(60));
    connect(&timer, SIGNAL(timeout()), SLOT(updateTime()));
    timer.start(1000);
    QDir dir("fitness");
    dir.setFilter(QDir::Files);
    QStringList filters;
    filters.append("*.so");
    filters.append("*.dll");
    dir.setNameFilters(filters);
    QStringList list = dir.entryList();
    for (int i = 0; i < list.size(); ++i)
    {
        QLibrary*lib = new QLibrary(dir.absoluteFilePath(list.at(i)));
        if(!lib->isLoaded() && !lib->load()) continue;
        typedef InsularGenetica::Interface*(*CreateModule)(void);
        CreateModule createModule = (CreateModule)(lib->resolve("CreateModule"));
        if(createModule)
        {
            InsularGenetica::Interface*iface = createModule();
            if(iface)
            {
                InsularGenetica::IFitness* module = dynamic_cast<InsularGenetica::IFitness*>(iface);
                if(module)
                {
                    m_fitness_modules.append(module);
                    m_fitness_libraries.append(lib);
                    continue;
                }else{
                    qDebug("createModule() return null");
                    delete iface;
                }
            }
        }else{
            qDebug("createModule() in %s not found", list.at(i).toLocal8Bit().data());
        }
        lib->unload();
        delete lib;
    }
    for(int i = 0; i < m_fitness_modules.size(); i++)
    {
        ui->cmb_fitness_functions->addItem(m_fitness_modules.at(i)->name());
    }
}

Dialog::~Dialog()
{
    if(calc_thread) delete calc_thread;
    delete ui;
}
void Dialog::calc(bool state)
{
    if(state)
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        ui->btn_exit->setEnabled(false);
        ui->btn_calc->setText(trUtf8("Cancel"));
        ui->tbl_results->setRowCount(0);
        ui->tbl_results->setColumnCount(0);
        ui->progress->setValue(0);
        if(calc_thread) delete calc_thread;
        calc_thread = new CalcThread(this);
        calc_thread->start();
    }
}
void Dialog::setMaxResults()
{
    ui->spin_box_results_count->setMaximum(ui->cpin_box_chromosome_size->value()*
                                           ui->cpin_box_population_size->value()*
                                           ui->cpin_box_islands_size->value());
}
void Dialog::updateTime()
{
    static QDateTime startdt;
    if(!ui->btn_calc->isChecked())
    {
        if(QDateTime::currentDateTime().secsTo(ui->dte_max_datetime->dateTime()) < 60)
        {
            ui->dte_max_datetime->setDateTime(QDateTime::currentDateTime().addSecs(60));
        }
        startdt = QDateTime::currentDateTime();
    }else{
        ui->progress->setValue(100-int(double(QDateTime::currentDateTime().secsTo(ui->dte_max_datetime->dateTime()))/
                                       double(startdt.secsTo(ui->dte_max_datetime->dateTime()))*
                                       100.));
    }
}
bool Dialog::isCanceled()
{
    return !ui->btn_calc->isChecked();
}
void Dialog::updateResults()
{
    calc_thread->updateResults();
    QApplication::restoreOverrideCursor();
}

CalcThread::CalcThread(Dialog*_d) : d(_d), f(NULL), best(NULL)
{
    connect(this, SIGNAL(finished()), d, SLOT(updateResults()));
}
CalcThread::~CalcThread()
{
    if(best) delete best;
}
void CalcThread::run()
{
    if(d->ui->cmb_fitness_functions->count())
    {
        f = d->m_fitness_modules.at(d->ui->cmb_fitness_functions->currentIndex());
        best = new InsularGenetica::CPopulation(
                   InsularGenetica::CGeneticController::calc(f,
                                                             d->ui->cpin_box_chromosome_size->value(),
                                                             d->ui->cpin_box_population_size->value(),
                                                             qMax(1,QDateTime::currentDateTime().secsTo(d->ui->dte_max_datetime->dateTime())/60),
                                                             d->ui->cpin_box_islands_size->value(),
                                                             d));
    }
}
void CalcThread::updateResults()
{
    InsularGenetica::CFitnessHelper *helper = dynamic_cast<InsularGenetica::CFitnessHelper*>(f);
    for(int i = 0; i < qMin(best->size(),d->ui->spin_box_results_count->value()); i++)
    {
        d->ui->tbl_results->setRowCount(d->ui->tbl_results->rowCount()+1);
        InsularGenetica::CChromosome chr(best->getChromosome(i));
        if(helper)
        {
            QList<double>phenotype = helper->genotype2phenotype(chr);
            d->ui->tbl_results->setColumnCount(qMax(d->ui->tbl_results->columnCount(),phenotype.size()));
            for(int j = 0; j < phenotype.size(); j++)
            {
                d->ui->tbl_results->setItem(d->ui->tbl_results->rowCount()-1, j,
                                            new QTableWidgetItem(QString::number(phenotype.at(j))));
            }
        }else{
            d->ui->tbl_results->setColumnCount(qMax(d->ui->tbl_results->columnCount(),1));
            char* code = new char[InsularGenetica::CChromosome::size()+1];
            for(unsigned int i = 0; i < InsularGenetica::CChromosome::size(); ++i)
            {
                code[i] = chr.getGene(i) ? '1' : '0';
            }
            code[InsularGenetica::CChromosome::size()] = '\0';
            d->ui->tbl_results->setItem(d->ui->tbl_results->rowCount()-1, 0,
                                        new QTableWidgetItem(QString(code)));
            delete[] code;
        }
    }
    QStringList headers;
    for(int i = 0; i < d->ui->tbl_results->columnCount(); i++)
    {
        headers.append(trUtf8("X%1").arg(i));
    }
    d->ui->tbl_results->setHorizontalHeaderLabels(headers);
    d->ui->progress->setValue(0);
    d->ui->btn_calc->setText(trUtf8("Evaluate"));
    d->ui->btn_calc->setChecked(false);
    d->ui->btn_exit->setEnabled(true);
}
