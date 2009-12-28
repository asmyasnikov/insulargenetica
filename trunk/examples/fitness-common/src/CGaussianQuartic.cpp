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
 * @file    CGaussianQuartic.cpp
 * @class   CGaussianQuartic
 * @brief   Implementation of CGaussianQuartic
 * @date    22/02/2009
 * @version 1.18
**/

#include "../../include/CGaussianQuartic.h"
#include <math.h>
#include <stdlib.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #include <qglobal.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
    #include <QtGlobal>
#endif

double gauss(double mx, double sigma)
{
    double x(0.);
    double y(0.);
    double r(0.);
    do{
        x = 2. * double(rand())/double(RAND_MAX) - 1.;
        y = 2. * double(rand())/double(RAND_MAX) - 1.;
        r = x*x + y*y;
    }while(r > 1.);
    return (mx + sigma * y * sqrt(-2. * log(r) / r));
};

/**
 * @brief   Base constructor
**/
CGaussianQuartic::CGaussianQuartic() : CFitnessHelper(2)
{
    min = -5.12;
    max =  5.12;
};
/**
 * @brief   Constructor
 * @param   count - variables count of De Jong function
**/
CGaussianQuartic::CGaussianQuartic(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
    Q_ASSERT(m_count > 0);
    min = -5.12;
    max =  5.12;
};
/**
 * @brief   Destructor
**/
CGaussianQuartic::~CGaussianQuartic(){};
/**
 * @brief   Evaluate method
 * @return  output value of De Jong function
**/
double CGaussianQuartic::calc(const InsularGenetica::CChromosome& chr)const
{
    double result = 0.;
    for(unsigned long i = 0; i < m_count; i++)
    {
        double x = (decode(chr, i)-0.5)*(max-min);
        result += pow(x,4.) + gauss(0,1);
    }
    m_counter++;
    return (m_invert ? -1. : 1.) * result;
};

/**
 * @brief   Getting name of function
 * @return  name of function
**/
const QString CGaussianQuartic::name()const
{
    return QObject::trUtf8("Gaussian Quartic [%1%2vars]")
           .arg(m_invert ? QObject::trUtf8("invert,") : QString::null)
           .arg(m_count);
};

