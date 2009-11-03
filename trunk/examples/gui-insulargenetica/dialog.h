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

/**
 * @class   CDialog
 * @brief   Declaration of CDialog
 *          CDialog provides accept to GUI-widgets and implement cancel service
**/
class Dialog : public QDialog,
               public InsularGenetica::ICancelService
{
    Q_OBJECT

public:
    /**
     * @brief   Base constructor
    **/
    Dialog(QWidget *parent = 0);
    /**
     * @brief   Destructor
    **/
    ~Dialog();
    /**
     * @brief   Checking: is evaluation canceled
     * @return  true,  if evaluation was canceled
     * @return  false, if evaluation wasn't canceled
    **/
    bool isCanceled();

    friend class CalcThread;

public slots:
    /**
     * @brief   Starting or finishing evaluations slot
    **/
    void calc(bool);
    /**
     * @brief   Changing top limit of results spin box
    **/
    void setMaxResults();
    /**
     * @brief   Auto updating time bottom limit
    **/
    void updateTime();
    /**
     * @brief   Updating results after thread finishing
    **/
    void updateResults();

private:
    Ui::DialogClass *ui; ///<! GUI
    CalcThread*calc_thread; ///<! evaluation thread
    QTimer timer;  ///<! timer
    QList<InsularGenetica::IFitness*> m_fitness_modules; ///<! fitness function
    QList<QLibrary*> m_fitness_libraries;  ///<! libraries of fitness functions
};

/**
 * @class   CalcThread
 * @brief   Declaration of CalcThread
**/
class CalcThread : public QThread
{
    Q_OBJECT
public :
    /**
     * @brief   Constructor
    **/
    CalcThread(Dialog*_d);
    /**
     * @brief   Destructor
    **/
    ~CalcThread();
    /**
     * @brief   Updating results after thread finishing
    **/
    void updateResults();
private:
    /**
     * @brief   Main method of evaluations
    **/
    void run();
private:
    Dialog*d; ///<! dialog pointer
    InsularGenetica::IFitness*f; ///<! Fitness function
    InsularGenetica::CPopulation*best; ///<! result
};

#endif // DIALOG_H
