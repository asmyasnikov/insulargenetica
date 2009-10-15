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
 * @file    CRouletteSelection.h
 * @brief   Файл содержит класс CRouletteSelection отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.18
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
         * @brief   Базовый конструктор
        **/
        CRouletteSelection(){};
        /**
         * @brief   Деструктор
        **/
        ~CRouletteSelection(){};
        /**
         * @brief  Метод отбора из популяции хромосом для дальнейшего
         *         скрещивания и мутации
         * @param  pop - популяция родителей, из которых производится отбор
         * @return sel - популяция родителей для скрещивания и мутаций
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
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Рулеточный отбор");
        };
    };
};

#endif // C_ROUL
