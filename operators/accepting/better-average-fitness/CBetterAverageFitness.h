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
** ПРИ СОХРАНЕНИИ ИНФОРМАЦИИ О РАЗРАБОТЧИКАХ ЭТОЙ БИБЛИОТЕКИ.
****************************************************************************/
/**
 * @file    CBetterAverageFitness.h
 * @brief   Файл содержит класс CBetterAverageFitness отбора родителей
 * @date    20/02/2009
**/
#ifndef C_BETTER_AVERAGE_FITNESS_H_INCLUDED
#define C_BETTER_AVERAGE_FITNESS_H_INCLUDED
#include "../../../idl/IAccepting.h"
#include "../../../include/CPopulation.h"
#include "../../../include/CChromosome.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qstring.h>
    #include <qobject.h>
#else
    #include <QtCore/QString>
    #include <QtCore/QObject>
#endif
namespace GeneticAlgorithm
{
    struct CBetterAverageFitness : virtual public IAccepting
    {
        /**
         * @brief   Базовый конструктор
        **/
        CBetterAverageFitness(){};
        /**
         * @brief   Деструктор
        **/
        ~CBetterAverageFitness(){};
        /**
         * @brief  Метод оценки пригодности хромосомы
         * @param  pop - популяция родителей
         * @param  cur - хромосома, которая оценивается на пригодность
         * @return true , если хромосома пригодна для популяции
         * @return false, если хромосома не пригодна для популяции
        **/
        bool accept(const CPopulation*pop, const CChromosome&cur)
        {
            Q_ASSERT(CChromosome::size());
            return (pop->getAverageFitness() < cur.fitness());
        };
        /**
         * @brief   Метод получения наименования генетического оператора
         * @return  наименование генетического оператора
        **/
        const QString name()
        {
            return QObject::trUtf8("Здоровье хромосомы больше "
                                   "среднего здоровья");
        };
    };
};
using namespace GeneticAlgorithm;
#endif // C_BETTER_AVERAGE_FITNESS_H_INCLUDED
