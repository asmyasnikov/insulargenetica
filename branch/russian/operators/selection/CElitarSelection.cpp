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
 * @file    CElitarSelection.cpp
 * @brief   Файл содержит класс CElitarSelection отбора родительских хромосом
 * @date    20/02/2009
**/
#include "CElitarSelection.h"
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
InsularGenetica::
CElitarSelection::
CElitarSelection(double percentage) :
    m_percentage(percentage)
{};
/**
 * @brief   Деструктор
**/
InsularGenetica::
CElitarSelection::
~CElitarSelection()
{};
/**
 * @brief  Метод отбора из популяции хромосом для дальнейшего скрещивания
 *         и мутации
 * @param  pop - популяция родителей, из которых производится отбор
 * @return sel - популяция родителей для скрещивания и мутаций
**/
void
InsularGenetica::
CElitarSelection::
select( const CPopulation&pop,
        CPopulation&      sel)
{
    Q_ASSERT(pop.size());
    double miminum = pop.getMinimumFitness();
    double summary = 0.;
    for(uint j = 0; j < pop.size(); j++)
    {
        summary += (pop.getChromosome(j).fitness() - miminum);
    }
    double number     = m_percentage * summary;
    double accumulate = 0.;
    for(uint i = 0; i < pop.size(); i++)
    {
        accumulate += (pop.getChromosome(i).fitness() - miminum);
        sel.addChromosome(pop.getChromosome(i));
        if((accumulate > number) && (sel.size() > 1)) break;
    }
    Q_ASSERT(sel.size() > 1);
};
/**
 * @brief   Метод получения наименования генетического оператора
 * @return  наименование генетического оператора
**/
const
QString
InsularGenetica::
CElitarSelection::
name()
{
    return QObject::trUtf8("%1-процентный элитарный отбор")
           .arg(m_percentage * 100.);
};

