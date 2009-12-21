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
 * @file    dialog.cpp
 * @date    19/12/2009
 * @version 1.18
**/

#include "../../include/CGeneticController.h"
#include <QtGui/QPainter>
#include "dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_timer(this),
    m_cyrcle(CCyrcle(200.,5)),
    m_calc_thread(NULL)
{
    resize(460,460);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(update()));
    m_timer.start(50);
    m_calc_thread = new CalcThread(&m_cyrcle,this);
    m_calc_thread->start();
}

Dialog::~Dialog()
{
    delete m_calc_thread;
}

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, 460, 460, Qt::white);
    QFont f = painter.font();
    f.setPointSize(f.pointSize()*2);
    painter.setFont(f);
    painter.drawText(QPoint(445,435),"y");
    painter.drawText(QPoint(25,20),"x");
    painter.translate(0,20);
    painter.drawLine(QPoint(30,10), QPoint(30,410));
    painter.drawLine(QPoint(130,10), QPoint(130,410));
    painter.drawLine(QPoint(230,10), QPoint(230,410));
    painter.drawLine(QPoint(330,10), QPoint(330,410));
    painter.drawLine(QPoint(430,10), QPoint(430,410));
    painter.drawLine(QPoint(30,410), QPoint(430,410));
    painter.drawLine(QPoint(30,310), QPoint(430,310));
    painter.drawLine(QPoint(30,210), QPoint(430,210));
    painter.drawLine(QPoint(30,110), QPoint(430,110));
    painter.drawLine(QPoint(30,10), QPoint(430,10));
    painter.drawText(QPoint(15,410),"0");
    painter.drawText(QPoint(5,220),"20");
    painter.drawText(QPoint(5,25),"40");
    painter.drawText(QPoint(35,430),"0");
    painter.drawText(QPoint(220,430),"20");
    painter.drawText(QPoint(415,430),"40");
    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(QPoint(230,210),60,20);
    QPen pen;
    pen.setWidth(20);
    pen.setCapStyle(Qt::RoundCap);
//    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(QPoint(190,217),QString::fromLocal8Bit("t=%1 сек.").arg(m_cyrcle.time(),0,'f',1));
    painter.translate(30.,410.);
    painter.rotate(270);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(m_cyrcle.point());
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawPoint(m_cyrcle.neigbourPoint());
}

/**
 * @brief   Constructor
**/
CalcThread::CalcThread(InsularGenetica::IFitness*fitness,
                       InsularGenetica::ICancelService*cancel) :
    m_fitness(fitness),
    m_cancel(cancel),
    m_cancel_flag(false)
{
}
/**
 * @brief   Destructor
**/
CalcThread::~CalcThread()
{
    if(isRunning())
    {
        m_cancel_flag = true;
        wait();
    }
}
/**
 * @brief   Main method of evaluations
**/
void CalcThread::run()
{
    InsularGenetica::CGeneticController::calc(m_fitness,
                                              8,
                                              32,
                                              60*24,
                                              2,
                                              this);
}

/**
 * @brief   Checking: is evaluation canceled
 * @return  true,  if evaluation was canceled
 * @return  false, if evaluation wasn't canceled
**/
bool CalcThread::isCanceled()
{
    if(m_cancel_flag) return true;
    else              return m_cancel->isCanceled();
}
