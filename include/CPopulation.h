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
 * @file    CPopulation.h
 * @brief   Файл содержит класс CPopulation,
 *          который является аналогом популяции
 *          в генетическом программировании
 * @date    13/02/2009
 * @version 1.15
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
         * @brief   Конструктор
         * @param   size - размер популяции
        **/
        CPopulation(int size = 0);
        /**
         * @brief   Конструктор копирования
         *          Производится глубокое копирование данных
         * @param   pop - популяция, из которой происходит
         *          конструирование текущей популяции
        **/
        CPopulation(const CPopulation& pop);
        /**
         * @brief   Оператор копирования
         *          Производится глубокое копирование данных
         * @param   pop - популяция, из которой происходит
         *          конструирование текущей популяции
        **/
        CPopulation& operator=(const CPopulation& pop);
        /**
         * @brief   Деструктор
        **/
        ~CPopulation();
        /**
         * @brief  Размер хромосомы
         * @return Размер хромосомы
        **/
        int size() const;
        /**
         * @brief  Получить хромосому
         * @return Хромосома
        **/
        const CChromosome& getChromosome(int index) const;
        /**
         * @brief  Добавить хромосому в популяцию
         * @param  chr - хромосома
        **/
        void addChromosome(const CChromosome& chr);
        /**
         * @brief  Рассчитать среднее здоровье популяции
         * @return Среднее здоровье популяции
        **/
        double getAverageFitness() const;
        /**
         * @brief  Рассчитать максимальное здоровье в популяции
         * @return Максимальное здоровье популяции
        **/
        double getMaximumFitness() const;
        /**
         * @brief  Рассчитать минимальное здоровье в популяции
         * @return Минимальное здоровье популяции
        **/
        double getMinimumFitness() const;
        /**
         * @brief  Рассчитать стенень однородности популяции
         * @param  pseudo - параметр позволяет рассчитать при (pseudo==true)
         *         псвдооднородность популяции. Псвдооднородность популяции
         *         рассчитывается с учетом незначительности отдельных
         *         несовпадений в генах приподавляющем большинстве совпадений
         * @return Стенень однородности популяции от 0 до 1
         * @return Стенень псевдооднородности популяции от 0.5 до 1
        **/
        double getHomogeneity(bool pseudo = false) const;
        /**
         * @brief  Заменить в популяции худшую хромосому
         * @param  chr - хромосома для замены
        **/
        void replaceChromosome(const CChromosome&chr);
        /**
         * @brief  Проверить, есть ли в популяции такая же хромосома
         * @param  chr - хромосома для проверки
        **/
        bool isPresent(const CChromosome&chr) const;
    private:
#if QT_VERSION < 0x040000
        QValueVector<CChromosome*> m_data; ///<! Хромосомы в популяции
#else
        QVector<CChromosome*> m_data;      ///<! Хромосомы в популяции
#endif
        ///<! Блокатор доступа, обеспечивающий атомарность методов
        mutable QMutex        m_mutex;
    };
};
#endif // CPOPULATION_H_INCLUDED
