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
    for(int j = 0; j < pop.size(); j++)
    {
        summary += (pop.getChromosome(j).fitness() - miminum);
    }
    double number     = m_percentage * summary;
    double accumulate = 0.;
    for(int i = 0; i < pop.size(); i++)
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
