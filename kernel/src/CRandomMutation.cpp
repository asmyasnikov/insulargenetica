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
 * @file    CRandomMutation.cpp
 * @brief   Файл содержит класс CRandomMutation отбора родительских хромосом
 * @date    20/02/2009
**/

#include "../../include/CRandomMutation.h"
#include <math.h>

/**
 * @brief   Базовый конструктор
**/
GeneticAlgorithm::
CRandomMutation::
CRandomMutation(double percentage) :
    m_percentage(int(double(RAND_MAX)*percentage))
{};
/**
 * @brief   Деструктор
**/
GeneticAlgorithm::
CRandomMutation::
~CRandomMutation(){};

/**
 * @brief  Метод "рождения" мутированных потомков
 * @param  chr  - родительская хромосома, из которой "рождается" мутированный потомок
 * @return cids - популяция потомков
**/
void
GeneticAlgorithm::
CRandomMutation::
mutate( const CChromosome&chr,
        CPopulation&cids)
{
    Q_ASSERT(CChromosome::size());
    CChromosome child(chr);
    child.begin();
    for(unsigned int i = 0; i < CChromosome::size(); i++)
    {
        if(rand() < m_percentage) child.invertGene(i);
    }
    child.commit();
    cids.addChromosome(child);
};

/**
 * @brief   Метод получения наименования генетического оператора
 * @return  наименование генетического оператора
**/
const
QString
GeneticAlgorithm::
CRandomMutation::
name()
{
    return QObject::trUtf8("%1-процентная случайная мутация").arg(ceil(m_percentage / double(RAND_MAX) * 100. - 0.01));
};
