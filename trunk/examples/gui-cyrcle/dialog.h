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
programs for N FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers / users,
no additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    dialog.h
 * @date    19/12/2009
 * @version 1.18
**/

#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <../fitness-functions/cyrcle/CCyrcle.h>
#include <../../idl/ICancelService.h>
#include <../../idl/IFitness.h>

class CalcThread;

class Dialog : public QDialog,
       virtual public InsularGenetica::ICancelService
{
    Q_OBJECT
    Q_DISABLE_COPY(Dialog)
public:
    explicit Dialog(QWidget *parent = 0);
    virtual ~Dialog();
    /**
     * @brief   Checking: is evaluation canceled
     * @return  true,  if evaluation was canceled
     * @return  false, if evaluation wasn't canceled
    **/
    bool isCanceled()
    {
        return isHidden();
    };
private slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QTimer      m_timer;
    CCyrcle     m_cyrcle;
    CalcThread *m_calc_thread;
};

/**
 * @class   CalcThread
 * @brief   Declaration of CalcThread
**/
class CalcThread : public QThread,
           virtual public InsularGenetica::ICancelService
{
    Q_OBJECT
public :
    /**
     * @brief   Constructor
    **/
    CalcThread(InsularGenetica::IFitness*fitness,
               InsularGenetica::ICancelService*cancel);
    /**
     * @brief   Destructor
    **/
    ~CalcThread();
    /**
     * @brief   Checking: is evaluation canceled
     * @return  true,  if evaluation was canceled
     * @return  false, if evaluation wasn't canceled
    **/
    bool isCanceled();
private:
    /**
     * @brief   Main method of evaluations
    **/
    void run();
private:
    InsularGenetica::IFitness*      m_fitness;    ///<! Fitness function
    InsularGenetica::ICancelService*m_cancel;     ///<! Cancel service
    bool                            m_cancel_flag;///<! Cancel flag
};

#endif // DIALOG_H
