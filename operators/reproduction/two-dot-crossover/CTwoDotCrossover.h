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
 * @file    CTwoDotCrossover.h
 * @brief   Файл содержит класс CTwoDotCrossover отбора родительских хромосом
 * @date    20/02/2009
**/

#ifndef C_TWO_DOT_CROSSOVER_H_INCLUDED
#define C_TWO_DOT_CROSSOVER_H_INCLUDED

#include "../../../idl/IReproduction.h"
#include "../../../include/CPopulation.h"
#include "../../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif

namespace GeneticAlgorithm
{
    struct CTwoDotCrossover : virtual public IReproduction
    {
        /**
         * @brief   Базовый конструктор
        **/
        CTwoDotCrossover(){};
        /**
         * @brief   Деструктор
        **/
        ~CTwoDotCrossover(){};

        /**
         * @brief  Метод получения из двух родителей одного или нескольких потомков путем скрещивания
         * @param  pair - пара родителей, из которых "рождаются" потомки
         * @return cids - популяция потомков
        **/
        void reproduct(const QPair<const CChromosome*,const CChromosome*>pair, CPopulation&cids)
        {
            Q_ASSERT(CChromosome::size() > 1);
            CChromosome child1(*pair.first);
            CChromosome child2(*pair.second);
            unsigned int locus1 = 0;
            unsigned int locus2 = 0;
            while(locus1 == locus2)
            {
                locus1 = qMax(1, qMin(int(CChromosome::size()) - 2, rand() % int(CChromosome::size())));
                locus2 = qMax(1, qMin(int(CChromosome::size()) - 2, rand() % int(CChromosome::size())));
            }
            if(locus1 > locus2)
            {
                unsigned int temp = locus1;
                locus1            = locus2;
                locus2            = temp;
            }
            child1.begin();
            child2.begin();
            for(unsigned int i = locus1; i < locus2; i++)
            {
                child1.setGene(i, pair.second->getGene(i));
                child2.setGene(i, pair.first->getGene(i));
            }
            child1.commit();
            cids.addChromosome(child1);
            child2.commit();
            cids.addChromosome(child2);
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Двухточечный кроссинговер");
        };
    };
};
using namespace GeneticAlgorithm;
#endif // C_TWO_DOT_CROSSOVER_H_INCLUDED
