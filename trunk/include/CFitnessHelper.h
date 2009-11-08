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
 * @file    CFitnessHelper.h
 * @class   CFitnessHelper
 * @brief   Class contains extra methods for genotype to phenotype
 *          and bitstring to double conversions
 * @author  Aleksey Myasnikov
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef C_FITNESS_HELPER_HEADER
#define C_FITNESS_HELPER_HEADER
#include "../idl/IFitness.h"
#include "export.h"
namespace InsularGenetica
{
    class CChromosome;
    struct Q_DECL_EXPORT CFitnessHelper : virtual public IFitness
    {
        /**
         * @brief   Construtor
        **/
        CFitnessHelper(unsigned int count);
        /**
         * @brief   Destructor
        **/
        ~CFitnessHelper();
        /**
         * @brief   Getting count of fitness calculations
         * @return  count of fitness calculations
        **/
        unsigned int count()const;
        /**
         * @brief   Method of genotype to phenotype conversion
         * @return  phenotype
         **/
#if QT_VERSION < 0x040000
        QValueList<double>
#else
        QList<double>
#endif
        genotype2phenotype(const CChromosome& chr);
    protected:
        unsigned int         m_count;  ///<! Number of fitnes function arguments
        mutable unsigned int m_counter;///<! Counter of fitness function calling
        /**
         * @brief  Method of bitstring (part of chromosome) to double
         *         value conversion. Part of chromosome is define one
         *         variable of many arguments.
         * @param  chr     - chromosome,
         * @param  index   - number of variable,
         * @return value of variable
        **/
        double decode(const CChromosome& chr,
                      unsigned int       index) const;

    protected:
        double min;
        double max;
    };
};
#endif // C_FITNESS_HELPER_HEADER
