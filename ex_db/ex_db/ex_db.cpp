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



int main()
{
    std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> exh = std::make_unique<ExerciseDbHandling::ExerciseDbFileHandler>();
    if (exh)
    {
        std::unique_ptr<ExerciseDbClass::ExerciseDb> db = std::make_unique< ExerciseDbClass::ExerciseDb>(std::move(exh));

        if (db)
        {
            std::shared_ptr<ExerciseDbClass::ExerciseDb> dbShared = std::move(db);

// TO DELETE - CODE TO TEST Workout Builder(s)
            std::shared_ptr < ExercideDbUI::BuildWorkoutImpl> bw = std::make_shared< ExercideDbUI::BuildWorkoutImpl>(dbShared);

            CoreData::Workout* w1 = new CoreData::Workout;
            CoreData::WorkoutSection s2 = { "main" };
            std::vector<CoreData::WorkoutSection> sections;
            sections.push_back(s2);
            w1->name = "jim";
            w1->sections = sections;

            CoreData::Workout* w2 = new CoreData::Workout;
            w2->name = "bill";

            CoreData::Workout* w3 = new CoreData::Workout;
            w3->name = "ted";

            bw->BuildWorkout(w1);
            bw->BuildWorkout(w2);
            bw->BuildWorkout(w3);

            CoreData::WorkoutSection* section = new CoreData::WorkoutSection;
            section->name = "warmup";

            bw->BuildSections(section, w1);

            int y = 0;
// DELETE TO HERE..

            if(dbShared)
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
