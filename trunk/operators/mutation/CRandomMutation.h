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
 * @file    CRandomMutation.h
 * @brief   Class CRandomMutation
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef C_RANDOM_MUTATION_H_INCLUDED
#define C_RANDOM_MUTATION_H_INCLUDED
#include "../../idl/IMutation.h"
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
    Q_DECL_EXPORT struct CRandomMutation : virtual public IMutation
    {
        /**
         * @brief   Constructor
        **/
        CRandomMutation(double percentage = 0.5);
        /**
         * @brief   Destructor
        **/
        ~CRandomMutation();
        /**
         * @brief  Method of getting childs by mutation
         * @param  chr  - parent
         * @return cids - childs population
        **/
        void mutate(const CChromosome&  chr,
                    CPopulation&        cids);
        /**
         * @brief   Getting name of genetic operator
         * @return  name of genetic operator
        **/
        const QString name();
    private:
        int m_percentage;
    };
};
#endif // C_RANDOM_MUTATION_H_INCLUDED
