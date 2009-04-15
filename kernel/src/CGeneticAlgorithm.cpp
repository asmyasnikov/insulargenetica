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
 * @file    CGeneticAlgorithm.cpp
 * @brief   Файл содержит реализацию класс CGeneticAlgorithm, который
 *          отвечает за бизнес-логику генетического алгоритма, за загрузку
 *          модулей генетического алгоритма, за динамическое определение
 *          частот использования генетических операторов
 * @date    05/03/2009
**/
#include "CGeneticAlgorithm.h"
#include <math.h>
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qapplication.h>
    #include <qdatetime.h>
    #include <qpair.h>
    #ifndef qMin
        #define qMin(a,b) QMIN((a),(b))
    #endif
    #ifndef qMax
        #define qMax(a,b) QMAX((a),(b))
    #endif
#else
    #include <QtCore/QCoreApplication>
    #include <QtCore/QDateTime>
    #include <QtCore/QTime>
    #include <QtCore/QPair>
#endif
#include "../../idl/ISelection.h"
#include "../../idl/IGrouping.h"
#include "../../idl/IReproduction.h"
#include "../../idl/IMutation.h"
#include "../../idl/IAccepting.h"
// Коэффициент "забывания" частот
#define FORGET_PERIOD 5
// Время для контроля бесконечных циклов - 30 секунд
#define INFINITE_LOOPING_TIME_SECONDS   30
// Время опроса событий пользователя - 0.5 секунды
#define PROCESS_EVENTS_TIME_MSECONDS    500
namespace GeneticAlgorithm
{
    struct COperatorStatistics
    {
        double m_used_time;
        double m_best_solutions;
        double m_statistical_frequency;
    };
};
unsigned int GeneticAlgorithm::CGeneticAlgorithm::m_population_size = 128;
/**
 * @brief Статический метод для установления размера популяции,
 *        одинакового на всех островах
 * @param size - размер популяции
**/
void
GeneticAlgorithm::
CGeneticAlgorithm::
setPopulationSize(unsigned int size)
{
    m_population_size = size;
};
/**
 * @brief Конструктор
 * @param limit_time  - Ограничение по времени
 * @param limit_steps - Ограничение по количеству поколений
**/
GeneticAlgorithm::
CGeneticAlgorithm::
CGeneticAlgorithm(
#if QT_VERSION < 0x040000
                  QValueList<IGeneticOperator*>operators,
#else
                  QList<IGeneticOperator*>operators,
#endif
                  unsigned long minutes) :
    m_minutes(minutes),
    m_neighbour(NULL),
    m_mutex(
#if QT_VERSION < 0x040000
            true
#else
            QMutex::Recursive
#endif
           )
{
    QMutexLocker locker(&m_mutex);
    m_result_code = NoCode;
    m_population = CPopulation(m_population_size);
    initLibraries(m_selections   , operators);
    initLibraries(m_groupings    , operators);
    initLibraries(m_reproductions, operators);
    initLibraries(m_mutations    , operators);
    initLibraries(m_acceptings   , operators);
    Q_ASSERT(m_selections.size());
    Q_ASSERT(m_groupings.size());
    Q_ASSERT(m_reproductions.size());
    Q_ASSERT(m_mutations.size());
    Q_ASSERT(m_acceptings.size());
};
/**
 * @brief Деструктор
**/
GeneticAlgorithm::
CGeneticAlgorithm::
~CGeneticAlgorithm()
{
    terminate();
    m_selections.clear();
    m_groupings.clear();
    m_reproductions.clear();
    m_mutations.clear();
    m_acceptings.clear();
};
/**
 * @brief Шаблон инициализирует статистические частоты для генетических
 *        операторов
 * @param map       - словарь, в который следует поместить модули
 *                    генетических операторов
 * @param operators - модули генетических операторов
**/
template<typename T>
void
GeneticAlgorithm::
CGeneticAlgorithm::
initLibraries(QMap<T*, COperatorStatistics>&map,
#if QT_VERSION < 0x040000
              QValueList<IGeneticOperator*> operators
#else
              QList<IGeneticOperator*>      operators
#endif
             )
{
    QMutexLocker locker(&m_mutex);
#if QT_VERSION < 0x040000
    for(QValueList<IGeneticOperator*>::iterator i = operators.begin();
        i != operators.end(); i++)
    {
        IGeneticOperator*op = *i;
#else
    foreach(IGeneticOperator*op, operators)
    {
#endif
        T*oper = dynamic_cast<T*>(op);
        if(oper)
        {
            COperatorStatistics stat;
            stat.m_used_time             = 0.;
            stat.m_best_solutions        = 0.;
            stat.m_statistical_frequency = 0.;
            map[oper] = stat;
        }
    }
#if QT_VERSION < 0x040000
    for(QMap<T*, COperatorStatistics>::iterator i = map.begin();
        i != map.end(); i++)
    {
        T*op = i.key();
#else
    foreach(T*op, map.keys())
    {
#endif
        map[op].m_statistical_frequency = 1./double(map.size());
    }
};
/**
 * @brief   Шаблон позволяет получить генетический оператор по его
 *          текущей частоте
 * @param   map - словарь, в котором содержатся модули генетических
 *          операторов
 * @return  Генетический оператор или NULL при ошибке
**/
template<typename T>
inline T*
GeneticAlgorithm::
CGeneticAlgorithm::
getGeneticOperator( QMap<T*,COperatorStatistics>&map) const
{
    QMutexLocker locker(&m_mutex);
    // Сначала выдаются генетические операторы, которые ранее не
    // использовались
#if QT_VERSION < 0x040000
    for(QMap<T*, COperatorStatistics>::iterator i = map.begin();
        i != map.end(); i++)
    {
        T*op = (T*)(i.key());
#else
    foreach(T*op, map.keys())
    {
#endif
        if(map[op].m_used_time < 1.e-7) return op;
    }
    // Если все операторы уже использовались, то выдаем оператор
    // по его частоте
    double random = double(rand())/double(RAND_MAX);
    double summ   = 0.;
#if QT_VERSION < 0x040000
    for(QMap<T*, COperatorStatistics>::iterator i = map.begin();
        i != map.end(); i++)
    {
        T*op = (T*)(i.key());
#else
    foreach(T*op, map.keys())
    {
#endif
        summ += map[op].m_statistical_frequency;
        if(random < summ) return op;
    }
    return (T*)(map.begin().key());
};
/**
 * @brief Шаблон позволяет выгружать модули генетических операторов
 * @param map       - словарь, в котором содержатся модули генетических
 *                    операторов
 * @param op        - генетический оператор, для которого производится
 *                    пересчет частоты
 * @param used_time - затраченнное время на работу оператора
 * @param best      - количество хороших хромосом при использовании оператора
**/
template<typename T>
inline void
GeneticAlgorithm::
CGeneticAlgorithm::
recalcStatisticalFrequency( QMap<T*, COperatorStatistics>&map,
                            const T*oper,
                            double used_time,
                            unsigned int best)
{
    QMutexLocker locker(&m_mutex);
    if(map.size() < 2) return;
    double summ = 0.;
#if QT_VERSION < 0x040000
    for(QMap<T*, COperatorStatistics>::iterator i = map.begin();
        i != map.end(); i++)
    {
        T*op = (T*)(i.key());
#else
    foreach(T*op, map.keys())
    {
#endif
        if(oper == op)
        {
            map[op].m_used_time      = 2./double(FORGET_PERIOD+1)*used_time +
                                         double(FORGET_PERIOD-1)/
                                         double(FORGET_PERIOD+1)*
                                         (map[op].m_used_time);
            map[op].m_best_solutions = 2./double(FORGET_PERIOD+1)*
                                         double(best) +
                                         double(FORGET_PERIOD-1)/
                                         double(FORGET_PERIOD+1)*
                                         (map[op].m_best_solutions);
        }
        map[op].m_statistical_frequency = qMax(map[op].m_best_solutions,0.5)/
                                          qMax(map[op].m_used_time,0.5);
        summ += map[op].m_statistical_frequency;
    }
#if QT_VERSION < 0x040000
    for(QMap<T*, COperatorStatistics>::iterator i = map.begin();
        i != map.end(); i++)
    {
        T*op = (T*)(i.key());
#else
    foreach(T*op, map.keys())
    {
#endif
        map[op].m_statistical_frequency /= summ;
    }
};
/**
 * @brief Основной цикл потока
**/
void
GeneticAlgorithm::
CGeneticAlgorithm::
run()
{
    QDateTime time_control = QDateTime::currentDateTime()
                             .addSecs(m_minutes*60);
    m_mutex.lock();
    m_result_code = Process;
    m_mutex.unlock();
    // Проверяем разнородность популяции. Из зацикливания выходим по времени
    QDateTime dtime = QDateTime::currentDateTime()
                      .addSecs(INFINITE_LOOPING_TIME_SECONDS);
    while(m_population.getHomogeneity(true) > 0.55 &&
          QDateTime::currentDateTime().secsTo(dtime) > 0)
    {
        QMutexLocker locker(&m_mutex);
        m_population = CPopulation(m_population_size);
    }
    QTime timer;
    timer.start();
    double best_fitness = m_population.getMaximumFitness();
    QTime unimprovability;
    unimprovability.start();
    unsigned int counter = 0;
    IAccepting*prev_accepting = NULL;
    double     prev_accepting_time = 0.;
    double     childs = 0.;
    // Входим в цикл поиска решения
    while( result() == Process )
    {
        if(timer.elapsed() > PROCESS_EVENTS_TIME_MSECONDS)
        {
#if QT_VERSION < 0x040000
            qApp->processEvents();
#else
            QCoreApplication::processEvents();
#endif
            timer.restart();
        }
        ISelection   *m_selection    = getGeneticOperator(m_selections   );
        IGrouping    *m_grouping     = getGeneticOperator(m_groupings    );
        IReproduction*m_reproduction = getGeneticOperator(m_reproductions);
        IMutation    *m_mutation     = getGeneticOperator(m_mutations    );
        IAccepting   *m_accepting    = getGeneticOperator(m_acceptings   );
        Q_ASSERT(m_selection);
        Q_ASSERT(m_grouping);
        Q_ASSERT(m_reproduction);
        Q_ASSERT(m_mutation);
        Q_ASSERT(m_accepting);
        CPopulation selection;
        QTime time;
        time.start();
        m_mutex.lock();
        m_selection->select(m_population, selection);
        m_mutex.unlock();
        double selection_time = double(time.elapsed());
        if(best_fitness < m_population.getMaximumFitness())
        {
            best_fitness = m_population.getMaximumFitness();
            unimprovability.restart();
        }
        bool timeIsUp = (unimprovability.elapsed()>
                         qMax(int(m_minutes)*600,6000));
        if(timeIsUp)
        {
            // Миграция
            if(m_neighbour)
            {
                CPopulation best_of_neighbour =
                    m_neighbour->getBestChromosomes(qMax(m_population_size/10,
                                                         (unsigned int)(1)));
                for(int i = 0; i < best_of_neighbour.size(); i++)
                {
                    QMutexLocker locker(&m_mutex);
                    m_population.replaceChromosome(best_of_neighbour
                                                   .getChromosome(i));
                }
            }
        }
        // Популяция хромосом, рожденных скрещиванием
        CPopulation reproduct;
        // Популяция мутированных хромосом
        CPopulation mutation;
        // Скрещивание дало хорошее потомство
        unsigned int good_reproduct = 0;
        // Мутация дала хорошее потомство
        unsigned int good_mutation  = 0;
        CParents parents;
        time.restart();
        m_grouping->group(selection, parents);
        double grouping_time = double(time.elapsed());
        // "Родим" потомков
        time.restart();
        for(CParents::iterator i = parents.begin(); i != parents.end(); i++)
        {
            m_reproduction->reproduct(*i, reproduct);
        }
        double reproduct_time = double(time.elapsed());
        for(int i = 0; i < reproduct.size(); i++)
        {
            QMutexLocker locker(&m_mutex);
            if(reproduct.getChromosome(i).fitness() <
               m_population.getMaximumFitness()) break;
            good_reproduct++;
        }
        // Если среди потомков, рожденных скрещиванием, есть лучшие,
        // то пересчиатем частоты операторов группировки и репродукции
        // Если после оценки пригодности лучшая хромосома будет отвергнута,
        // то в результате изменения частот вероятность "рождения" такой же
        // хромосомы повышается
        recalcStatisticalFrequency(m_groupings    , m_grouping    ,
                                   grouping_time+reproduct_time,
                                   good_reproduct);
        recalcStatisticalFrequency(m_reproductions, m_reproduction,
                                   grouping_time+reproduct_time,
                                   good_reproduct);
        // "Мутируем" потомков
        time.restart();
        for(int i = 0; i < selection.size(); i++)
        {
            m_mutation->mutate(selection.getChromosome(i), mutation);
        }
        double mutation_time = double(time.elapsed());
        for(int i = 0; i < mutation.size(); i++)
        {
            QMutexLocker locker(&m_mutex);
            if(mutation.getChromosome(i).fitness() <
               m_population.getMaximumFitness()) break;
            good_mutation++;
        }
        // Если среди потомков, рожденных скрещиванием, есть лучшие,
        // то пересчиатем частоты операторов мутации
        // Даже если после оценки пригодности лучшая хромосома будет
        // отвергнута, то в результате изменения частот вероятность
        // "мутации" к такой же хромосоме повышается
        recalcStatisticalFrequency(m_mutations, m_mutation,
                                   mutation_time, good_mutation);
        // Если среди потомков есть лучшие и родительсий пул не включал
        // случайных хромосом, то пересчиатем частоты операторов отбора,
        // Так как отобранные хромосомы мутировали, скрещивались и дали
        // хорошее потомство, то, очевидно, что такой отбор хороший
        if(!timeIsUp)
        {
            recalcStatisticalFrequency( m_selections,
                                        m_selection,
                                        selection_time+reproduct_time+
                                        mutation_time+grouping_time,
                                        good_reproduct+good_mutation);
        }
        time.restart();
        for(int i = 0; i < reproduct.size(); i++)
        {
            QMutexLocker locker(&m_mutex);
            if(m_accepting->accept(&m_population,reproduct.getChromosome(i)))
            {
                QMutexLocker locker(&m_mutex);
                m_population.replaceChromosome(reproduct.getChromosome(i));
            }
        }
        for(int i = 0; i < mutation.size(); i++)
        {
            if(m_accepting->accept(&m_population, mutation.getChromosome(i)))
            {
                QMutexLocker locker(&m_mutex);
                m_population.replaceChromosome(mutation.getChromosome(i));
            }
        }
        double accepting_time = double(time.elapsed());
        // Если популяция стала разнороднее, то пересчитаем частоту
        // операторов оценки пригодности хромосом.
        if(prev_accepting)
        {
            recalcStatisticalFrequency( m_acceptings,
                                        prev_accepting,
                                        prev_accepting_time,
                                        good_reproduct+good_mutation );
        }
        prev_accepting      = m_accepting;
        prev_accepting_time = accepting_time;
        counter++;
        childs += reproduct.size() + mutation.size();
        if(result() != Cancel)
        {
            QMutexLocker locker(&m_mutex);
            if(unimprovability.elapsed() >
               qMax(int(m_minutes)*6000,60000))
                 m_result_code = DeadLock;
            else if(m_population.getHomogeneity() > 0.95)
                 m_result_code = GoodHomogeneity;
            else if(QDateTime::currentDateTime().secsTo(time_control)<0)
                 m_result_code = MaxTimes;
        }
    }
};
GeneticAlgorithm::
CGeneticAlgorithm::
ResultCode
GeneticAlgorithm::
CGeneticAlgorithm::
result() const
{
    QMutexLocker locker(&m_mutex);
    return m_result_code;
};
void
GeneticAlgorithm::
CGeneticAlgorithm::
setNeighbourAlgorithm(CGeneticAlgorithm*neighbour)
{
    QMutexLocker locker(&m_mutex);
    m_neighbour     = neighbour;
};
/**
 * @brief   Метод получения size-текущих оптимальных решений
 * @param   size - размер популяции на выходе
 * @return  Популяция size или меньших наилучших решений
**/
GeneticAlgorithm::
CPopulation
GeneticAlgorithm::
CGeneticAlgorithm::
getBestChromosomes(int size) const
{
    QMutexLocker locker(&m_mutex);
    CPopulation result;
    for(int i = 0; i < m_population.size() && i < size; i++)
    {
        result.addChromosome(m_population.getChromosome(i));
    }
    return result;
};
/**
 * @brief Прерывание работы основного цикла потока
**/
void
GeneticAlgorithm::
CGeneticAlgorithm::
cancel()
{
    if(result() == Process)
    {
        QMutexLocker locker(&m_mutex);
        m_result_code = Cancel;
    }
    if(m_neighbour && result() == Process)
    {
        m_neighbour->cancel();
    }
};
