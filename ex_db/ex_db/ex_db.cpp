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
//#include "hpp/ExerciseData.h"
#include "hpp/ExercideDbUI.h"


namespace ExerciseTemplates
{
    class Sections
    {
    private:
        std::vector<std::string> m_sections;

    public:
        Sections() = default;
        Sections(Sections& sec) = delete;

        void GetSections(std::vector<std::string>& secs)
        {
            secs = m_sections;
        }

        bool AddSection(std::string sec)
        {
            bool ret = { false };
            auto it = std::find(m_sections.begin(), m_sections.end(), sec);
            if (it == m_sections.end())
            {
                m_sections.push_back(sec);
                ret = true;
            }
            return ret;
        }
    };


    class AddExerciseToSection
    {
    private:
        std::shared_ptr<CoreData::BaseEx> m_exercises;
        std::unique_ptr<std::vector<CoreData::ExDescription>> m_userExercises;

    public:

    };

}


int main()
{
    std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> exh = std::make_unique<ExerciseDbHandling::ExerciseDbFileHandler>();
    if (exh)
    {
        std::unique_ptr<ExerciseDbClass::ExerciseDb> db = std::make_unique< ExerciseDbClass::ExerciseDb>(std::move(exh));

        if (db)
        {
            std::shared_ptr<ExerciseDbClass::ExerciseDb> dbShared = std::move(db);

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
