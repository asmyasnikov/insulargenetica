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
 * @file    CChromosome.h
 * @brief   Файл содержит класс CChromosome,
 *          который является аналогом хромосомы
 *          в генетическом программировании
 * @date    20/02/2009
 * @version 1.15
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
         * @brief   Установить размер хромосомы
         *          Допускается только один раз за сеанс
         *          устанавливать размер хромосомы
         * @param   size - размер хромосомы
        **/
        static void setSize(unsigned int size);
        /**
         * @brief   Получить размер хромосомы
         * @return  Размер хромосомы
        **/
        static unsigned int size();
        /**
         * @brief   Установить указатель на целевую фунцию для расчета
         *          здоровья хромосомы.
         * @param   function - указатель на целевую функцию
        **/
        static void setFitnessFunction(IFitness* function);
        /**
         * @brief   Конструктор по умолчанию
         *          Хромосома инициализируется случайными значениями
        **/
        CChromosome(void);
        /**
         * @brief   Конструктор
         *          Хромосома инициализируется значениями def
         * @param   def - значение для инициализации хромосомы
        **/
        CChromosome(bool def);
        /**
         * @brief   Конструктор копирования
         *          Производится глубокое копирование данных
         * @param   chr - хромосома, из которой происходит
         *          конструирование текущей хромосомы
        **/
        CChromosome(const CChromosome& chr);
        /**
         * @brief   Оператор копирования
         *          Производится глубокое копирование данных
         * @param   chr - хромосома, из которой происходит
         *          конструирование текущей хромосомы
        **/
        CChromosome&operator=(const CChromosome& chr);
        /**
         * @brief   Деструктор
        **/
        ~CChromosome();
        /**
         * @brief   Метод открытия транзакции
         *          При открытых транзакциях расчет здоровья не производится
        **/
        void begin();
        /**
         * @brief   Метод закрытия транзакции
         *          При открытых транзакциях расчет здоровья не производится
        **/
        void commit();
        /**
         * @brief   Оператор сравнения двух хромосом
         * @param   chr - хромосома, с которой нужно сравнить
         * @return  true - если хромосомы равны
        **/
        bool operator ==(const CChromosome& chr) const;
        /**
         * @brief   Метод получения значения гена
         * @param   locus - позиция гена
         * @return  значение гена
        **/
        bool getGene(unsigned int locus) const;
        /**
         * @brief   Метод установления значения гена
         * @param   locus - позиция гена
         * @param   value - значение гена
        **/
        void setGene(unsigned int   locus,
                     bool           value);
        /**
         * @brief   Метод инвертирования значения гена
         * @param   locus - позиция гена
        **/
        void invertGene(unsigned int locus);
        /**
         * @brief   Метод получения значения здоровья хромосомы
         * @return  Здоровье хромосомы
        **/
        double fitness() const;
    private:
        static unsigned int m_bit_size;      ///<! Длина хромосомы
        static unsigned int m_byte_size;     ///<! Количество байт памяти
        static IFitness*    m_function;      ///<! Функция расчета здоровья
        unsigned char*      m_data;          ///<! Хранилище данных
        double              m_fitness;       ///<! Значение функции здоровья
        int                 m_transactions;  ///<! Кол-во открытых транзакций
    };
};
#endif // CCHROMOSOME
