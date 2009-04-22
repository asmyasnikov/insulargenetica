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
 * @file    main.cpp
 * @brief   Основной файл запуска алгоритма, когда алгоритм собран как исполняемый файл
 * @date    04/03/2009
**/

#include <iostream>
#include "../../include/export.h"
#include "../../include/CPopulation.h"
#include "../../include/CFitnessHelper.h"
#include "../../include/CChromosome.h"
#include "../../include/CGeneticController.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qapplication.h>
    #include <qlibrary.h>
    #include <qdir.h>
    #include <qfile.h>
    #include <qtextstream.h>
    #include <qtextcodec.h>
#else
    #include <QtCore/QCoreApplication>
    #include <QtCore/QLibrary>
    #include <QtCore/QDir>
    #include <QtCore/QFile>
    #include <QtCore/QTextStream>
    #include <QtCore/QTextCodec>
#endif
#include <math.h>

using namespace GeneticAlgorithm;

int main(int argc, char**argv)
{
    srand(clock() % 128);
#if QT_VERSION < 0x040000
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif
#ifdef Q_OS_WIN
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM-866"));
#endif
    QDir dir("fitness");
    dir.setFilter(QDir::Files);
#if QT_VERSION < 0x040000
    dir.setNameFilter("*.so *.dll");
#else
    QStringList filters;
    filters.append("*.so");
    filters.append("*.dll");
    dir.setNameFilters(filters);
#endif
    QStringList list = dir.entryList();
#if QT_VERSION < 0x040000
    QValueList<IFitness*> m_fitness_modules;
    QValueList<QLibrary*> m_fitness_libraries;
#else
    QList<IFitness*> m_fitness_modules;
    QList<QLibrary*> m_fitness_libraries;
#endif
    for (int i = 0; i < list.size(); ++i)
    {
#if QT_VERSION < 0x040000
        QLibrary*lib = new QLibrary(dir.absFilePath(*list.at(i)));
#else
        QLibrary*lib = new QLibrary(dir.absoluteFilePath(list.at(i)));
#endif
        if(!lib->isLoaded() && !lib->load()) continue;
        typedef Interface*(*CreateModule)(void);
        CreateModule createModule = (CreateModule)(lib->resolve("CreateModule"));
        if(createModule)
        {
            Interface*iface = createModule();
            if(iface)
            {
                IFitness* module = dynamic_cast<IFitness*>(iface);
                if(module)
                {
                    m_fitness_modules.append(module);
                    m_fitness_libraries.append(lib);
                    continue;
                }else{
                    qDebug("createModule() return null");
                    delete iface;
                }
            }
        }else{
#if QT_VERSION < 0x040000
            qDebug("createModule() in %s not found", (*list.at(i)).local8Bit().data());
#else
            qDebug("createModule() in %s not found", list.at(i).toLocal8Bit().data());
#endif
        }
        lib->unload();
        delete lib;
    }
    Q_ASSERT(m_fitness_modules.size());
    std::cout << QObject::trUtf8("Выберите функцию из списка:")
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    for(int i = 0; i < m_fitness_modules.size(); i++)
    {
        std::cout << QObject::trUtf8("%1. Функция \'%2\'")
                     .arg(i)
#if QT_VERSION < 0x040000
                     .arg((*m_fitness_modules.at(i))->name())
                     .local8Bit()
#else
                     .arg(m_fitness_modules.at(i)->name())
                     .toLocal8Bit()
#endif
                     .data() << std::endl;
    }
    int fid = m_fitness_modules.size();
    while(fid >= m_fitness_modules.size())
    {
        std::cout << QObject::trUtf8("Введите номер функции [0..%1] : ")
                     .arg(m_fitness_modules.size()-1)
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> fid;
    }
    int chrsize = 0;
    while(chrsize < 2 || chrsize > 1024)
    {
        std::cout << QObject::trUtf8("Введите длину хромосомы [2..1024] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> chrsize;
    }
    int popsize = 0;
    while(popsize < 2 || popsize > 1024)
    {
        std::cout << QObject::trUtf8("Введите размер популяции [2..1024] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> popsize;
    }
    int limit = 0;
    while(limit < 1 || limit > 10080)
    {
        std::cout << QObject::trUtf8("Введите максимальное время эволюции, минут [1..10080] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> limit;
    }
    int island = -1;
    while(island < 0 || island > 512)
    {
        std::cout << QObject::trUtf8("Введите количество островов [0..512] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> island;
    }
#if QT_VERSION < 0x040000
    IFitness*f = *m_fitness_modules.at(fid);
#else
    IFitness*f = m_fitness_modules.at(fid);
#endif
    CGeneticController*calculator = CGeneticController::getCalculator(f,chrsize,popsize,limit,island);
    CPopulation best = calculator->getBestSolutions(3);
    delete calculator;
    std::cout << QObject::trUtf8("Среднее здоровье   популяции = %1")
                 .arg(best.getAverageFitness())
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    std::cout << QObject::trUtf8("Гомогенность       популяции = %1")
                 .arg(best.getHomogeneity())
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    std::cout << QObject::trUtf8("Псевдогомогенность популяции = %1")
                 .arg(best.getHomogeneity(true))
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    for(int j = 0; j < popsize && j < 3; j++)
    {
        CChromosome chr = best.getChromosome(j);
        char* code = new char[CChromosome::size()+1];
        for(unsigned int i = 0; i < CChromosome::size(); ++i)
        {
            code[i] = chr.getGene(i) ? '1' : '0';
        }
        code[CChromosome::size()] = '\0';
        qDebug( "f(%s)=%f",
                code,
                chr.fitness());
        delete[] code;
    }
    for(int i = 0; i < m_fitness_modules.size(); i++)
    {
#if QT_VERSION < 0x040000
        delete (*m_fitness_modules.at(i));
#else
        delete m_fitness_modules.at(i);
#endif
    }
    m_fitness_modules.clear();
    for(int i = 0; i < m_fitness_libraries.size(); i++)
    {
#if QT_VERSION < 0x040000
        (*m_fitness_libraries.at(i))->unload();
        delete (*m_fitness_libraries.at(i));
#else
        m_fitness_libraries.at(i)->unload();
        delete m_fitness_libraries.at(i);
#endif
    }
    m_fitness_libraries.clear();
    qDebug("--------------------------------------------------------------");
    std::cout << QObject::trUtf8("Для выхода введите \"ok\": ")
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data();
    char chr;
    std::cin >> chr;
}
