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
 * @file    CRandomSelection.h
 * @brief   Файл содержит класс CRandomSelection отбора родительских хромосом
 * @date    20/02/2009
**/

#ifndef C_RANDOM_SELECTION_H_INCLUDED
#define C_RANDOM_SELECTION_H_INCLUDED

#include "../../../idl/ISelection.h"
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
    struct CRandomSelection : virtual public ISelection
    {
        /**
         * @brief   Базовый конструктор
        **/
        CRandomSelection(){};
        /**
         * @brief   Деструктор
        **/
        ~CRandomSelection(){};

        /**
         * @brief  Метод отбора из популяции хромосом для дальнейшего скрещивания и мутации
         * @param  pop - популяция родителей, из которых производится отбор
         * @return sel - популяция родителей для скрещивания и мутаций
        **/
        void select(const CPopulation&pop, CPopulation&sel)
        {
            Q_ASSERT(pop.size());
            for(int i = 0; i < pop.size(); i++)
            {
                sel.addChromosome(pop.getChromosome(rand() % pop.size()));
            }
            Q_ASSERT(sel.size() > 1);
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Случайный отбор");
        };
    };
};
using namespace GeneticAlgorithm;
#endif // C_RANDOM_SELECTION_H_INCLUDED

