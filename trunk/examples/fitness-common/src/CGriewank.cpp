/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
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
 * @file    CGriewank.cpp
 * @class   CGriewank
 * @brief   Implmentation of CGriewank
 * @date    10/02/2009
 * @version 1.18
**/

#include "../../include/CGriewank.h"
#include <math.h>
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif

/**
 * @brief   Base constructor
**/
CGriewank::CGriewank() : CFitnessHelper(10)
{
    min = -20.;
    max =  20.;
};
/**
 * @brief   Constructor
 * @param   count - variables count of De Jong function
**/
CGriewank::CGriewank(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
    min = -20.;
    max =  20.;
};
/**
 * @brief   Destructor
**/
CGriewank::~CGriewank(){};
/**
 * @brief   Evaluate method
 * @return  output value of De Jong function
**/
double CGriewank::calc(const InsularGenetica::CChromosome& chr)const
{
    double result = 0.;
    double multiplicate = 1.;
    double summ         = 0.;
    for(unsigned long i = 0; i < m_count; i++)
    {
        double x      = (decode(chr, i)-0.5)*(max-min);
        summ         += x*x/4000;
        multiplicate *= cos(x/sqrt(double(i+1)));
    }
    result=(m_invert?-1.:1.)*(multiplicate-summ-1.);
    m_counter++;
    return result;
};

/**
 * @brief   Getting name of function
 * @return  name of function
**/
const QString CGriewank::name()const
{
    return QObject::trUtf8("Griewank [%1%2vars]")
           .arg(m_invert ? QObject::trUtf8("invert,") : QString::null)
           .arg(m_count);
};
