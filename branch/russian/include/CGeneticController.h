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
 * @file    CGeneticController.h
 * @brief   Файл содержит класс CGeneticController, который отвечает
 *          за бизнес-логику островной модели генетического алгоритма
 *          Генетический алгорит является алгоритмом безусловной
 *          оптимизации. Для того, чтобы учесть ограничения, можно
 *          использовать, например, метод штрафных (барьерных) функций.
 * @date    23/03/2009
 * @version 1.14
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
namespace InsularGenetica
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
     *        |                                    | Обработка событий
     *        |                                    | пользователя, например,
     *        |                                    | нажатие на кнопку
     *        |                                    | "Отмена", приводящее
     *        |                                    | к срабатыванию слота
     *        |                                    | SomeDialog::cancel()
     *        |                                    | // Слот отмены расчета
     *        |    CPopulation pop =               | SomeDialog::cancel()
     *        |      ::calc->getBestSolutions(3);  | {
     *        |                                    |   if(::calc)
     *        |                                    |      ::calc->cancel();
     *        |                                    | }
     *        |                                    |
     *        |    delete ::calc;
     *        |    ::calc = NULL;
     *        | }
     *        |
     *
    **/
    struct Q_DECL_EXPORT CGeneticController : protected QThread
    {
        enum OperatorType
        {
            Accepting,
            Grouping,
            Reproduction,
            Selection,
            Mutation
        };
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
         *                 (в2 раза больше числа установленных процессоров).
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
         *                 (в2 раза больше числа установленных процессоров).
         *  `              По определению, надежность решения повышается при
         *                 увеличении числа островов. Однако время поиска
         *                 увеличивается за счет потребления ресурсов при
         *                 передаче управления "островам".
         * Допускается НЕпотокобезопасная реализация функции здоровья
        **/
        static CGeneticController*getCalculator(IFitness*    fitness,
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
         * @brief Основной цикл потока
        **/
        void run();
    private:
#if QT_VERSION < 0x040000
        ///<! Острова - потоки генетических алгоритмов
        QValueList <CGeneticAlgorithm*>m_algorithms;
        ///<! Словарь операторов отбора
        QValueList <IGeneticOperator*> m_operators;
#else
        ///<! Острова - потоки генетических алгоритмов
        QList <CGeneticAlgorithm*>     m_algorithms;
        ///<! Словарь операторов отбора
        QList <IGeneticOperator*>      m_operators;
#endif
        ///<! Популяция лучших решений
        CPopulation                    m_best_solutions;
        ///<! Размер популяции лучших решений
        unsigned long                  m_best_solutions_size;
        ///<! Длительность расчета в минутах
        unsigned long                  m_minutes;
        ///<! Функция расчета здоровья хромосомы
        IFitness*                      m_function;
    };
};
#endif // C_GENETIC_CONTROLLER_H_INCLUDED
