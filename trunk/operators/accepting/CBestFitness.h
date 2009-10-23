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
 * @file    CBestFitness.h
 * @class   CBestFitness
 * @brief   Accepting if fitness of chomosome better the best
 *          fitness in population
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef C_BEST_FITNESS_H_INCLUDED
#define C_BEST_FITNESS_H_INCLUDED
#include "../../idl/IAccepting.h"
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
    struct CBestFitness : virtual public IAccepting
    {
        /**
         * @brief   Base constructor
        **/
        CBestFitness(){};
        /**
         * @brief   Destructor
        **/
        ~CBestFitness(){};
        /**
         * @brief  Method of acceptiing chromosome
         * @param  pop - population of parents
         * @param  cur - current chromosome
         * @return true , if chromosome cur was accepted
         * @return false, if chromosome cur was not accepted
        **/
        bool accept(const CPopulation*pop, const CChromosome&cur)
        {
            Q_ASSERT(CChromosome::size());
            return (pop->getChromosome(0) < cur);
        };
        /**
         * @brief   Getting name of genetic operator
         * @return  Name of genetic operator
        **/
        const QString name()
        {
            return QObject::trUtf8("Fitness of chromosome is "
                                   "better the best fitness "
                                   "in population");
        };
    };
};

#endif // C_BEST_FITNESS_H_INCLUDED
