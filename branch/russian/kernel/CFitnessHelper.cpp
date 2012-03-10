/****************************************************************************
** Copyright (C) 2009 Мясников А.С. Сергеевич.
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
** программы для ЭВМ" за N 2010610175 от 11.01.2010 г. Копия
** свидетельства о регистрации представлена в файле CERTIFICATE
** в корне проекта.
** Это не накладывает на конечных разработчиков/пользователей никаких
** дополнительных ограничений, кроме предусмотренных GNU Lesser GPL,
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКЕ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CFitnessHelper.cpp
 * @brief   Файл содержит реализацию вспомогательных методов,
 *          необходимых при расчете целевой функции
 * @date    20/02/2009
 * @version 1.14
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

