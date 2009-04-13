/****************************************************************************
** Copyright (C) 2009 Мясников Алексей Сергеевич.
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
****************************************************************************/
/**
 * @file    CFitnessHelper.cpp
 * @brief   Файл содержит реализацию вспомогательных методов,
 *          необходимых при расчете целевой функции
 * @date    20/02/2009
**/

#include "../../include/CFitnessHelper.h"
#include "../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qobject.h>
#else
    #include <QtCore/QObject>
#endif

/**
 * @brief   Базовый конструктор
**/
GeneticAlgorithm::
CFitnessHelper::
CFitnessHelper(unsigned int count) :
    m_count(count)
{
    m_counter = 0;
};

/**
 * @brief   Деструктор
**/
GeneticAlgorithm::
CFitnessHelper::
~CFitnessHelper()
{
};

/** @brief  Метод для преобразования хромосомы (части хромосомы) к типу double
            Перед использованием этого метода переменная m_count обязательно
            должна быть инициализирована ненулевым положительным значеним.
    @param  chr     - хромосома,
    @param  index   - номер переменной,
    @return значение в диапазоне от 0 до 1, соответствующее участку хромосомы
**/
double
GeneticAlgorithm::
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
GeneticAlgorithm::
CFitnessHelper::
count()
{
    return m_counter;
};
