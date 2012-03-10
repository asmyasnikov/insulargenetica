/****************************************************************************
** Copyright (C) 2009 Мясников А.С. Сергеевич.
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
** программы для ЭВМ" за N 2010610175 от 11.01.2010 г. Копия
** свидетельства о регистрации представлена в файле CERTIFICATE
** в корне проекта.
** Это не накладывает на конечных разработчиков/пользователей никаких
** дополнительных ограничений, кроме предусмотренных GNU Lesser GPL,
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКЕ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CRouletteSelection.h
 * @brief   Файл содержит класс CRouletteSelection отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.14
**/
#ifndef C_ROULETTE_SELECTION_H_INCLUDED
#define C_ROULETTE_SELECTION_H_INCLUDED
#include "../../idl/ISelection.h"
#include "../../include/CPopulation.h"
#include "../../include/CChromosome.h"
#include <qglobal.h>
#include <stdlib.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif
namespace InsularGenetica
{
    struct CRouletteSelection : virtual public ISelection
    {
        /**
         * @brief   Базовый конструктор
        **/
        CRouletteSelection(){};
        /**
         * @brief   Деструктор
        **/
        ~CRouletteSelection(){};
        /**
         * @brief  Метод отбора из популяции хромосом для дальнейшего
         *         скрещивания и мутации
         * @param  pop - популяция родителей, из которых производится отбор
         * @return sel - популяция родителей для скрещивания и мутаций
        **/
        void select(const CPopulation&pop, CPopulation&sel)
        {
            Q_ASSERT(pop.size() > 1);
            double miminum = pop.getMinimumFitness();
            double summary = 0.;
            for(uint j = 0; j < pop.size(); j++)
            {
                summary += (pop.getChromosome(j).fitness() - miminum);
            }
            for(uint i = 0; i < pop.size(); i++)
            {
                double number     = double(rand()) /
                                    double(RAND_MAX) * summary;
                double accumulate = 0.;
                for(uint j = 0; j < pop.size(); j++)
                {
                    accumulate += (pop.getChromosome(j).fitness() - miminum);
                    if(accumulate > number)
                    {
                        sel.addChromosome(pop.getChromosome(j));
                        break;
                    }
                }
            }
            int i = 0;
            while(sel.size() < 2)
            {
                sel.addChromosome(pop.getChromosome(i % pop.size()));
                i++;
            }
            Q_ASSERT(sel.size() > 1);
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Рулеточный отбор");
        };
    };
};

#endif // C_ROUL

