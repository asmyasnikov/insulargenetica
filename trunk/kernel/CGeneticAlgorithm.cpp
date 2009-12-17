/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser GPL as publishedby
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details. You should have received a
copy of the GNU Lesser GPL in file LICENSE along with this
program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for N FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers/users, no
additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CGeneticAlgorithm.cpp
 * @brief   Class CGeneticAlgorithm implement business logic of
 *          genetic algorithm, provide control of genetic operators
 * @author  Aleksey Myasnikov
 * @date    05/03/2009
 * @version 3.3
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
#include "../idl/ISelection.h"
#include "../idl/IGrouping.h"
#include "../idl/IReproduction.h"
#include "../idl/IMutation.h"
#include "../idl/IAccepting.h"
// Period of forget
#define FORGET_PERIOD 5
// Time for control infinite loop - 30 seconds
#define INFINITE_LOOPING_TIME_SECONDS   30
// Time of process events - 0.5 second
#define PROCESS_EVENTS_TIME_MSECONDS    500
// Don't use mutation
//#define DONT_USE_MUTATION
// Use mutation
#define USE_MUTATION
// Use stohastic mutation
//#define USE_STOHASTIC_MUTATION

#ifdef DONT_USE_MUTATION
    #undef USE_MUTATION
    #undef USE_STOHASTIC_MUTATION
#else
    #ifdef USE_STOHASTIC_MUTATION
        #ifdef USE_MUTATION
            #undef USE_MUTATION
        #endif
    #endif
#endif

namespace InsularGenetica
{
    struct COperatorStatistics
    {
        double m_used_time;
        double m_best_solutions;
        double m_statistical_frequency;
    };
};
unsigned int InsularGenetica::CGeneticAlgorithm::m_population_size = 128;
/**
 * @brief Static method of setting population size
 * @param size - size of population
**/
void
InsularGenetica::
CGeneticAlgorithm::
setPopulationSize(unsigned int size)
{
    m_population_size = size;
};
/**
 * @brief Constructor
 * @param operators - genetic operators
 * @param minutes   - maximum minutes of evalutions
**/
InsularGenetica::
CGeneticAlgorithm::
CGeneticAlgorithm(ICancelService*cancel_service,
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
    m_cancel_service = cancel_service;
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
 * @brief Destructor
**/
InsularGenetica::
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
 * @brief Initialization of genetic operator frequencies
 *        and parse operators by types
 * @param map       - operators by types
 * @param operators - genetic operators
**/
template<typename T>
void
InsularGenetica::
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
 * @brief   Getting genetic operator by current frequency of
 *          genetic operator
 * @param   map - genetic operators for choise
 * @return  operator pointer or NULL (if get error)
**/
template<typename T>
inline T*
InsularGenetica::
CGeneticAlgorithm::
getGeneticOperator( QMap<T*,COperatorStatistics>&map) const
{
    QMutexLocker locker(&m_mutex);
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
 * @brief Update of genetic operators frequencies
 * @param map       - group of genetic operators
 * @param op        - current genetic operator
 * @param used_time - time of algorithm work with operator op
 * @param best      - number of good chromosomes returned operator op
**/
template<typename T>
inline void
InsularGenetica::
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
 * @brief Main loop of algorithm
**/
void
InsularGenetica::
CGeneticAlgorithm::
run()
{
    QDateTime time_control = QDateTime::currentDateTime()
                             .addSecs(m_minutes*60);
    m_mutex.lock();
    m_result_code = Process;
    m_mutex.unlock();
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
    CChromosome best_chr(m_population.getChromosome(0));
    QTime unimprovability;
    unimprovability.start();
    unsigned int counter = 0;
    IAccepting*prev_accepting = NULL;
    double     prev_accepting_time = 0.;
    double     childs = 0.;
    while( result() == Process )
    {
        if(timer.elapsed() > PROCESS_EVENTS_TIME_MSECONDS)
        {
            if(m_cancel_service->isCanceled())
            {
                QMutexLocker locker(&m_mutex);
                m_result_code = Cancel;
            }
            timer.restart();
        }
        ISelection   *m_selection    = getGeneticOperator(m_selections   );
        IGrouping    *m_grouping     = getGeneticOperator(m_groupings    );
        IReproduction*m_reproduction = getGeneticOperator(m_reproductions);
#ifndef DONT_USE_MUTATION
        IMutation    *m_mutation     = getGeneticOperator(m_mutations    );
#endif
        IAccepting   *m_accepting    = getGeneticOperator(m_acceptings   );
        Q_ASSERT(m_selection);
        Q_ASSERT(m_grouping);
        Q_ASSERT(m_reproduction);
#ifndef DONT_USE_MUTATION
        Q_ASSERT(m_mutation);
#endif
        Q_ASSERT(m_accepting);
        CPopulation selection;
        QTime time;
        time.start();
        m_mutex.lock();
        m_selection->select(m_population, selection);
        m_mutex.unlock();
        double selection_time = double(time.elapsed());
        if(best_chr < m_population.getChromosome(0))
        {
            best_chr = m_population.getChromosome(0);
            unimprovability.restart();
        }
        bool timeIsUp = (unimprovability.elapsed()>
                         qMax(int(m_minutes)*600,6000));
        if(timeIsUp)
        {
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
        CPopulation reproduct;
        unsigned int good_reproduct = 0;
#ifndef DONT_USE_MUTATION
        CPopulation mutation;
        unsigned int good_mutation  = 0;
#endif
        CParents parents;
        time.restart();
        m_grouping->group(selection, parents);
        double grouping_time = double(time.elapsed());
        time.restart();
        for(CParents::iterator i = parents.begin(); i != parents.end(); i++)
        {
            m_reproduction->reproduct(*i, reproduct);
        }
        double reproduct_time = double(time.elapsed());
        for(int i = 0; i < reproduct.size(); i++)
        {
            QMutexLocker locker(&m_mutex);
            if(reproduct.getChromosome(i) <
               m_population.getChromosome(0)) break;
            good_reproduct++;
        }
        recalcStatisticalFrequency(m_groupings    , m_grouping    ,
                                   grouping_time+reproduct_time,
                                   good_reproduct);
        recalcStatisticalFrequency(m_reproductions, m_reproduction,
                                   grouping_time+reproduct_time,
                                   good_reproduct);
#ifndef DONT_USE_MUTATION
        time.restart();
        for(int i = 0; i < selection.size(); i++)
        {
#ifndef USE_STOHASTIC_MUTATION
            if(double(rand())/double(RAND_MAX) < 1./double(CChromosome::size()))
#endif
            m_mutation->mutate(selection.getChromosome(i), mutation);
        }
        double mutation_time = double(time.elapsed());
        for(int i = 0; i < mutation.size(); i++)
        {
            QMutexLocker locker(&m_mutex);
            if(mutation.getChromosome(i) <
               m_population.getChromosome(0)) break;
            good_mutation++;
        }
        recalcStatisticalFrequency(m_mutations, m_mutation,
                                   mutation_time, good_mutation);
#endif
        if(!timeIsUp)
        {
            recalcStatisticalFrequency( m_selections,
                                        m_selection,
                                        selection_time+
                                        reproduct_time
#ifndef DONT_USE_MUTATION
                                        +mutation_time
#endif
                                        +grouping_time,
                                        good_reproduct
#ifndef DONT_USE_MUTATION
                                        +good_mutation
#endif
                                      );
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
#ifndef DONT_USE_MUTATION
        for(int i = 0; i < mutation.size(); i++)
        {
            if(m_accepting->accept(&m_population, mutation.getChromosome(i)))
            {
                QMutexLocker locker(&m_mutex);
                m_population.replaceChromosome(mutation.getChromosome(i));
            }
        }
#endif
        double accepting_time = double(time.elapsed());
        if(prev_accepting)
        {
            recalcStatisticalFrequency( m_acceptings,
                                        prev_accepting,
                                        prev_accepting_time,
                                        good_reproduct
#ifndef DONT_USE_MUTATION
                                        +good_mutation
#endif
                                      );
        }
        prev_accepting      = m_accepting;
        prev_accepting_time = accepting_time;
        counter++;
        childs += reproduct.size() 
#ifndef DONT_USE_MUTATION
                  + mutation.size()
#endif
                  ;
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
InsularGenetica::
CGeneticAlgorithm::
ResultCode
InsularGenetica::
CGeneticAlgorithm::
result() const
{
    QMutexLocker locker(&m_mutex);
    return m_result_code;
};
void
InsularGenetica::
CGeneticAlgorithm::
setNeighbourAlgorithm(CGeneticAlgorithm*neighbour)
{
    QMutexLocker locker(&m_mutex);
    m_neighbour     = neighbour;
};
/**
 * @brief   Getting best chromosomes
 * @param   size - population size
 * @return  size or less then size chromsomes in population
**/
InsularGenetica::
CPopulation
InsularGenetica::
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
 * @brief Cancelling evalutions
**/
void
InsularGenetica::
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
