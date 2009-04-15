/****************************************************************************
** Copyleft (C) 2009 Мясников Алексей Сергеевич.
** Contact: AlekseyMyasnikov@yandex.ru
**          amyasnikov@npomis.ru
**          AlekseyMyasnikov@mail.ru
**          MyasnikovAleksey@mail.ru
** Этот файл является частью реализации библиотеки островного генетического
** алгоритма с динамическим распределением вероятностей выбора генетических
** операторов в каждой группе операторов
** Данная библиотека является свободным программным обеспечением. Вы вправе
** распространять её и/или модифицировать в соответствии с условиями версии 3
** либо по вашему выбору с условиями более поздней версии Стандартной
** Общественной Лицензии Ограниченного Применения GNU, опубликованной
** Free Software Foundation.
** Я распространяю эту библиотеку в надежде на то, что она будет вам
** полезной, однако НЕ ПРЕДОСТАВЛЯЮ НА НЕЕ НИКАКИХ ГАРАНТИЙ, в том числе
** ГАРАНТИИ ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и ПРИГОДНОСТИ ДЛЯ ИСПОЛЬЗОВАНИЯ
** В КОНКРЕТНЫХ ЦЕЛЯХ. Для получения более подробной информации ознакомьтесь
** со Стандартной Общественной Лицензией Ограниченного Применений GNU в
** файле LICENSE в корне исходных текстов проекта или по адресу:
** http://www.gnu.org/copyleft/lgpl.html.
** Обращаю Ваше внимание на то, что библиотека InsularGenetica
** зарегистрирована Российским агенством по патентам и товарным знакам
** (РОСПАТЕНТ), о чем выдано "Свидетельство об официальной регистрации
** программы для ЭВМ" за № FIXME от FIXME FIXME FIXME года. Копия
** свидетельства о регистрации представлена в файле CERTIFICATE
** в корне проекта.
** Это не накладывает на конечных разработчиков/пользователей никаких
** дополнительных ограничений, кроме предусмотренных GNU Lesser GPL,
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКЕ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CPopulation.cpp
 * @brief   Файл содержит класс CPopulation,
 *          который является аналогом популяции
 *          в генетическом программировании
 * @date    13/02/2009
**/
#include <math.h>
#include "../../idl/IFitness.h"
#include "../../include/CPopulation.h"
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
CChromosome&
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
CPopulation&
GeneticAlgorithm::
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
