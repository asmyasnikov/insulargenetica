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
 * @file    CInBreeding.h
 * @brief   Файл содержит класс CInBreeding отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.18
**/
#ifndef C_INBRIDING_H_INCLUDED
#define C_INBRIDING_H_INCLUDED
#include "../../idl/IGrouping.h"
#include "../../include/CPopulation.h"
#include "../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #include <qvaluevector.h>
    #include <qpair.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
    #include <QtCore/QVector>
    #include <QtCore/QPair>
#endif
namespace InsularGenetica
{
    struct CInBreeding : virtual public IGrouping
    {
        /**
         * @brief   Базовый конструктор
        **/
        CInBreeding(){};
        /**
         * @brief   Деструктор
        **/
        ~CInBreeding(){};
        /**
         * @brief  Метод отбора пар родительских хромосом для скрещивания
         * @param  sel - популяция родителей для скрещивания
         * @return par - набор пар родителей
        **/
        void group(const CPopulation&sel, CParents&par)
        {
            Q_ASSERT(sel.size());
#if QT_VERSION < 0x040000
            QValueVector<QPair<double, QPair<const CChromosome*,
                                             const CChromosome*> > >h_matrix;
#else
            QVector<QPair<double, QPair<const CChromosome*,
                                        const CChromosome*> > >h_matrix;
#endif
            for(int i = 0; i < sel.size(); i++)
            {
                for(int j = i+1; j < sel.size(); j++)
                {
                    const CChromosome*first  = &sel.getChromosome(i);
                    const CChromosome*second = &sel.getChromosome(j);
                    if((*first) == (*second)) continue;
                    double homogeinity = 0.;
                    for(unsigned int k = 0; k < CChromosome::size(); k++)
                    {
                        if(first->getGene(k) == second->getGene(k))
                        {
                            homogeinity += 1.;
                        }
                    }
                    h_matrix.push_back(qMakePair(homogeinity,
                                                 qMakePair(first,second)));
                }
            }
            for(int i = 0; i < h_matrix.size(); i++)
            {
                for(int j = i+1; j < h_matrix.size(); j++)
                {
                    if(h_matrix[i].first < h_matrix[j].first)
                    {
                        QPair<double, QPair<const CChromosome*,
                                            const CChromosome*> > temp =
                            h_matrix[j];
                        h_matrix[j] = h_matrix[i];
                        h_matrix[i] = temp;
                    }
                }
            }
            for(int i = 0; i < qMin(h_matrix.size(), sel.size()*2); i++)
            {
                par.append(h_matrix[i].second);
            }
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Инбридинг (группировка "
                                   "близких хромосом)");
        };
    };
};

#endif // C_INBRIDING_H_INCLUDED
