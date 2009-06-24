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
 * @file    CFitnessHelper.cpp
 * @brief   Файл содержит реализацию вспомогательных методов,
 *          необходимых при расчете целевой функции
 * @date    20/02/2009
 * @version 1.18
**/
#include "../include/CFitnessHelper.h"
#include "../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qobject.h>
#else
    #include <QtCore/QObject>
#endif
/**
 * @brief   Базовый конструктор
**/
InsularGenetica::
CFitnessHelper::
CFitnessHelper(unsigned int count) :
    m_count(count)
{
    m_counter = 0;
};
/**
 * @brief   Деструктор
**/
InsularGenetica::
CFitnessHelper::
~CFitnessHelper()
{
};
/** @brief  Метод для преобразования части хромосомы к типу double
            Перед использованием этого метода переменная m_count обязательно
            должна быть инициализирована ненулевым положительным значеним.
    @param  chr     - хромосома,
    @param  index   - номер переменной,
    @return значение в диапазоне от 0 до 1, соответствующее участку хромосомы
**/
double
InsularGenetica::
CFitnessHelper::
decode( const CChromosome&  chr,
        unsigned int        index) const
{
    Q_ASSERT(CChromosome::size() >= m_count);
    unsigned int start  = (CChromosome::size()/m_count*index);
    unsigned int end    = (index == (m_count-1)) ?
                           CChromosome::size() :
                          (CChromosome::size()/m_count*(index+1));
    double result   = 0.;
    double base     = 1.;
    double maximum  = 0.;
    for(unsigned int i = end; i > start; i--)
    {
        if(chr.getGene(i-1))
        {
            result += base;
        }
        maximum += base;
        base *= 2.;
    }
    return (result/maximum);
};
/**
 * @brief   Метод получения количества рассчитанных целевых функций
 * @return  Количество рассчитанных целевых функций
**/
unsigned int
InsularGenetica::
CFitnessHelper::
count()
{
    return m_counter;
};
/**
 * @brief   Метод перекодирования генотипа в фенотип
 * @return  Фенотип
 **/
#if QT_VERSION < 0x040000
QValueList<double>
#else
QList<double>
#endif
InsularGenetica::
CFitnessHelper::
genotype2phenotype(const CChromosome& chr)
{
#if QT_VERSION < 0x040000
    QValueList<double>
#else
    QList<double>
#endif
    result;
    for(unsigned int i = 0; i < m_count; i++)
    {
        result.append(decode(chr, i)*(max-min)+min);
    }
    return result;
}