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
 * @file    CGeneticController.h
 * @brief   Файл содержит класс CGeneticController, который отвечает
 *          за бизнес-логику островной модели генетического алгоритма
 *          Генетический алгорит является алгоритмом безусловной
 *          оптимизации. Для того, чтобы учесть ограничения, можно
 *          использовать, например, метод штрафных (барьерных) функций.
 * @date    23/03/2009
**/

#ifndef C_GENETIC_CONTROLLER_H_INCLUDED
#define C_GENETIC_CONTROLLER_H_INCLUDED

#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qthread.h>
    #include <qvaluelist.h>
#else
    #include <QtCore/QThread>
    #include <QtCore/QSet>
#endif
#include "CPopulation.h"

class QLibrary;

namespace GeneticAlgorithm
{
    class CGeneticAlgorithm;
    class IGeneticOperator;

    /**
     * @brief Это основной класс генетического алгоритма
     *        Конструктор класса намеренно недоступен
     *        Существует 2 способа расчета:
     *
     *      1)Без возможности отмены расчета в произвольный момент времени:
     *
     *        |
     *        | CPopulation pop = CGeneticController::calc(...)
     *        |
     *
     *      2)С возможностью отмены расчета в произвольный момент времени:
     *
     *        |
     *        | CGeneticController*calc = NULL;
     *        |
     *        | // слот расчета
     *        | SomeDialog::calculate()
     *        | {
     *        |    ::calc = CGeneticController::getCalculator(...);
     *        |                                    |
     *        |                                    | Обработка событий пользователя, например,
     *        |                                    | нажатие на кнопку "Отмена", приводящее
     *        |                                    | к срабатыванию слота SomeDialog::cancel()
     *        |                                    | // Слот отмены расчета
     *        |    CPopulation pop =               | SomeDialog::cancel()
     *        |      ::calc->getBestSolutions(3);  | {
     *        |                                    |   if(::calc) ::calc->cancel();
     *        |                                    | }
     *        |                                    |
     *        |    delete ::calc;
     *        |    ::calc = NULL;
     *        | }
     *        |
     *
    **/
    Q_DECL_EXPORT struct CGeneticController : protected QThread
    {
        /**
         * @brief Деструктор
        **/
        ~CGeneticController(void);
        /**
         * @brief Отмена расчета алгоритма
        **/
        void cancel();
        /**
         * @brief Получить результаты расчета
        **/
        CPopulation getBestSolutions(int size);
        /**
         * @brief Статический метод поиска решения
         * @param fitness - целевая функция (функция здоровья),
         * @param chromosom - размер хромосомы,
         * @param population - размер популяции,
         * @param minutes - максимальное количество минут для расчета,
         * @param island - количество островов алгоритма. При (island = -1)
         *                 количество островов определяется автоматически
         *                 (в2 раза больше числа установленных процессоров) .
         *  `              По определению, надежность решения повышается при
         *                 увеличении числа островов. Однако время поиска
         *                 увеличивается за счет потребления ресурсов при
         *                 передаче управления "островам".
         * Допускается НЕпотокобезопасная реализация функции здоровья
        **/
        static CPopulation calc(IFitness*    fitness,
                                unsigned int chromosom,
                                unsigned int population = 128,
                                unsigned int minutes = 60,
                                int          island = -1);
        /**
         * @brief Статический метод конструирования потока "калькулятора"
         * @param fitness - целевая функция (функция здоровья),
         * @param chromosom - размер хромосомы,
         * @param population - размер популяции,
         * @param minutes - максимальное количество минут для расчета,
         * @param island - количество островов алгоритма. При (island = -1)
         *                 количество островов определяется автоматически
         *                 (в2 раза больше числа установленных процессоров) .
         *  `              По определению, надежность решения повышается при
         *                 увеличении числа островов. Однако время поиска
         *                 увеличивается за счет потребления ресурсов при
         *                 передаче управления "островам".
         * Допускается НЕпотокобезопасная реализация функции здоровья
        **/
        static CGeneticController*getCalculator( IFitness*    fitness,
                                                 unsigned int chromosom,
                                                 unsigned int population = 128,
                                                 unsigned int minutes = 60,
                                                 int          island = -1 );

    private:
        /**
         * @brief Конструктор
         * @param island - количество островов
        **/
        CGeneticController(IFitness*     fitness,
                           unsigned int  population,
                           unsigned int  island,
                           unsigned long minutes);
        /**
         * @brief Шаблон позволяет загружать библиотки из соответствующих поддирректорий
         * @param map   - словарь, в который следует поместить модули генетических операторов
         * @param name  - наименование поддирректории, в которой следует производить поиск
        **/
        void loadLibraries(const QString& name);
        /**
         * @brief Основной цикл потока
        **/
        void run();

    private:
#if QT_VERSION < 0x040000
        QValueList <CGeneticAlgorithm*>         m_algorithms;         ///<! Острова - потоки генетических алгоритмов
        QValueList <IGeneticOperator*>          m_operators;          ///<! Словарь операторов отбора
        QValueList<QLibrary*>                   m_load_libs;          ///<! Список загруженных библиотек
#else
        QList <CGeneticAlgorithm*>              m_algorithms;         ///<! Острова - потоки генетических алгоритмов
        QList <IGeneticOperator*>               m_operators;          ///<! Словарь операторов отбора
        QList <QLibrary*>                       m_load_libs;          ///<! Список загруженных библиотек
#endif
        CPopulation                             m_best_solutions;     ///<! Популяция лучших решений
        unsigned long                           m_best_solutions_size;///<! Размер популяции лучших решений
        unsigned long                           m_minutes;		      ///<! Длительность расчета в минутах
        IFitness*                               m_function;           ///<! Функция расчета здоровья хромосомы
    };
};
#endif // C_GENETIC_CONTROLLER_H_INCLUDED
