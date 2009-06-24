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
 * @file    CRandomMutation.cpp
 * @brief   Файл содержит класс CRandomMutation отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.18
**/
#include "CRandomMutation.h"
#include <math.h>
/**
 * @brief   Базовый конструктор
**/
InsularGenetica::
CRandomMutation::
CRandomMutation(double percentage) :
    m_percentage(int(double(RAND_MAX)*percentage))
{};
/**
 * @brief   Деструктор
**/
InsularGenetica::
CRandomMutation::
~CRandomMutation(){};
/**
 * @brief  Метод "рождения" мутированных потомков
 * @param  chr  - родительская хромосома, из которой "рождается" мутант
 * @return cids - популяция потомков
**/
void
InsularGenetica::
CRandomMutation::
mutate( const CChromosome&chr,
        CPopulation&cids)
{
    Q_ASSERT(CChromosome::size());
    CChromosome child(chr);
    child.begin();
    for(unsigned int i = 0; i < CChromosome::size(); i++)
    {
        if(rand() < m_percentage) child.invertGene(i);
    }
    child.commit();
    cids.addChromosome(child);
};
/**
 * @brief   Метод получения наименования генетического оператора
 * @return  наименование генетического оператора
**/
const
QString
InsularGenetica::
CRandomMutation::
name()
{
    return QObject::trUtf8("%1-процентная случайная мутация").arg(ceil(m_percentage / double(RAND_MAX) * 100. - 0.01));
};