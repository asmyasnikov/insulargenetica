#include "dialog.h"
#include "ui_dialog.h"
#include "../../include/export.h"
#include "../../include/CPopulation.h"
#include "../../include/CFitnessHelper.h"
#include "../../include/CChromosome.h"
#include "../../include/CGeneticController.h"
#include <QtCore/QDir>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogClass),
      timer(this),
      calculator(NULL)
{
    ui->setupUi(this);
    ui->dte_max_datetime->setDateTime(QDateTime::currentDateTime().addSecs(60));
    connect(&timer, SIGNAL(timeout()), SLOT(updateTime()));
    timer.start(1000);
    calc_state = false;
    ui->btn_cancel->hide();
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
    delete ui;
}
void Dialog::calc()
{
    calc_state = true;
    ui->btn_cancel->show();
    ui->btn_calc->hide();
    ui->tbl_results->setRowCount(0);
    ui->tbl_results->setColumnCount(0);
    ui->progress->setValue(0);
    if(ui->cmb_fitness_functions->count())
    {
        InsularGenetica::IFitness*f = m_fitness_modules.at(ui->cmb_fitness_functions->currentIndex());
        InsularGenetica::CGeneticController*calculator =
                InsularGenetica::CGeneticController::getCalculator(f,
                                                                   ui->cpin_box_chromosome_size->value(),
                                                                   ui->cpin_box_population_size->value(),
                                                                   qMax(1,QDateTime::currentDateTime().secsTo(ui->dte_max_datetime->dateTime())/60),
                                                                   ui->cpin_box_islands_size->value());
        InsularGenetica::CPopulation best = 
                calculator->getBestSolutions(ui->spin_box_results_count->value());
        delete calculator;
        calculator = NULL;
        InsularGenetica::CFitnessHelper *helper = dynamic_cast<InsularGenetica::CFitnessHelper*>(f);
        for(int i = 0; i < best.size(); i++)
        {
            ui->tbl_results->setRowCount(ui->tbl_results->rowCount()+1);
            InsularGenetica::CChromosome chr = best.getChromosome(i);
            if(helper)
            {
                QList<double>phenotype = helper->genotype2phenotype(chr);
                ui->tbl_results->setColumnCount(qMax(ui->tbl_results->columnCount(),phenotype.size()));
                for(int j = 0; j < phenotype.size(); j++)
                {
                    ui->tbl_results->setItem(ui->tbl_results->rowCount()-1, j,
                                             new QTableWidgetItem(QString::number(phenotype.at(j))));
                }
            }else{
                ui->tbl_results->setColumnCount(qMax(ui->tbl_results->columnCount(),1));
                char* code = new char[InsularGenetica::CChromosome::size()+1];
                for(unsigned int i = 0; i < InsularGenetica::CChromosome::size(); ++i)
                {
                    code[i] = chr.getGene(i) ? '1' : '0';
                }
                code[InsularGenetica::CChromosome::size()] = '\0';
                ui->tbl_results->setItem(ui->tbl_results->rowCount()-1, 0,
                                         new QTableWidgetItem(QString(code)));
                delete[] code;
            }
            if(!i)
            {
                ui->tbl_results->setColumnCount(ui->tbl_results->columnCount()+1);
            }
            ui->tbl_results->setItem(ui->tbl_results->rowCount()-1, ui->tbl_results->columnCount()-1,
                                     new QTableWidgetItem(QString::number(chr.fitness())));
        }
        QStringList headers;
        for(int i = 0; i < ui->tbl_results->columnCount()-1; i++)
        {
            headers.append(trUtf8("X%1").arg(i));
        }
        headers.append(trUtf8("Fitness"));
        ui->tbl_results->setHorizontalHeaderLabels(headers);
    }
    ui->progress->setValue(0);
    calc_state = false;
    ui->btn_cancel->hide();
    ui->btn_calc->show();
}
void Dialog::cancel()
{
    calc_state = false;
    ui->btn_cancel->hide();
    ui->btn_calc->show();
    if(calculator)
    {
        calculator->cancel();
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
    if(!calc_state)
    {
        if(QDateTime::currentDateTime().secsTo(ui->dte_max_datetime->dateTime()) < 60)
        {
            ui->dte_max_datetime->setDateTime(QDateTime::currentDateTime().addSecs(60));
        }
        startdt = QDateTime::currentDateTime();
    }else{
        ui->progress->setValue(int(double(QDateTime::currentDateTime().secsTo(ui->dte_max_datetime->dateTime()))/
                                   double(startdt.secsTo(ui->dte_max_datetime->dateTime()))*
                                   100.));
    }
}
