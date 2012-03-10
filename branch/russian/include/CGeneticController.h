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
#include "../idl/ICancelService.h"
namespace InsularGenetica
{
    class CGeneticAlgorithm;
    class IGeneticOperator;
    /**
     * @brief Это основной класс генетического алгоритма
     *        Конструктор класса намеренно недоступен
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
        CPopulation getBestSolutions(uint size);
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
         * @brief   Этот метод позволяет проверить факт отмены расчета
         * @return  cancel status
        **/
        bool isCanceled();

        /**
         * @brief   Заменить худшую хромосому на заданную
         *          Этот метод помогает инициализироватьисходную популяцию
         *          нужным значением
         * @chr     Хромосома для замены
        **/
        void replaceWorstChromosome(const CChromosome& chr);
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
        ///<! Указатель на сервис отмены
        ICancelService*                m_cancel_service;
        ///<! Статус расчета
        bool                           m_is_calculate;
        ///<! Блокатор
        QMutex                         m_mutex;
    };
};
#endif // C_GENETIC_CONTROLLER_H_INCLUDED

