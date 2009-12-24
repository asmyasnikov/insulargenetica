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
 * @file    CCyrcle.cpp
 * @class   CCyrcle
 * @brief   Declaration of CCyrcle
 * @date    19/12/2009
 * @version 1.18
**/

#include "CCyrcle.h"
#include <math.h>

/**
 * @brief   Base constructor
**/
CCyrcle::CCyrcle(double radius, double period) :
    InsularGenetica::CFitnessHelper(2),
    m_timer(QTime(0,0,0,0)),
    m_radius(radius),
    m_period(int(period*1000.)),
    m_best(NULL),
    m_update_best_cromosome(true)
{
    min = 0;
    max = 2*m_radius;
    m_timer.start();
}
/**
 * @brief   Destructor
**/
CCyrcle::~CCyrcle()
{
}
/**
 * @brief   Getting name of function
 * @return  name of function
**/
const QString CCyrcle::name()const
{
    return QObject::trUtf8("Cyrcle [2vars,rad=%1]")
           .arg(m_radius);
}
/**
 * @brief   Evaluate method
 * @return  output value of Cyrcle function
**/
double CCyrcle::calc(const InsularGenetica::CChromosome& chr)const
{
    double angle  = double(m_timer.elapsed()%m_period)/double(m_period)*2.*M_PI;
    double x1     = m_radius+m_radius*cos(angle);
    double y1     = m_radius+m_radius*sin(angle);
    double x2     = (decode(chr, 0))*2*m_radius;
    double y2     = (decode(chr, 1))*2*m_radius;
    double result = -sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))+
                     100.*(cos(M_PI*(x2-x1)/2.)+cos(M_PI*(y2-y1)/2.))-(x2-x1)*(x2-x1)-(y2-y1)*(y2-y1);
    if(m_update_best_cromosome)
    {
        m_update_best_cromosome = false;
        if(!m_best)
        {
            m_best = new InsularGenetica::CChromosome(chr);
        }else if(calc(*m_best) < result){
            delete m_best;
            m_best = new InsularGenetica::CChromosome(chr);
        }
        m_update_best_cromosome = true;
    }
    return result;
}
/**
 * @brief Getting current point
**/
QPoint CCyrcle::point() const
{
    double angle = double(m_timer.elapsed()%m_period)/double(m_period)*2.*M_PI;
    double x     = m_radius+m_radius*cos(angle);
    double y     = m_radius+m_radius*sin(angle);
    return QPoint(int(x),int(y));
}

/**
 * @brief Getting neigbour point
**/
QPoint CCyrcle::neigbourPoint() const
{
    if(m_best)
    {
        double x = (decode(*m_best, 0))*2*m_radius;
        double y = (decode(*m_best, 1))*2*m_radius;
        return QPoint(int(x),int(y));
    }else{
        return QPoint(0,0);
    }
}

/**
 * @brief Getting neigbour point
**/
double CCyrcle::time() const
{
    return double(m_timer.elapsed()%m_period)/1000.;
}
