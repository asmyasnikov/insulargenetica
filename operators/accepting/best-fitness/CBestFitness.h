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
 * @file    CBestFitness.h
 * @brief   Файл содержит класс CBestFitness отбора родительских хромосом
 * @date    20/02/2009
**/

#ifndef C_BEST_FITNESS_H_INCLUDED
#define C_BEST_FITNESS_H_INCLUDED

#include "../../../idl/IAccepting.h"
#include "../../../include/CPopulation.h"
#include "../../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif

namespace GeneticAlgorithm
{
    struct CBestFitness : virtual public IAccepting
    {
        /**
         * @brief   Базовый конструктор
        **/
        CBestFitness(){};
        /**
         * @brief   Деструктор
        **/
        ~CBestFitness(){};

        /**
         * @brief   Метод оценки пригодности хромосомы
         * @warning Здоровье хромосомы должно быть заранеее рассчитано
         * @param   pop - популяция родителей, относительно которых производится оценка пригодности
         * @param   cur - хромосома, которая оценивается на пригодности популяции
         * @return  true , если хромосома пригодна для популяции
         * @return  false, если хромосома не пригодна для популяции
        **/
        bool accept(const CPopulation*pop, const CChromosome&cur)
        {
            Q_ASSERT(CChromosome::size());
            return (pop->getMaximumFitness() < cur.fitness());
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Здоровье хромосомы лучше, чем лучшее в популяции");
        };
    };
};

using namespace GeneticAlgorithm;

#endif // C_BEST_FITNESS_H_INCLUDED

