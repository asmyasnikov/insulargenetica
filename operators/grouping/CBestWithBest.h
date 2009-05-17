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
 * @file    CBestWithBest.h
 * @brief   Файл содержит класс CBestWithBest отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.18
**/
#ifndef C_BEST_WITH_BEST_H_INCLUDED
#define C_BEST_WITH_BEST_H_INCLUDED
#include "../../idl/IGrouping.h"
#include "../../include/CPopulation.h"
#include "../../include/CChromosome.h"
#include <qglobal.h>
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
    struct CBestWithBest : virtual public IGrouping
    {
        /**
         * @brief   Базовый конструктор
        **/
        CBestWithBest(){};
        /**
         * @brief   Деструктор
        **/
        ~CBestWithBest(){};
        /**
         * @brief  Метод отбора пар родительских хромосом для скрещивания
         * @param  sel - популяция родителей для скрещивания
         * @return par - набор пар родителей
        **/
        void group(const CPopulation&sel, CParents&par)
        {
            Q_ASSERT(sel.size());
        if(sel.size()>3)
        {
                unsigned int size_new = int(double(sel.size()) *
                                            double(rand()) /
                                            double(RAND_MAX));
                for(unsigned int i = 0; i < size_new; i++)
                {
                unsigned int best1 = 0;
                unsigned int best2 = 0;
                while(best1 == best2)
                {
                        best1 = rand()%(sel.size()/2);
                        best2 = rand()%(sel.size()/2);
                }
                par.append(qMakePair(&sel.getChromosome(best1),
                                         &sel.getChromosome(best2)));
            }
            }else if(sel.size()>1){
                par.append(qMakePair(&sel.getChromosome(0),
                                     &sel.getChromosome(1)));
        }
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Группировка лучших хромосом с лучшими");
        };
    };
};

#endif // C_BE
