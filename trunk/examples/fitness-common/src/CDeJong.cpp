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
 * @file    CDeJong.cpp
 * @class   CDeJong
 * @brief   Implementation of CDeJong
 * @date    22/02/2009
 * @version 1.18
**/

#include "../../include/CDeJong.h"
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
CDeJong::CDeJong() : CFitnessHelper(2)
{
    min = -65.036;
    max =  65.036;
};
/**
 * @brief   Constructor
 * @param   count - variables count of De Jong function
**/
CDeJong::CDeJong(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
    Q_ASSERT((m_count == 2) || (m_count == 5));
    if(m_count == 2)
    {
        min = -65.036;
        max =  65.036;
    }else{
        min = -5.12;
        max =  5.12;
    }
};
/**
 * @brief   Destructor
**/
CDeJong::~CDeJong(){};
/**
 * @brief   Evaluate method
 * @return  output value of De Jong function
**/
double CDeJong::calc(const InsularGenetica::CChromosome& chr)const
{
    double result = (m_count == 2 ? 0.002 : 0.);
    if(m_count == 2)
    {
        double x1 = (decode(chr, 0)-0.5)*(max-min);
        double x2 = (decode(chr, 1)-0.5)*(max-min);
        for(int j = 1; j < 26; j++)
        {
            result += 1./(double(j)+pow(x1-16.*(double(j/5)-2.),6.)+
                                    pow(x2-16.*(double(j)-5.*double(j/5)-2.),6.));
        }
    }else if(m_count == 5){
        for(unsigned long i = 0; i < m_count; i++)
        {
            double x = (decode(chr, i)-0.5)*(max-min);
            result += floor(x);
        }
    }
    m_counter++;
    return result;
};

/**
 * @brief   Getting name of function
 * @return  name of function
**/
const QString CDeJong::name()const
{
    return QObject::trUtf8("DeJong [%1%2vars]")
           .arg(m_invert ? QObject::trUtf8("invert,") : QString::null)
           .arg(m_count);
};
