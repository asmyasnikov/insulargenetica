/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser GPL as publishedby
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details. You should have received a
copy of the GNU Lesser GPL in file LICENSE along with this
program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for N FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers/users, no
additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CElitarSelection.cpp
 * @class   CElitarSelection
 * @brief   Elitar selection of chromosomes from population to
 *          parents pool
 * @author  Aleksey Myasnikov
 * @date    20/02/2009
 * @version 3.3
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
 * @brief   Constructor
**/
InsularGenetica::
CElitarSelection::
CElitarSelection(double percentage) :
    m_percentage(percentage)
{};
/**
 * @brief   Destructor
**/
InsularGenetica::
CElitarSelection::
~CElitarSelection()
{};
/**
 * @brief  Method of selection chromosomes from population
 * @param  pop - parents population
 * @return sel - parents pool
**/
void
InsularGenetica::
CElitarSelection::
select( const CPopulation&pop,
        CPopulation&      sel)
{
    Q_ASSERT(pop.size());
    int number = int(double(pop.size())*m_percentage);
    for(int i = 0; i < number; i++)
    {
        sel.addChromosome(pop.getChromosome(i));
    }
    Q_ASSERT(sel.size() > 1);
};
/**
 * @brief   Getting name of genetic operator
 * @return  name of genetic operator
**/
const
QString
InsularGenetica::
CElitarSelection::
name()
{
    return QObject::trUtf8("%1-percentage elitar selection")
           .arg(m_percentage * 100.);
};
