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
 * @brief   Файл содержит класс CDeJong,
 *          который реализует тестовую целевую функцию DeJong'а.
 * @date    22/02/2009
 * @version 1.15
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
 * @brief   Базовый конструктор
**/
CDeJong::CDeJong() : CFitnessHelper(10) {};
/**
 * @brief   Конструктор
 * @param   count - количество переменных целевой функции
**/
CDeJong::CDeJong(unsigned int count, bool invert) :
    CFitnessHelper(count),
    m_invert(invert)
{
    Q_ASSERT((m_count == 2) || (m_count == 5));
};
/**
 * @brief   Деструктор
**/
CDeJong::~CDeJong(){};
/**
 * @brief   Метод вычисления значения целевой функции
 * @return  значение функции
**/
double CDeJong::calc(const InsularGenetica::CChromosome& chr)
{
    double result = (m_count == 2 ? 0.002 : 0.);
    if(m_count == 2)
    {
        double x1 = (decode(chr, 0)-0.5)*131.072;
        double x2 = (decode(chr, 1)-0.5)*131.072;
        for(int j = 1; j < 26; j++)
        {
            result += 1./(double(j)+pow(x1-16.*(double(j/5)-2.),6.)+
                                    pow(x2-16.*(double(j)-5.*double(j/5)-2.),6.));
        }
    }else if(m_count == 5){
        for(unsigned long i = 0; i < m_count; i++)
        {
            double x = (decode(chr, i)-0.5)*10.24;
            result += floor(x);
        }
    }
    m_counter++;
    return result;
};

/**
 * @brief   Метод получения наименования функции
 * @return  наименование функции
**/
const QString CDeJong::name()
{
    return QObject::trUtf8("DeJong [%1%2vars]")
           .arg(m_invert ? QObject::trUtf8("invert,") : QString::null)
           .arg(m_count);
};
