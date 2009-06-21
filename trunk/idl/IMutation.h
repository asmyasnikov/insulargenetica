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
 * @file    IMutation.h
 * @brief   Файл содержит интерфейс IMutation отбора родительских пар
 * @date    17/02/2009
 * @version 1.18
**/
#ifndef INTERFACE_MUTATION_H_INCLUDED
#define INTERFACE_MUTATION_H_INCLUDED
#include "IGeneticOperator.h"
#include "../include/CChromosome.h"
#include "../include/CPopulation.h"
namespace InsularGenetica
{
    struct IMutation : virtual public IGeneticOperator
    {
        /**
         * @brief  Метод "рождения" мутированных потомков
         * @param  chr  - родительская хромосома, из которой "рождается"
         *                мутированный потомок
         * @return cids - популяция потомков
        **/
        virtual void mutate(const CChromosome&  chr,
                            CPopulation&        cids) = 0;
    };
};
#endif // INTERFACE_MUTATIO
