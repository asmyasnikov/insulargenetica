/****************************************************************************
** Copyright (C) 2009 Мясников Алексей Сергеевич.
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
****************************************************************************/
/**
 * @file    CPopulation.h
 * @brief   Файл содержит класс CPopulation,
 *          который является аналогом популяции
 *          в генетическом программировании
 * @date    13/02/2009
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

namespace GeneticAlgorithm
{
    Q_DECL_EXPORT struct CPopulation : virtual public Interface
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
         * @param  pseudo - параметр позволяет рассчитать при (pseudo == true)
         *         псвдооднородность популяции. Псвдооднородность популяции
         *         рассчитывается с учетом незначительности отдельных несовпадений
         *         в генах приподавляющем большинстве совпадений.
         * @return Стенень однородности популяции в диапазоне от 0 до 1
         * @return Стенень псевдооднородности популяции в диапазоне от 0.5 до 1
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
        mutable QMutex        m_mutex;     ///<! Блокатор доступа, обеспечивающий атомарность методов
    };
};
#endif // CPOPULATION_H_INCLUDED
