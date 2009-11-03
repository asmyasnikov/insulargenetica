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
 * @file    CPopulation.h
 * @brief   Class CPopulation - population analog
 * @date    13/02/2009
 * @version 3.3
**/
#ifndef CPOPULATION_H_INCLUDED
#define CPOPULATION_H_INCLUDED
#include "export.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qmutex.h>
    #include <qvaluevector.h>
#else
    #include <QtCore/QMutex>
    #include <QtCore/QVector>
#endif
#include "../idl/IFitness.h"
#include "CChromosome.h"
namespace InsularGenetica
{
    struct Q_DECL_EXPORT CPopulation : virtual public Interface
    {
        /**
         * @brief   Constructor
         * @param   size - population size
        **/
        CPopulation(int size = 0);
        /**
         * @brief   Copy constructor
         * @param   pop - source population
        **/
        CPopulation(const CPopulation& pop);
        /**
         * @brief   Copy operator
         * @param   pop - source population
        **/
        CPopulation& operator=(const CPopulation& pop);
        /**
         * @brief   Destructor
        **/
        ~CPopulation();
        /**
         * @brief  Size of population
         * @return Size of population
        **/
        int size() const;
        /**
         * @brief  Getting chromosome
         * @return Chromosome
        **/
        const CChromosome& getChromosome(int index) const;
        /**
         * @brief  Adding chromosome into population
         * @param  chr - chromosome
        **/
        void addChromosome(const CChromosome& chr);
        /**
         * @brief  Getting population homogeineity
         * @param  pseudo - getting population pseudo homogeineity
         * @return Population homogeineity [0;1]
         * @return Population pseudo-homogeineity [0.5;1]
        **/
        double getHomogeneity(bool pseudo = false) const;
        /**
         * @brief  Method of replace chromosome
         * @param  chr - chromosome
        **/
        void replaceChromosome(const CChromosome&chr);
        /**
         * @brief  Checking chromosome exists
         * @param  chr - chromosome
        **/
        bool isPresent(const CChromosome&chr) const;
    private:
#if QT_VERSION < 0x040000
        QValueList<CChromosome> m_data; ///<! Chromosome in population
#else
        QList     <CChromosome> m_data; ///<! Chromosome in population
#endif
        ///<! Mutex
        mutable QMutex          m_mutex;
    };
};
#endif // CPOPULATION_H_INCLUDED
