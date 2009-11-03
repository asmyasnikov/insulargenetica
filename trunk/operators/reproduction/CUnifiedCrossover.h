/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser GPL as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details.
You should have received a copy of the GNU Lesser GPL
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
 * @file    CUnifiedCrossover.h
 * @brief   Class CUnifiedCrossover
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef C_UNIFIED_CROSSOVER_H_INCLUDED
#define C_UNIFIED_CROSSOVER_H_INCLUDED
#include "../../idl/IReproduction.h"
#include "../../include/CPopulation.h"
#include "../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif
namespace InsularGenetica
{
    struct CUnifiedCrossover : virtual public IReproduction
    {
        /**
         * @brief   Constructor
        **/
        CUnifiedCrossover(){};
        /**
         * @brief   Destructor
        **/
        ~CUnifiedCrossover(){};
        /**
         * @brief  Method of getting childs from two parents by reproduction
         * @param  pair - two parents
         * @return cids - childs population
        **/
        void reproduct(const QPair<const CChromosome*,
                                   const CChromosome*>pair,
                       CPopulation&cids)
        {
            Q_ASSERT(CChromosome::size() > 1);
            CChromosome child1(*pair.first);
            CChromosome child2(*pair.second);
            for(unsigned int i = 0; i < CChromosome::size(); i++)
            {
                if(rand() % 2)
                {
                    child1.setGene(i, pair.second->getGene(i));
                    child2.setGene(i, pair.first->getGene(i));
                }
            }
            cids.addChromosome(child1);
            cids.addChromosome(child2);
        };
        /**
         * @brief   Getting name of genetic operator
         * @return  name of genetic operator
        **/
        const QString name()
        {
            return QObject::trUtf8("Unified multi-dot crossover");
        };
    };
};

#endif // C_UNIFIED_CROSSOVER_H_INCLUDED
