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
 * @file    COutBriding.h
 * @brief   Файл содержит класс COutBriding отбора родительских хромосом
 * @date    20/02/2009
**/
#ifndef C_INBRIDING_H_INCLUDED
#define C_INBRIDING_H_INCLUDED
#include "../../../idl/IGrouping.h"
#include "../../../include/CPopulation.h"
#include "../../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
    #include <qvaluevector.h>
    #include <qpair.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
    #include <QtCore/QVector>
    #include <QtCore/QPair>
#endif
namespace GeneticAlgorithm
{
    struct COutBriding : virtual public IGrouping
    {
        /**
         * @brief   Базовый конструктор
        **/
        COutBriding(){};
        /**
         * @brief   Деструктор
        **/
        ~COutBriding(){};
        /**
         * @brief  Метод отбора пар родительских хромосом для скрещивания
         * @param  sel - популяция родителей для скрещивания
         * @return par - набор пар родителей
        **/
        void group(const CPopulation&sel, CParents&par)
        {
            Q_ASSERT(sel.size());
#if QT_VERSION < 0x040000
            QValueVector<QPair<double, QPair<const CChromosome*,
                                             const CChromosome*> > >h_matrix;
#else
            QVector<QPair<double, QPair<const CChromosome*,
                                        const CChromosome*> > >h_matrix;
#endif
            for(int i = 0; i < sel.size(); i++)
            {
                for(int j = i+1; j < sel.size(); j++)
                {
                    const CChromosome*first  = &sel.getChromosome(i);
                    const CChromosome*second = &sel.getChromosome(j);
                    if((*first) == (*second)) continue;
                    double homogeinity = 0.;
                    for(unsigned int k = 0; k < CChromosome::size(); k++)
                    {
                        if(first->getGene(k) == second->getGene(k))
                        {
                            homogeinity += 1.;
                        }
                    }
                    h_matrix.push_back(qMakePair(homogeinity,
                                                 qMakePair(first,second)));
                }
            }
            for(int i = 0; i < h_matrix.size(); i++)
            {
                for(int j = i+1; j < h_matrix.size(); j++)
                {
                    if(h_matrix[i].first > h_matrix[j].first)
                    {
                        QPair<double, QPair<const CChromosome*,
                                            const CChromosome*> > temp =
                             h_matrix[j];
                        h_matrix[j] = h_matrix[i];
                        h_matrix[i] = temp;
                    }
                }
            }
            for(int i = 0; i < qMin(h_matrix.size(), sel.size()*2); i++)
            {
                par.append(h_matrix[i].second);
            }
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Аутбридинг (группировка "
                                   "дальних хромосом)");
        };
    };
};
using namespace GeneticAlgorithm;
#endif // C_INBRIDING_H_INCLUDED
