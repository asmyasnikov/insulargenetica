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
 * @file    CGriewank.h
 * @brief   Файл содержит класс CGriewank,
 *          который реализует тестовую целевую функцию CGriewank.
 * @date    04/03/2009
**/

#ifndef C_GRIEWANK_MODULE_H_INCLUDED
#define C_GRIEWANK_MODULE_H_INCLUDED

#include "../../include/export.h"
#include "../../include/CFitnessHelper.h"
#include "../../idl/IFitness.h"
#include "../../include/CChromosome.h"

class QString;

using namespace GeneticAlgorithm;

Q_DECL_EXPORT struct CGriewank : virtual public IFitness,
                                 virtual public CFitnessHelper
{
    /**
     * @brief   Базовый конструктор
    **/
    CGriewank();
    /**
     * @brief   Конструктор
     * @param   count - количество переменных целевой функции
    **/
    CGriewank(unsigned int count, bool invert = false);
    /**
     * @brief   Деструктор
    **/
    ~CGriewank();
    /**
     * @brief   Метод вычисления значения целевой функции
     * @return  значение функции
    **/
    double calc(const CChromosome& chr);
    /**
     * @brief   Метод получения наименования функции
     * @return  наименование функции
    **/
    const QString name();
private:
    bool            m_invert;
};

#endif // C_GRIEWANK_MODULE_H_INCLUDED
