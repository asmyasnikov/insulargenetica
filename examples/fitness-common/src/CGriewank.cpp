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
 * @brief   Файл содержит класс CGriewank,
 *          который реализует тестовую целевую функцию Griewank.
 * @date    10/02/2009
 * @version 1.15
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
 * @brief   Базовый конструктор
**/
CGriewank::CGriewank() : CFitnessHelper(10) {};
/**
 * @brief   Конструктор
 * @param   count - количество переменных целевой функции
**/
CGriewank::CGriewank(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
};
/**
 * @brief   Деструктор
**/
CGriewank::~CGriewank(){};
/**
 * @brief   Метод вычисления значения целевой функции
 * @return  значение функции
**/
double CGriewank::calc(const InsularGenetica::CChromosome& chr)
{
    double result = 0.;
    double multiplicate = 1.;
    double summ         = 0.;
    for(unsigned long i = 0; i < m_count; i++)
    {
        double x      = (decode(chr, i)-0.5)*40;
        summ         += x*x/4000;
        multiplicate *= cos(x/sqrt(double(i+1)));
    }
    result=(m_invert?-1.:1.)*(multiplicate-summ-1.);
    m_counter++;
    return result;
};

/**
 * @brief   Метод получения наименования функции
 * @return  наименование функции
**/
const QString CGriewank::name()
{
    return QObject::trUtf8("Griewank [%1%2vars]")
           .arg(m_invert ? QObject::trUtf8("invert,") : QString::null)
           .arg(m_count);
};
