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
 * @file    IFitness.h
 * @brief   Файл содержит интерфейс IFitnes,
 *          который обеспечивает единообразие
 *          доступа к целевым функциям
 * @date    20/02/2009
**/

#ifndef FITNESS_INTERFACE
#define FITNESS_INTERFACE

#include "Interface.h"

class QString;

namespace GeneticAlgorithm
{
    class CChromosome;

    struct IFitness : virtual public Interface
    {
        /**
         * @brief   Метод вычисления значения целевой функции
         * @return  значение функции
        **/
        virtual double calc(const CChromosome& chr) = 0;
        /**
         * @brief   Метод получения наименования функции
         * @return  наименование функции
        **/
        virtual const QString name() = 0;
        /**
         * @brief   Метод получения количества рассчитанных целевых функций
         * @return  Количество рассчитанных целевых функций
        **/
        virtual unsigned int count() = 0;
    };
};
#endif // FITNESS_INTERFACE
