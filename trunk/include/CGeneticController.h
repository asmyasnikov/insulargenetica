/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
in file LICENSE along with this program.  If not, see
<http://www.gnu.org/licenses/>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for № FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers / users,
no additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CGeneticController.h
 * @brief   Файл содержит класс CGeneticController, который отвечает
 *          за бизнес-логику островной модели генетического алгоритма
 *          Генетический алгорит является алгоритмом безусловной
 *          оптимизации. Для того, чтобы учесть ограничения, можно
 *          использовать, например, метод штрафных (барьерных) функций.
 * @date    23/03/2009
 * @version 1.18
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
#include "../idl/ICancelService.h"
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
    struct Q_DECL_EXPORT CGeneticController : public ICancelService
    {
        /**
         * @brief Деструктор
        **/
        ~CGeneticController(void);
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
         *                 (в 2 раза больше числа установленных процессоров).
         *  `              По определению, надежность решения повышается при
         *                 увеличении числа островов. Однако время поиска
         *                 увеличивается за счет потребления ресурсов при
         *                 передаче управления "островам".
         * Допускается НЕпотокобезопасная реализация функции здоровья
        **/
        static CPopulation calc(IFitness*       fitness,
                                unsigned int    chromosom,
                                unsigned int    population = 128,
                                unsigned int    minutes = 60,
                                int             island = -1,
                                ICancelService* cancel_service = NULL);
        /**
         * @brief Статический метод конструирования "калькулятора"
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
        static CGeneticController*getCalculator(IFitness*       fitness,
                                                unsigned int    chromosom,
                                                unsigned int    population = 128,
                                                unsigned int    minutes = 60,
                                                int             island = -1,
                                                ICancelService* cancel_service = NULL);
        /**
         * @brief   This method provides canceling of evaluations.
         * @return  cancel status
        **/
        bool isCanceled();
    private:
        /**
         * @brief Конструктор
         * @param island - количество островов
        **/
        CGeneticController(IFitness*       fitness,
                           unsigned int    population,
                           unsigned int    island,
                           unsigned long   minutes,
                           ICancelService* cancel_service = NULL);
        /**
         * @brief Calculate method
        **/
        void calculate();
    private:
        enum OperatorType
        {
            Accepting,
            Grouping,
            Reproduction,
            Selection,
            Mutation
        };
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
        ///<! Cancel service pointer
        ICancelService*                m_cancel_service;
        ///<! Calculate status
        bool                           m_is_calculate;
        ///<! Locker
        QMutex                         m_mutex;
    };
};
#endif // C_GENETIC_CONTROLLER_H_INCLUDED
