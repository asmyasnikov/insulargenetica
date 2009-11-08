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
 * @file    CChromosome.cpp
 * @class   CChromosome
 * @brief   This is the class CChromosome. It implement chromosome
 *          in genetic programming
 * @author  Aleksey Myasnikov
 * @date    20/02/2009
 * @version 3.3
**/
#include "../include/CChromosome.h"
#include <stdlib.h>
unsigned int                InsularGenetica::CChromosome::m_bit_size  = 0;
unsigned int                InsularGenetica::CChromosome::m_byte_size = 0;
InsularGenetica::IFitness*  InsularGenetica::CChromosome::m_function  = 0;
/**
 * @brief   Setting static pointer of fitness function
 * @param   function - pointer of fitness function
**/
void
InsularGenetica::
CChromosome::
setFitnessFunction(IFitness* function)
{
    m_function = function;
};
/**
 * @brief   Setting size of chromosome
 * @param   size - size of chromosome
**/
void
InsularGenetica::
CChromosome::
setSize(unsigned int size)
{
    m_bit_size = size;
    m_byte_size = ((m_bit_size/8)+
                   ((m_bit_size-((m_bit_size/8)*8))>0?1:0));
};
/**
 * @brief   Getting size of chromosome
 * @return  size of chromosome
**/
unsigned int
InsularGenetica::
CChromosome::
size()
{
    return m_bit_size;
};
/**
 * @brief   Constructor
**/
InsularGenetica::
CChromosome::
CChromosome(void)
{
    Q_ASSERT(m_byte_size);
    Q_ASSERT(m_function);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, rand()%2);
    }
};
/**
 * @brief   Constructor with initialization by def-values
 * @param   def - value of chromosome genes
**/
InsularGenetica::
CChromosome::
CChromosome(bool def)
{
    Q_ASSERT(m_byte_size);
    Q_ASSERT(m_function);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, def);
    }
};
/**
 * @brief   Copy constructor
 * @param   chr - source chromosome
**/
InsularGenetica::
CChromosome::
CChromosome(const CChromosome& chr)
{
    Q_ASSERT(m_byte_size);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, chr.getGene(i));
    }
};
/**
 * @brief   Copy constructor
 * @param   chr - source chromosome
**/
InsularGenetica::
CChromosome&
InsularGenetica::
CChromosome::
operator=(const CChromosome& chr)
{
    Q_ASSERT(m_byte_size);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, chr.getGene(i));
    }
    return *this;
};
/**
 * @brief   Destructor
**/
InsularGenetica::
CChromosome::
~CChromosome()
{
    delete[] m_data;
};
/**
 * @brief   The comparison of two chromosomes
 * @param   chr - chromosome by the comparison
 * @return  true - if chromosme chr is equal to this chromosome
**/
bool
InsularGenetica::
CChromosome::
operator ==(const CChromosome& chr) const
{
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        if(getGene(i) != chr.getGene(i)) return false;
    }
    return true;
};
/**
 * @brief   Getting value of gene
 * @param   locus - position of gene
 * @return  value of gene
**/
bool
InsularGenetica::
CChromosome::
getGene(unsigned int locus) const
{
    Q_ASSERT(locus < m_bit_size);
    unsigned int byte=locus/8;
    return (m_data[byte]>>(7-locus+byte*8))&1;
};
/**
 * @brief   Setting value of gene
 * @param   locus - position of gene
 * @param   value - new value of gene
**/
void
InsularGenetica::
CChromosome::
setGene(unsigned int locus,
        bool value)
{
    Q_ASSERT(locus < m_bit_size);
    unsigned int byte=locus/8;
    unsigned int disposal=locus-byte*8;
    m_data[byte]=((m_data[byte]|(1<<(7-disposal)))^(1<<(7-disposal)))|
                 (((unsigned char)value)<<(7-disposal));
};
/**
 * @brief   Inverting value of gene
 * @param   locus - position of gene
**/
void
InsularGenetica::
CChromosome::
invertGene(unsigned int locus)
{
    Q_ASSERT(locus < m_bit_size);
    unsigned int byte=locus/8;
    m_data[byte]^=(1<<(7-locus+byte*8));
};
/**
 * @brief   Comparing current chromosome with chromosome chr
 * @param   true, if chromosome chr is better current cromosome
**/
bool
InsularGenetica::
CChromosome::
operator<(const CChromosome& chr)const
{
    return m_function->compare(chr,*this);
};
