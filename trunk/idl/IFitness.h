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
 * @file    IFitness.h
 * @brief   Файл содержит интерфейс IFitnes,
 *          который обеспечивает единообразие
 *          доступа к целевым функциям
 * @date    20/02/2009
 * @version 1.18
**/
#ifndef FITNESS_INTERFACE
#define FITNESS_INTERFACE
#include "Interface.h"
class QString;
namespace InsularGenetica
{
    class CChromosome;
    struct IFitness : virtual public Interface
    {
        /**
         * @brief   Метод вычисления значения целевой функции
         * @return  значение функции
        **/
        virtual double calc(const CChromosome& chr) = 0;
        /**
         * @brief   Метод получения наименования функции
         * @return  наименование функции
        **/
        virtual const QString name() = 0;
        /**
         * @brief   Метод получения количества рассчитанных целевых функций
         * @return  Количество рассчитанных целевых функций
        **/
        virtual unsigned int count() = 0;
    };
};
#endif // FITNESS_INTERF
