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
 * @file    CGeneticAlgorithm.h
 * @brief   Файл содержит класс CGeneticAlgorithm, который отвечает за
 *          бизнес-логику генетического алгоритма, за загрузку модулей
 *          генетического алгоритма, за динамическое определение частот
 *          использования генетических операторов
 * @date    05/03/2009
 * @version 1.18
**/
#ifndef GENETIC_ALGORITHM_FACTORY_H_INCLUDED
#define GENETIC_ALGORITHM_FACTORY_H_INCLUDED
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qthread.h>
    #include <qmutex.h>
    #include <qmap.h>
    #include <qlist.h>
#else
    #include <QtCore/QThread>
    #include <QtCore/QMutex>
    #include <QtCore/QMap>
    #include <QtCore/QList>
#endif
#include "../idl/IGeneticOperator.h"
#include "../idl/ICancelService.h"
#include "../include/CPopulation.h"
class QString;
namespace InsularGenetica
{
    class COperatorStatistics;
    class IFitness;
    class ISelection;
    class IGrouping;
    class IReproduction;
    class IMutation;
    class IAccepting;
    Q_DECL_EXPORT struct CGeneticAlgorithm : public QThread,
                                     virtual public Interface
    {
        enum ResultCode
        {
            NoCode              = -1, // Алгоритм еще работает
            Process             =  0, // Задача находится в процессе расчета
            MaxTimes            =  1, // Максимальное время эволюции
            DeadLock            =  2, // Длительное неулучшение решения
            GoodHomogeneity     =  3, // Достижение 95-% однородности
            Cancel              =  4  // Отмена
        };
        /**
         * @brief Статический метод для установления размера популяции,
         *        одинакового на всех островах
         * @param size - размер популяции
        **/
        static void setPopulationSize(unsigned int size);
        /**
         * @brief Конструктор
         * @param operators - Генетические операторы
         * @param minutes   - Ограничение по времени
        **/
        CGeneticAlgorithm(ICancelService*cancel_service,
#if QT_VERSION < 0x040000
                          QValueList<IGeneticOperator*> operators,
#else
                          QList<IGeneticOperator*>      operators,
#endif
                          unsigned long                 minutes = 168);
        /**
         * @brief Деструктор
        **/
        ~CGeneticAlgorithm();
        /**
         * @brief   Метод позволяет получить код завершения алгоритма
         * @return  Код завершения алгоритма
        **/
        ResultCode result() const;
        /**
         * @brief   Метод назначения ближайшего острова в островной модели
         *          с топологией "кольцо"
         * @param   neighbour - генетический алгоритм соседнего острова
        **/
        void setNeighbourAlgorithm(CGeneticAlgorithm*neighbour);
        /**
         * @brief   Метод получения size-текущих оптимальных решений
         * @param   size - размер популяции на выходе
         * @return  Популяция size или меньших наилучших решений
        **/
        CPopulation getBestChromosomes(int size) const;
        /**
         * @brief Основной цикл потока
        **/
        void run();
        /**
         * @brief Прерывание работы основного цикла потока
        **/
        void cancel();
    private:
        /**
         * @brief Шаблон инициализирует статистические частоты для
         *        генетических операторов
         * @param map       - словарь, в который следует поместить модули
         *                    генетических операторов
         * @param operators - модули генетических операторов
        **/
        template<typename T>
        void initLibraries(QMap<T*, COperatorStatistics>&   map,
#if QT_VERSION < 0x040000
                           QValueList<IGeneticOperator*>    operators
#else
                           QList<IGeneticOperator*>         operators
#endif
                          );
        /**
         * @brief   Шаблон позволяет получить генетический оператор по
         *          его текущей частоте
         * @param   map - словарь, в котором содержатся модули
         *          генетических операторов
         * @return  Генетический оператор или NULL при ошибке
        **/
        template<typename T>
        inline T* getGeneticOperator(QMap<T*, COperatorStatistics>&map) const;
        /**
         * @brief Шаблон позволяет выгружать модули генетических операторов
         * @param map       - словарь, в котором содержатся модули
         *                    генетических операторов
         * @param op        - генетический оператор, для которого
         *                    производится пересчет частоты
         * @param used_time - затраченнное время на работу оператора
         * @param best      - количество хороших хромосом при использовании
         *                    оператора
        **/
        template<typename T>
        inline void
        recalcStatisticalFrequency( QMap<T*, COperatorStatistics>& map,
                                    const T*                       op,
                                    double                         used_time,
                                    unsigned int                   best);
    private:
        ///<! Словарь операторов отбора
        QMap <ISelection*   , COperatorStatistics> m_selections;
        ///<! Словарь операторов соединения пар
        QMap <IGrouping*    , COperatorStatistics> m_groupings;
        ///<! Словарь операторов скрещивания
        QMap <IReproduction*, COperatorStatistics> m_reproductions;
        ///<! Словарь операторов мутации
        QMap <IMutation*    , COperatorStatistics> m_mutations;
        ///<! Словарь операторов оценки пригодности потомков
        QMap <IAccepting*   , COperatorStatistics> m_acceptings;
        ///<! Ограничение по времени
        unsigned long                              m_minutes;
        ///<! Код завершения алгоритма
        ResultCode                                 m_result_code;
        ///<! Размер популяции
        static unsigned int                        m_population_size;
        ///<! Популяция острова
        CPopulation                                m_population;
        ///<! Алгоритм соседнего острова
        CGeneticAlgorithm*                         m_neighbour;
        ///<! Блокатор совместных данных
        mutable QMutex                             m_mutex;
        ///<! Cancel service pointer
        ICancelService*                            m_cancel_service;
    };
};
#endif // GENETIC_ALGORITHM_FACTORY_H_INCLUDED
