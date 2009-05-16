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
 * @file    CChromosome.cpp
 * @brief   Файл содержит класс CChromosome,
 *          который является аналогом хромосомы
 *          в генетическом программировании
 * @date    20/02/2009
 * @version 1.15
**/
#include "../include/CChromosome.h"
#include <stdlib.h>
unsigned int                InsularGenetica::CChromosome::m_bit_size  = 0;
unsigned int                InsularGenetica::CChromosome::m_byte_size = 0;
InsularGenetica::IFitness*  InsularGenetica::CChromosome::m_function  = 0;
/**
 * @brief   Установить указатель на целевую фунцию для расчета
 *          здоровья хромосомы.
 * @param   function - указатель на целевую функцию
**/
void
InsularGenetica::
CChromosome::
setFitnessFunction(IFitness* function)
{
    m_function = function;
};
/**
 * @brief   Установить размер хромосомы
 *          Допускается только один раз за сеанс
 *          устанавливать размер хромосомы
 * @param   size - размер хромосомы
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
 * @brief   Получить размер хромосомы
 * @return  Размер хромосомы
**/
unsigned int
InsularGenetica::
CChromosome::
size()
{
    return m_bit_size;
};
/**
 * @brief   Конструктор
**/
InsularGenetica::
CChromosome::
CChromosome(void) :
    m_transactions(1)
{
    Q_ASSERT(m_byte_size);
    Q_ASSERT(m_function);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, rand()%2);
    }
    commit();
};
/**
 * @brief   Конструктор
 *          Хромосома инициализируется значениями def
 * @param   def - значение для инициализации хромосомы
**/
InsularGenetica::
CChromosome::
CChromosome(bool def) :
    m_transactions(1)
{
    Q_ASSERT(m_byte_size);
    Q_ASSERT(m_function);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, def);
    }
    commit();
};
/**
 * @brief   Конструктор копирования
 *          Производится глубокое копирование данных
 * @param   chr - хромосома, из которой происходит
 *          конструирование текущей хромосомы
**/
InsularGenetica::
CChromosome::
CChromosome(const CChromosome& chr) :
    m_transactions(chr.m_transactions+1)
{
    Q_ASSERT(m_byte_size);
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, chr.getGene(i));
    }
    m_fitness = chr.fitness();
    // Так как здоровье уже рассчитано у копируемой хромосомы,
    // то вместо commit() просто уменьшим счетчик транзакций;
    m_transactions--;
};
/**
 * @brief   Оператор копирования
 *          Производится глубокое копирование данных
 * @param   chr - хромосома, из которой происходит
 *          конструирование текущей хромосомы
**/
InsularGenetica::
CChromosome&
InsularGenetica::
CChromosome::
operator=(const CChromosome& chr)
{
    Q_ASSERT(m_byte_size);
    m_transactions = chr.m_transactions+1;
    m_data = new unsigned char[m_byte_size];
    for(unsigned int i = 0; i < m_bit_size; ++i)
    {
        setGene(i, chr.getGene(i));
    }
    m_fitness = chr.fitness();
    // Так как здоровье уже рассчитано у копируемой хромосомы,
    // то вместо commit() просто уменьшим счетчик транзакций;
    m_transactions--;
    return *this;
};
/**
 * @brief   Деструктор
**/
InsularGenetica::
CChromosome::
~CChromosome()
{
    delete[] m_data;
};
/**
 * @brief   Метод открытия транзакции
 *          При открытых транзакциях расчет здоровья не производится
**/
void
InsularGenetica::
CChromosome::
begin()
{
    m_transactions++;
};
/**
 * @brief   Метод закрытия транзакции
 *          При открытых транзакциях расчет здоровья не производится
**/
void
InsularGenetica::
CChromosome::
commit()
{
    m_transactions--;
    if(!m_transactions)
    {
        m_fitness = m_function->calc(*this);
    }
};
/**
 * @brief   Оператор сравнения двух хромосом
 * @param   chr - хромосома, с которой нужно сравнить
 * @return  true - если хромосомы равны
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
 * @brief   Метод получения значения гена
 * @param   locus - позиция гена
 * @return  значение гена
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
 * @brief   Метод установления значения гена
 * @param   locus - позиция гена
 * @param   value - значение гена
**/
void
InsularGenetica::
CChromosome::
setGene(unsigned int locus,
        bool value)
{
    begin();
    Q_ASSERT(locus < m_bit_size);
    unsigned int byte=locus/8;
    unsigned int disposal=locus-byte*8;
    m_data[byte]=((m_data[byte]|(1<<(7-disposal)))^(1<<(7-disposal)))|
                 (((unsigned char)value)<<(7-disposal));
    commit();
};
/**
 * @brief   Метод инвертирования значения гена
 * @param   locus - позиция гена
**/
void
InsularGenetica::
CChromosome::
invertGene(unsigned int locus)
{
    begin();
    Q_ASSERT(locus < m_bit_size);
    unsigned int byte=locus/8;
    m_data[byte]^=(1<<(7-locus+byte*8));
    commit();
};
/**
 * @brief   Метод получения значения здоровья хромосомы
 * @return  Здоровье хромосомы
**/
double
InsularGenetica::
CChromosome::
fitness() const
{
    Q_ASSERT(!m_transactions);
    return m_fitness;
};
