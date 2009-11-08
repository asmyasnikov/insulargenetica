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
 * @file    CRouletteSelection.h
 * @brief   Class CRouletteSelection
 * @author  Aleksey Myasnikov
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef C_ROULETTE_SELECTION_H_INCLUDED
#define C_ROULETTE_SELECTION_H_INCLUDED
#include "../../idl/ISelection.h"
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
    struct CRouletteSelection : virtual public ISelection
    {
        /**
         * @brief   Constructor
        **/
        CRouletteSelection(){};
        /**
         * @brief   Destructor
        **/
        ~CRouletteSelection(){};
        /**
         * @brief  Method of selection chromosomes from population
         * @param  pop - parents population
         * @return sel - parents pool
        **/
        void select(const CPopulation&pop, CPopulation&sel)
        {
            Q_ASSERT(pop.size() > 1);
            double weight  = (double(pop.size()+1)*double(pop.size()+1)-
                              double(pop.size())-1)/2./double(pop.size());
            for(int i = 0; i < pop.size(); i++)
            {
                double number     = double(rand()) /
                                    double(RAND_MAX);
                double accumulate = double(pop.size()-i)*weight;
                if(accumulate > number)
                {
                    sel.addChromosome(pop.getChromosome(i));
                }
            }
            int i = 0;
            while(sel.size() < 2)
            {
                sel.addChromosome(pop.getChromosome(i % pop.size()));
                i++;
            }
            Q_ASSERT(sel.size() > 1);
        };
        /**
         * @brief   Getting name of genetic operator
         * @return  name of genetic operator
        **/
        const QString name()
        {
            return QObject::trUtf8("Roulette selection");
        };
    };
};

#endif // C_ROULETTE_SELECTION_H_INCLUDED
