/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser GPL as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details.
You should have received a copy of the GNU Lesser GPL
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
 * @file    CCyrcle.h
 * @class   CCyrcle
 * @brief   Declaration of CCyrcle
 * @date    19/12/2009
 * @version 1.18
**/

#ifndef CCYRCLE_MODULE_H_INCLUDED
#define CCYRCLE_MODULE_H_INCLUDED

#include <QtCore/QTime>
#include <QtCore/QPoint>
#include "../../../include/export.h"
#include "../../../include/CFitnessHelper.h"
#include "../../../idl/IFitness.h"
#include "../../../include/CChromosome.h"

class QString;

struct Q_DECL_EXPORT CCyrcle : virtual public InsularGenetica::IFitness,
                               virtual public InsularGenetica::CFitnessHelper
{
    /**
     * @brief   Constructor
     * @param   radius - radius of cyrcle
     * @param   period - period of single circle inverse
    **/
    CCyrcle(double radius = 20., double period = 5);
    /**
     * @brief   Destructor
    **/
    ~CCyrcle();
    /**
     * @brief   Getting name of function
     * @return  name of function
    **/
    const QString name()const;
    /**
     * @brief Getting current point
    **/
    QPoint point() const;
    /**
     * @brief Getting neigbour point
    **/
    QPoint neigbourPoint() const;
    /**
     * @brief Getting neigbour point
    **/
    double time() const;
private:
    /**
     * @brief   Evaluate method
     * @return  output value of De Jong function
    **/
    double calc(const InsularGenetica::CChromosome& chr)const;
private:
    QTime                                       m_timer;
    double                                      m_radius;
    int                                         m_period;
    mutable const InsularGenetica::CChromosome* m_best;
    mutable bool                                m_update_best_cromosome;
};

#endif // CCYRCLE_MODULE_H_INCLUDED
