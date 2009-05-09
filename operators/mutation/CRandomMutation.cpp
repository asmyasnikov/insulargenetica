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
** Обращаю Ваше внимание на то, что библиотека InsularGenetica
** зарегистрирована Российским агенством по патентам и товарным знакам
** (РОСПАТЕНТ), о чем выдано "Свидетельство об официальной регистрации
** программы для ЭВМ" за № FIXME от FIXME FIXME FIXME года. Копия
** свидетельства о регистрации представлена в файле CERTIFICATE
** в корне проекта.
** Это не накладывает на конечных разработчиков/пользователей никаких
** дополнительных ограничений, кроме предусмотренных GNU Lesser GPL,
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКЕ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CRandomMutation.cpp
 * @brief   Файл содержит класс CRandomMutation отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.15
**/
#include "CRandomMutation.h"
#include <math.h>
/**
 * @brief   Базовый конструктор
**/
InsularGenetica::
CRandomMutation::
CRandomMutation(double percentage) :
    m_percentage(int(double(RAND_MAX)*percentage))
{};
/**
 * @brief   Деструктор
**/
InsularGenetica::
CRandomMutation::
~CRandomMutation(){};
/**
 * @brief  Метод "рождения" мутированных потомков
 * @param  chr  - родительская хромосома, из которой "рождается" мутант
 * @return cids - популяция потомков
**/
void
InsularGenetica::
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
InsularGenetica::
CRandomMutation::
name()
{
    return QObject::trUtf8("%1-процентная случайная мутация").arg(ceil(m_percentage / double(RAND_MAX) * 100. - 0.01));
};
