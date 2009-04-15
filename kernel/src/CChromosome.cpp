/****************************************************************************
** Copyleft (C) 2009 Мясников Алексей Сергеевич.
** Contact: AlekseyMyasnikov@yandex.ru
**          amyasnikov@npomis.ru
**          AlekseyMyasnikov@mail.ru
**          MyasnikovAleksey@mail.ru
** Этот файл является частью реализации библиотеки островного генетического
** алгоритма с динамическим распределением вероятностей выбора генетических
** операторов в каждой группе операторов
** Данная библиотека является свободным программным обеспечением. Вы вправе
** распространять её и/или модифицировать в соответствии с условиями версии 3
** либо по вашему выбору с условиями более поздней версии Стандартной
** Общественной Лицензии Ограниченного Применения GNU, опубликованной
** Free Software Foundation.
** Я распространяю эту библиотеку в надежде на то, что она будет вам
** полезной, однако НЕ ПРЕДОСТАВЛЯЮ НА НЕЕ НИКАКИХ ГАРАНТИЙ, в том числе
** ГАРАНТИИ ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и ПРИГОДНОСТИ ДЛЯ ИСПОЛЬЗОВАНИЯ
** В КОНКРЕТНЫХ ЦЕЛЯХ. Для получения более подробной информации ознакомьтесь
** со Стандартной Общественной Лицензией Ограниченного Применений GNU в
** файле LICENSE в корне исходных текстов проекта или по адресу:
** http://www.gnu.org/copyleft/lgpl.html.
** Обращаю Ваше внимание на то, что библиотека InsularGenetica
** зарегистрирована Российским агенством по патентам и товарным знакам
** (РОСПАТЕНТ), о чем выдано "Свидетельство об официальной регистрации
** программы для ЭВМ" за № FIXME от FIXME FIXME FIXME года. Копия
** свидетельства о регистрации представлена в файле CERTIFICATE
** в корне проекта.
** Это не накладывает на конечных разработчиков/пользователей никаких
** дополнительных ограничений, кроме предусмотренных GNU Lesser GPL,
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКАХ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CChromosome.cpp
 * @brief   Файл содержит класс CChromosome,
 *          который является аналогом хромосомы
 *          в генетическом программировании
 * @date    20/02/2009
**/
#include "../../include/export.h"
#include "../../include/CChromosome.h"
#include <stdlib.h>
unsigned int                GeneticAlgorithm::CChromosome::m_bit_size  = 0;
unsigned int                GeneticAlgorithm::CChromosome::m_byte_size = 0;
GeneticAlgorithm::IFitness* GeneticAlgorithm::CChromosome::m_function  = 0;
/**
 * @brief   Установить указатель на целевую фунцию для расчета
 *          здоровья хромосомы.
 * @param   function - указатель на целевую функцию
**/
void
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
CChromosome::
size()
{
    return m_bit_size;
};
/**
 * @brief   Конструктор
**/
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
CChromosome&
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
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
GeneticAlgorithm::
CChromosome::
fitness() const
{
    Q_ASSERT(!m_transactions);
    return m_fitness;
};
