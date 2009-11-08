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
 * @file    CBestWithAll.h
 * @class   CBestWithAll
 * @brief   Best chromosome from parents pool group with all chromosomes
 * @author  Aleksey Myasnikov
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef C_BEST_WITH_ALL_H_INCLUDED
#define C_BEST_WITH_ALL_H_INCLUDED
#include "../../idl/IGrouping.h"
#include "../../include/CPopulation.h"
#include "../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif
namespace InsularGenetica
{
    struct CBestWithAll : virtual public IGrouping
    {
        /**
         * @brief   Base constructor
        **/
        CBestWithAll(){};
        /**
         * @brief   Destructor
        **/
        ~CBestWithAll(){};
        /**
         * @brief  Method of grouping chromosomes from pool of parents to pairs
         * @param  sel - pool of parents
         * @return par - pairs of parents
        **/
        void group(const CPopulation&sel, CParents&par)
        {
            Q_ASSERT(sel.size());
            const CChromosome* best = &sel.getChromosome(0);
            for(int i = 1; i < sel.size(); i++)
            {
                par.append(qMakePair(best,&sel.getChromosome(i)));
            }
        };
        /**
         * @brief   Getting name of genetic operator
         * @return  Name of genetic operator
        **/
        const QString name()
        {
            return QObject::trUtf8("Best chromosome group "
                                   "with all chromosomes");
        };
    };
};

#endif // C_BEST_WITH_ALL_H_INCLUDED
