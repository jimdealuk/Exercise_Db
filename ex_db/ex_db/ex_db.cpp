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
            std::shared_ptr < ExercideDbUI::BuildWorkoutImpl> bw = std::make_shared< ExercideDbUI::BuildWorkoutImpl>();

            std::string jim = { "jim" };
            bw->BuildWorkout(jim);

            CoreData::WorkoutComponent* sec1 = new CoreData::WorkoutComponent();
            std::string warmup = { "warmup" };
            sec1->SetName(warmup);
            sec1->SetCompType(CoreData::sectionTags);

            CoreData::WorkoutComponent* sec2 = new CoreData::WorkoutComponent();
            std::string main = { "main" };
            sec2->SetName(main);
            sec2->SetCompType(CoreData::sectionTags);


            bw->BuildSections(jim, sec1);
            bw->BuildSections(jim, sec2);


            CoreData::WeightEx* ex1 = new CoreData::WeightEx();
            std::string press = { "press" };
            ex1->SetName(press);
            ex1->SetCompType(CoreData::tagBase);
            bw->BuildExerciseList(warmup, ex1);


            CoreData::WeightEx* ex2 = new CoreData::WeightEx();
            std::string clean = { "clean" };
            ex2->SetName(clean);
            ex2->SetCompType(CoreData::tagBase);
            bw->BuildExerciseList(warmup, ex2);


            CoreData::WeightEx* ex3 = new CoreData::WeightEx();
            std::string squat = { "squat" };
            ex3->SetName(squat);
            ex3->SetCompType(CoreData::tagBase);
            bw->BuildExerciseList(main, ex3);


            CoreData::WorkoutComponent* sec3 = new CoreData::WorkoutComponent();
            std::string stretch = { "stretch" };
            sec3->SetName(stretch);
            sec3->SetCompType(CoreData::sectionTags);
            bw->BuildSections(jim, sec3);


            CoreData::StretchEx* ex4 = new CoreData::StretchEx();
            ex4->SetName(stretch);
            ex4->SetCompType(CoreData::tagBase);
            bw->BuildExerciseList(stretch, ex4);


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
