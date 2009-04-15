/****************************************************************************
** Copyleft (C) 2009 Мясников Алексей Сергеевич.
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
 * @file    COneDotMutation.h
 * @brief   Файл содержит класс COneDotMutation отбора родительских хромосом
 * @date    20/02/2009
**/
#ifndef C_ONE_DOT_MUTATION_H_INCLUDED
#define C_ONE_DOT_MUTATION_H_INCLUDED
#include "../../../idl/IMutation.h"
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
    struct COneDotMutation : virtual public IMutation
    {
        /**
         * @brief   Базовый конструктор
        **/
        COneDotMutation(){};
        /**
         * @brief   Деструктор
        **/
        ~COneDotMutation(){};
        /**
         * @brief  Метод "рождения" мутированных потомков
         * @param  chr  - родительская хромосома, из которой "рождается"
         *                мутированный потомок
         * @return cids - популяция потомков
        **/
        void mutate(const CChromosome&chr, CPopulation&cids)
        {
            Q_ASSERT(CChromosome::size());
            CChromosome child(chr);
            unsigned int locus = qMax(1,
                                      qMin(int(CChromosome::size()) - 2,
                                           rand()%int(CChromosome::size())));
            child.invertGene(locus);
            cids.addChromosome(child);
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Одноточечная мутация");
        };
    };
};
using namespace GeneticAlgorithm;
#endif // C_O
