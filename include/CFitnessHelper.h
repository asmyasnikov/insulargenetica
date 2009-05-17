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
 * @file    CFitnessHelper.h
 * @brief   Файл содержит реализацию вспомогательных методов,
 *          необходимых при расчете целевой функции
 * @date    20/02/2009
 * @version 1.15
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
         * @brief   Базовый конструктор
        **/
        CFitnessHelper(unsigned int count);
        /**
         * @brief   Деструктор
        **/
        ~CFitnessHelper();
        /**
         * @brief   Метод получения количества рассчитанных целевых функций
         * @return  Количество рассчитанных целевых функций
        **/
        unsigned int count();
        /**
         * @brief   Метод перекодирования генотипа в фенотип
         * @return  Фенотип
         **/
#if QT_VERSION < 0x040000
        QValueList<double>
#else
        QList<double>
#endif
        genotype2phenotype(const CChromosome& chr);
    protected:
        unsigned int    m_count;  ///<! Количество аргументов функции
        unsigned int    m_counter;///<! Счетчик обращений к функции
        /** @brief  Метод для преобразования хромосомы (части хромосомы)
                    к типу double. Перед использованием этого метода
                    переменная m_count обязательно должна быть
                    инициализирована ненулевым положительным значеним.
            @param  chr     - хромосома,
            @param  index   - номер переменной,
            @return значение в диапазоне от 0 до 1, соответствующее
                    участку хромосомы
        **/
        double decode(const CChromosome& chr,
                      unsigned int       index) const;

    protected:
        double min;
        double max;
    };
};
#endif // C_FITNESS_
