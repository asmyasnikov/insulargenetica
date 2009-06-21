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
 * @file    CGeneticAlgorithm.h
 * @brief   Файл содержит класс CGeneticAlgorithm, который отвечает за
 *          бизнес-логику генетического алгоритма, за загрузку модулей
 *          генетического алгоритма, за динамическое определение частот
 *          использования генетических операторов
 * @date    05/03/2009
 * @version 1.14
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
        CGeneticAlgorithm(
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
    };
};
#endif // GENETIC_ALGORITHM_FACTORY_H_INCLUDED
