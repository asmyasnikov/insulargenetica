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
 * @file    CRandomGrouping.h
 * @brief   Файл содержит класс CRandomGrouping отбора родительских хромосом
 * @date    20/02/2009
 * @version 1.14
**/
#ifndef C_RANDOM_GROUPING_H_INCLUDED
#define C_RANDOM_GROUPING_H_INCLUDED
#include "../../idl/IGrouping.h"
#include "../../include/CPopulation.h"
#include "../../include/CChromosome.h"
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
namespace InsularGenetica
{
    struct CRandomGrouping : virtual public IGrouping
    {
        /**
         * @brief   Базовый конструктор
        **/
        CRandomGrouping(){};
        /**
         * @brief   Деструктор
        **/
        ~CRandomGrouping(){};
        /**
         * @brief  Метод отбора пар родительских хромосом для скрещивания
         * @param  sel - популяция родителей для скрещивания
         * @return par - набор пар родителей
        **/
        void group(const CPopulation&sel, CParents&par)
        {
            if(sel.size()>1)
            {
                unsigned int size_new = sel.size()/2;
                for(unsigned int i = 0; i < size_new; i++)
                {
                    unsigned int sel1 = 0;
                    unsigned int sel2 = 0;
                    while(sel1 == sel2)
                    {
                            sel1 = rand()%sel.size();
                            sel2 = rand()%sel.size();
                    }
                    par.append(qMakePair(&sel.getChromosome(sel1),
                                             &sel.getChromosome(sel2)));
                }
            }
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Группировка всех хромосом со всеми");
        };
    };
};

#endif // C_RANDOM_GROUPING_H_INCLUDED

