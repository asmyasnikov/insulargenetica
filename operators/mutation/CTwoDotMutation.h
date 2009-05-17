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
 * @file    CTwoDotMutation.h
 * @brief   Файл содержит класс CTwoDotMutation отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.18
**/
#ifndef C_TWO_DOT_MUTATION_H_INCLUDED
#define C_TWO_DOT_MUTATION_H_INCLUDED
#include "../../idl/IMutation.h"
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
    struct CTwoDotMutation : virtual public IMutation
    {
        /**
         * @brief   Базовый конструктор
        **/
        CTwoDotMutation(){};
        /**
         * @brief   Деструктор
        **/
        ~CTwoDotMutation(){};
        /**
         * @brief  Метод "рождения" мутированных потомков
         * @param  chr  - родительская хромосома, из которой "рождается"
         *                мутированный потомок
         * @return cids - популяция потомков
        **/
        void mutate(const CChromosome&chr, CPopulation&cids)
        {
            Q_ASSERT(CChromosome::size());
            CChromosome child(chr);
            unsigned int locus2 = 0;
            unsigned int locus1 = 0;
            while(locus1 == locus2)
            {
                locus1 = qMin(int(CChromosome::size()) - 1,
                              rand() % int(CChromosome::size()));
                locus2 = qMin(int(CChromosome::size()) - 1,
                              rand() % int(CChromosome::size()));
            }
            child.begin();
            child.invertGene(locus1);
            child.invertGene(locus2);
            child.commit();
            cids.addChromosome(child);
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Двухточечная мутация");
        };
    };
};

#endif // C_T
