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
 * @file    CRastrigin.cpp
 * @brief   Файл содержит класс CRastrigin,
 *          который реализует тестовую целевую функцию Растригина.
 * @date    10/02/2009
 * @version 1.15
**/

#include "../../include/CRastrigin.h"
#include <math.h>
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif

/**
 * @brief   Базовый конструктор
**/
CRastrigin::CRastrigin() : CFitnessHelper(10)
{
    min = -5.12;
    max =  5.12;
};
/**
 * @brief   Конструктор
 * @param   count - количество переменных целевой функции
 * @param   filter - Фильтрация хромосом по доле единичных битов [0..1]
**/
CRastrigin::CRastrigin(unsigned int count, bool invert, double filter) :
    CFitnessHelper(count),
    m_invert(invert),
    m_filter(filter)
{
    min = -5.12;
    max =  5.12;
};
/**
 * @brief   Деструктор
**/
CRastrigin::~CRastrigin(){};
/**
 * @brief   Метод вычисления значения целевой функции
 * @return  значение функции
**/
double CRastrigin::calc(const InsularGenetica::CChromosome& chr)
{
    double result = 0.;
    for(unsigned long i = 0; i < m_count; i++)
    {
        double x = (decode(chr, i)-0.5)*(max-min);
        result  += (10.*(cos(2.*M_PI*x)-1) - x*x);
    }
    if(m_filter > 0. && m_filter <= 1.)
    {
        unsigned int count = 0;
        for(uint i = 0; i < InsularGenetica::CChromosome::size(); i++)
        {
            if(chr.getGene(i)) count++;
        }
        result *= exp(qMax(0.,double(count)-m_filter*double(InsularGenetica::CChromosome::size())));
    }
    m_counter++;
    return ((m_invert)?(-1.):(1.))*result;
};

/**
 * @brief   Метод получения наименования функции
 * @return  наименование функции
**/
const QString CRastrigin::name()
{
    return QObject::trUtf8("Rastrigin [%1%2vars%3]")
           .arg(m_invert ? QObject::trUtf8("invert,") : QString::null)
           .arg(m_count)
           .arg(m_filter>0.?QObject::trUtf8(",limit%1%bit").arg(int(m_filter*100.)):QString::null);
};
