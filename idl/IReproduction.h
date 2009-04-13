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
 * @file    IReproduction.h
 * @brief   Файл содержит интерфейс IReproduction создания потомков из родительской пары
 * @date    17/02/2009
**/

#ifndef INTERFACE_REPRODUCTION_H_INCLUDED
#define INTERFACE_REPRODUCTION_H_INCLUDED

#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qpair.h>
#else
    #include <QtCore/QPair>
#endif
#include "IGeneticOperator.h"
#include "../include/CChromosome.h"
#include "../include/CPopulation.h"

namespace GeneticAlgorithm
{
    struct IReproduction : virtual public IGeneticOperator
    {
        /**
         * @brief  Метод получения из двух родителей одного или нескольких потомков путем скрещивания
         * @param  pair - пара родителей, из которых "рождаются" потомки
         * @return cids - популяция потомков
        **/
        virtual void reproduct( const QPair<const CChromosome*,const CChromosome*>  pair,
                                CPopulation&                                        cids) = 0;
    };
};
#endif // INTERFACE_REPRODUCTION_H_INCLUDED
