// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#include <iostream>
#include <fstream>

#include <string>
#include <locale>  // std::locale, std::tolower
#include <cassert> // for assert()

#include <vector>

#include <filesystem>

#include "hpp/CoreData.h"
#include "hpp/ExerciseDbHandlerBase.h"
#include "hpp/ExerciseDbFileHandler.h"
#include "hpp/ExerciseDb.h"
#include "hpp/ExerciseDbUI.h"
#include "hpp/WorkoutBuilder.h"

#include "hpp/ExercideDbFileHandlerFactory.h"


int main()
{

    ExerciseDbFileHandlerFactory::ExerciseDbFileHandlerFactory* fileHandlerFactory = new ExerciseDbFileHandlerFactory::ExerciseDbFileHandlerFactory();
    if(fileHandlerFactory)
    {
        std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> exh = fileHandlerFactory->FactoryMethod();
        if (exh)
        {
            std::unique_ptr<ExerciseDbClass::ExerciseDb> db = std::make_unique< ExerciseDbClass::ExerciseDb>(std::move(exh));

            if (db)
            {
                std::shared_ptr<ExerciseDbClass::ExerciseDb> dbShared = std::move(db);

                if (dbShared)
                {
                    dbShared->LoadExercisesFromDb();

                    std::unique_ptr<ExercideDbUI::Invoker> ui = std::make_unique < ExercideDbUI::Invoker>(dbShared);
                    if (ui)
                    {
                        ui->StartUI();
                    }
                }
            }
        }
    }

}
