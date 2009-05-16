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
 * @file    CPopulation.cpp
 * @brief   Файл содержит класс CPopulation,
 *          который является аналогом популяции
 *          в генетическом программировании
 * @date    13/02/2009
 * @version 1.15
**/
#include <math.h>
#include "../idl/IFitness.h"
#include "../include/CPopulation.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qobject.h>
    #include <qmutex.h>
    #include <qvaluevector.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QObject>
    #include <QtCore/QMutexLocker>
#endif
/**
 * @brief   Конструктор
 * @param   size - размер популяции
**/
InsularGenetica::
CPopulation::
CPopulation(int size) :
    m_mutex(
#if QT_VERSION < 0x040000
             true
#else
             QMutex::Recursive
#endif
           )
{
    QMutexLocker locker(&m_mutex);
    if(size)
    {
        m_data.reserve(size);
        while(m_data.size() < size)
        {
            CChromosome chr;
            addChromosome(chr);
        }
    }
};
/**
 * @brief   Конструктор копирования
 *          Производится глубокое копирование данных
 * @param   pop - популяция, из которой происходит
 *          конструирование текущей популяции
**/
InsularGenetica::
CPopulation::
CPopulation(const CPopulation& pop) :
    m_mutex(
#if QT_VERSION < 0x040000
             true
#else
             QMutex::Recursive
#endif
           )
{
    QMutexLocker locker(&m_mutex);
    if(pop.size())
    {
        m_data.reserve(pop.size());
        for(int i = 0; i < pop.size(); i++)
        {
            m_data.push_back(new CChromosome(pop.getChromosome(i)));
        }
    }
};
/**
 * @brief   Деструктор
**/
InsularGenetica::
CPopulation::
~CPopulation()
{
    QMutexLocker locker(&m_mutex);
    for(int j = 0; j < m_data.size(); j++)
    {
        delete m_data[j];
    }
};
/**
 * @brief  Размер хромосомы
 * @return Размер хромосомы
**/
int
InsularGenetica::
CPopulation::
size() const
{
    QMutexLocker locker(&m_mutex);
    return m_data.size();
};
/**
 * @brief  Получить хромосому
 * @return Хромосома
**/
const
InsularGenetica::
CChromosome&
InsularGenetica::
CPopulation::
getChromosome(int index) const
{
    QMutexLocker locker(&m_mutex);
    Q_ASSERT(index < m_data.size());
    return *m_data.at(index);
};
/**
 * @brief  Добавить хромосому в популяцию
 * @param  chr - хромосома
**/
void
InsularGenetica::
CPopulation::
addChromosome(const CChromosome& chr)
{
    if(!isPresent(chr))
    {
        QMutexLocker locker(&m_mutex);
        CChromosome*cur = new CChromosome(chr);
        double fnew = cur->fitness();
        m_data.push_back(cur);
        if(m_data.size() > 1)
        {
            for(int i = m_data.size()-1; i > 0; i--)
            {
                if(fnew > m_data[i-1]->fitness())
                {
                    CChromosome*temp = m_data[i-1];
                    m_data[i-1]      = m_data[i];
                    m_data[i]        = temp;
                }
            }
        }
    }
};
/**
 * @brief  Рассчитать среднее здоровье популяции
 * @return Среднее здоровье популяции
**/
double
InsularGenetica::
CPopulation::
getAverageFitness() const
{
    QMutexLocker locker(&m_mutex);
    double average = 0.;
    for(int i = 0; i < m_data.size(); i++)
    {
        average += m_data[i]->fitness();
    }
    average /= double(m_data.size());
    return average;
};
/**
 * @brief  Рассчитать максимальное здоровье в популяции
 * @return Максимальное здоровье популяции
**/
double
InsularGenetica::
CPopulation::
getMaximumFitness() const
{
    QMutexLocker locker(&m_mutex);
    return m_data.size() ? m_data[0]->fitness() : -HUGE_VAL;
};
/**
 * @brief  Рассчитать минимальное здоровье в популяции
 * @return Минимальное здоровье популяции
**/
double
InsularGenetica::
CPopulation::
getMinimumFitness() const
{
    QMutexLocker locker(&m_mutex);
    return m_data.size() ? m_data[m_data.size()-1]->fitness() : HUGE_VAL;
};
/**
 * @brief  Рассчитать стенень однородности популяции
 * @param  pseudo - параметр позволяет рассчитать при (pseudo == true)
 *         псвдооднородность популяции. Псвдооднородность популяции
 *         рассчитывается с учетом незначительности отдельных несовпадений
 *         в генах приподавляющем большинстве совпадений.
 * @return Стенень однородности популяции в диапазоне от 0 до 1
 * @return Стенень псевдооднородности популяции в диапазоне от 0.5 до 1
**/
double
InsularGenetica::
CPopulation::
getHomogeneity(bool pseudo) const
{
    QMutexLocker locker(&m_mutex);
    int homogeneity = 0;
    for(unsigned int i = 0; i < CChromosome::size(); i++)
    {
        int tgene = 0;
        for(int j = 0; j < m_data.size(); j++)
        {
            if(m_data[j]->getGene(i)) tgene++;
        }
        if(pseudo)
        {
            homogeneity += qMax(tgene, m_data.size() - tgene);
        }else{
            if(qMax(tgene, m_data.size() - tgene) == m_data.size())
            {
                homogeneity += 1;
            }
        }
    }
    return double(homogeneity) /
           double(CChromosome::size()*(pseudo?m_data.size():1));
};
/**
 * @brief  Заменить в популяции худшую хромосому
 * @param  chr - хромосома для замены
**/
void
InsularGenetica::
CPopulation::
replaceChromosome(const CChromosome&chr)
{
    if(chr.fitness() > m_data[m_data.size()-1]->fitness())
    {
        if(!isPresent(chr))
        {
            QMutexLocker locker(&m_mutex);
            delete m_data[m_data.size()-1];
            CChromosome*cur = new CChromosome(chr);
            double fnew = cur->fitness();
            m_data[m_data.size()-1] = cur;
            if(m_data.size() > 1)
            {
                for(int i = m_data.size()-1; i > 0; i--)
                {
                    if(fnew > m_data[i-1]->fitness())
                    {
                        CChromosome*temp = m_data[i-1];
                        m_data[i-1]      = m_data[i];
                        m_data[i]        = temp;
                    }
                }
            }
        }
    }
};
/**
 * @brief  Проверить, есть ли в популяции такая же хромосома
 * @param  chr - хромосома для проверки
**/
bool
InsularGenetica::
CPopulation::
isPresent(const CChromosome&chr) const
{
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_data.size(); i++)
    {
        if(*m_data[i] == chr) return true;
    }
    return false;
};
/**
 * @brief   Оператор копирования
 *          Производится глубокое копирование данных
 * @param   pop - популяция, из которой происходит
 *          конструирование текущей популяции
**/
InsularGenetica::
CPopulation&
InsularGenetica::
CPopulation::
operator=(const CPopulation& pop)
{
    QMutexLocker locker(&m_mutex);
    if(pop.size())
    {
        m_data.reserve(pop.size());
        for(int i = 0; i < pop.size(); i++)
        {
            m_data.push_back(new CChromosome(pop.getChromosome(i)));
        }
    }
    return *this;
};
