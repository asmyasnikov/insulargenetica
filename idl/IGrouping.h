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
 * @file    IGrouping.h
 * @brief   Файл содержит интерфейс IGrouping отбора родительских пар
 * @date    17/02/2009
**/

#ifndef INTERFACE_GROUPING_H_INCLUDED
#define INTERFACE_GROUPING_H_INCLUDED

#include "IGeneticOperator.h"
#include "../include/CPopulation.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qlist.h>
    #include <qpair.h>
#else
    #include <QtCore/QList>
    #include <QtCore/QPair>
#endif

namespace GeneticAlgorithm
{
    #if QT_VERSION < 0x040000
    typedef QValueList<QPair<const CChromosome*, const CChromosome*> >CParents;
    #else
    typedef QList<QPair<const CChromosome*, const CChromosome*> >CParents;
    #endif

    struct IGrouping : virtual public IGeneticOperator
    {
        /**
         * @brief  Метод отбора пар родительских хромосом для дальнейшего скрещивания
         * @param  sel - популяция родителей для скрещивания
         * @return par - набор пар родителей
        **/
        virtual void group( const CPopulation&  sel,
                            CParents&           par) = 0;
    };
};
#endif // INTERFACE_GROUPING_H_INCLUDED
