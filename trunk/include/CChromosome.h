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
 * @file    CChromosome.h
 * @class   CChromosome
 * @brief   This is the class CChromosome. It implement chromosome
 *          in genetic programming
 * @author  Aleksey Myasnikov
 * @date    20/02/2009
 * @version 3.3
**/
#ifndef CCHROMOSOME_H_INCLUDED
#define CCHROMOSOME_H_INCLUDED
#include "export.h"
#include "../idl/Interface.h"
#include "../idl/IFitness.h"
namespace InsularGenetica
{
    struct Q_DECL_EXPORT CChromosome : virtual public Interface
    {
        /**
         * @brief   Setting size of chromosome
         * @param   size - size of chromosome
        **/
        static void setSize(unsigned int size);
        /**
         * @brief   Getting size of chromosome
         * @param   size - size of chromosome
        **/
        static unsigned int size();
        /**
         * @brief   Setting static pointer of fitness function
         * @param   function - pointer of fitness function
        **/
        static void setFitnessFunction(IFitness* function);
        /**
         * @brief   Default constructor
         *          Chromosome init the random values
        **/
        CChromosome(void);
        /**
         * @brief   Constructor
         * @param   def - value of chromosome genes
        **/
        CChromosome(bool def);
        /**
         * @brief   Copy constructor
         * @param   chr - source chromosome
        **/
        CChromosome(const CChromosome& chr);
        /**
         * @brief   Copy constructor
         * @param   chr - source chromosome
        **/
        CChromosome&operator=(const CChromosome& chr);
        /**
         * @brief   Destructor
        **/
        ~CChromosome();
        /**
         * @brief   The comparison of two chromosomes
         * @param   chr - chromosome by the comparison
         * @return  true - if chromosme chr is equal to this chromosome
        **/
        bool operator ==(const CChromosome& chr) const;
        /**
         * @brief   Getting value of gene
         * @param   locus - position of gene
         * @return  value of gene
        **/
        bool getGene(unsigned int locus) const;
        /**
         * @brief   Setting value of gene
         * @param   locus - position of gene
         * @return  value of gene
        **/
        void setGene(unsigned int   locus,
                     bool           value);
        /**
         * @brief   Inverting value of gene
         * @param   locus - position of gene
        **/
        void invertGene(unsigned int locus);
        /**
         * @brief   Comparing current chromosome with chromosome chr
         * @param   true, if chromosome chr is better current cromosome
        **/
        bool operator<(const CChromosome& chr)const;
    private:
        static unsigned int m_bit_size;      ///<! Length of chromosome
        static unsigned int m_byte_size;     ///<! Number of memory byte
        static IFitness*    m_function;      ///<! Fitness function poiner
        unsigned char*      m_data;          ///<! Values of chromosome genes
    };
};
#endif // CCHROMOSOME_H_INCLUDED
