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
 * @file    IGrouping.h
 * @brief   Interface IGrouping
 * @date    17/02/2009
 * @version 1.18
**/
#ifndef INTERFACE_GROUPING_H_INCLUDED
#define INTERFACE_GROUPING_H_INCLUDED
#include "IGeneticOperator.h"
#include "../include/CPopulation.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qlist.h>
    #include <qpair.h>
#else
    #include <QtCore/QList>
    #include <QtCore/QPair>
#endif
namespace InsularGenetica
{
    #if QT_VERSION < 0x040000
    typedef QValueList<QPair<const CChromosome*, const CChromosome*> >CParents;
    #else
    typedef QList<QPair<const CChromosome*, const CChromosome*> >CParents;
    #endif
    struct IGrouping : virtual public IGeneticOperator
    {
        /**
         * @brief  Method of grouping chromosomes from pool of parents to pairs
         * @param  sel - pool of parents
         * @return par - pairs of parents
        **/
        virtual void group( const CPopulation&  sel,
                            CParents&           par) = 0;
    };
};
#endif // INTERFACE_GROUPING_H_INCLUDED
