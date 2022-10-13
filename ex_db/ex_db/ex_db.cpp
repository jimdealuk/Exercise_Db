// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
#include "hpp/ExerciseData.h"




/* Creates Application "UI"
*  all commands to interact with exercise database
*  invoker that handles the creation of base "UI" 
*  creation and execution of the commands
* 
*  ALL commands classes implement Command base class.
*/
namespace ExercideDbUI
{

    class Command {
    public:
        virtual ~Command() {
        }
        virtual void Execute() const = 0;
    };

    class DisplayTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayTagsCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver){
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {
            std::cout << "List of current tags\n";
            std::vector<std::string> tags;
            m_dataSource->GetTagsDb(tags);
            for (auto tg : tags)
            {
                std::cout << tg << std::endl;
            }
        }
    };

    class DisplayExercisesCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayExercisesCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {
            std::cout << "List of current exercises \n";
            std::vector<CoreData::BaseEx> tags;
            m_dataSource->GetExDb(tags);
            for (auto tg : tags)
            {
                std::cout << tg.exName << std::endl;
            }
        }
    };


    class AddTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddTagsCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {

            std::string tag = { "" };
            bool stopAdding = { false };
            do {
                std::vector<std::string> tags;
                m_dataSource->GetTagsDb(tags);
                for (auto tg : tags)
                {
                    std::cout << tg << std::endl;
                }
                std::cout << "Input new tag - fin tp finish: \n";
                std::getline(std::cin, tag);
                if (tag.length() > 0)
                {
                    if (tag == "fin")
                    {
                        stopAdding = true;
                    }

                    if (!stopAdding)
                    {
                        if (!m_dataSource->AddTag(tag))
                        {
                            std::cout << "tag not added \n";
                        }
                    }
                    std::cout << "*************************** \n";
                }

            } while (!stopAdding);
        }
    };


    class SaveDbToFileCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        SaveDbToFileCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {
            std::cout << "Saving data to file\n";
            m_dataSource->SaveExerciseDb();
        }
    };

    class AddExerciseCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddExerciseCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {

            try 
            {
                std::vector<CoreData::BaseEx> exs;
                m_dataSource->GetExDb(exs);

                for (auto ex : exs)
                {
                    std::cout << ex.exName << "\n";
                }

                std::string exercise = { "" };
                std::cout << "Add exercise name\n";
                std::cin.clear();

                do
                {
                    std::getline(std::cin, exercise);
                } while (exercise.length() == 0);

                std::locale loc;
                std::string exTag = { "" };
                for (auto elem : exercise)
                {
                    char c = std::tolower(elem, loc);
                    if (isalpha(c))
                    {
                        exTag.append(1, c);
                    }
                }

                if (exTag == "fin")
                {
                    return;
                }

                if (m_dataSource->CheckExerciseExists(exTag))
                {
                    return;
                }

                // new exercise - havent finished..
                CoreData::BaseEx exToAdd = { exTag };

                std::string tag = { "" };
                bool stopAdding = { false };
                do {
                    std::cout << "Current tags : \n";
                    std::vector<std::string> tags;
                    m_dataSource->GetTagsDb(tags);
                    for (auto tg : tags)
                    {
                        std::cout << tg << std::endl;
                    }
                    std::cout << "Input new tag - fin to finish: \n";

                    do
                    {
                        std::getline(std::cin, tag);
                    } while (tag.length() == 0);

                    if (tag == "fin")
                    {
                        stopAdding = true;
                    }

                    if (!stopAdding)
                    {
                        if (!m_dataSource->CheckTagsExists(tag))
                        {
                            m_dataSource->AddTag(tag);
                        }

                        auto it = std::find(exToAdd.exTags.begin(), exToAdd.exTags.end(), tag);
                        if (it == exToAdd.exTags.end())
                        {
                            exToAdd.exTags.push_back(tag);
                        }
                    }

                    std::cout << "*************************** \n";

                } while (!stopAdding);
                m_dataSource->AddExercise(exToAdd);
            }
            catch (...)
            {
                std::cout << "Couldn't add exercise \n";
            }
        }
    };


    class Invoker {
        std::shared_ptr <ExerciseDataApp::ExerciseData> m_exDb;


    public:
        Invoker() = default;
        Invoker(Invoker& inv) = delete;
        Invoker(std::shared_ptr<ExerciseDataApp::ExerciseData> receiver) {
            m_exDb = std::move(receiver);
        }

        void StartUI() {
            bool exitMenu = { false };
            std::cout << "welcome to exercise database - choose your option : \n";
            int opt = { 0 };
            std::cout << "1: List Exercises\n";
            std::cout << "2: List Tags\n";
            std::cout << "3: Add Tag \n";
            std::cout << "4: Add Exercise \n";
            std::cout << "5: save Exercises to file \n";
            std::cout << "6: Exit \n";

            std::cin >> opt;
            do {
                
                do {
                    if ((opt < 1) || (opt > 6))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        std::cout << "Invalid Entry - please enter number between 1 & 6\n";
                        std::cin >> opt;
                    }
                } while ((opt < 1) || (opt > 6));

                switch (opt)
                {
                    case 1:
                    {
                        std::unique_ptr<ExercideDbUI::DisplayExercisesCommand> dtc = std::make_unique<ExercideDbUI::DisplayExercisesCommand>(m_exDb.get());
                        dtc->Execute();
                        break;
                    }
                    case 2:
                    {
                        std::unique_ptr<ExercideDbUI::DisplayTagsCommand> dtc = std::make_unique<ExercideDbUI::DisplayTagsCommand>(m_exDb.get());
                        dtc->Execute();
                        break;

                    }
                    case 3:
                    {
                        std::unique_ptr<ExercideDbUI::AddTagsCommand> atc = std::make_unique<ExercideDbUI::AddTagsCommand>(m_exDb.get());
                        atc->Execute();
                        break;

                    }
                    case 4:
                    {
                        std::unique_ptr<ExercideDbUI::AddExerciseCommand> stc = std::make_unique<ExercideDbUI::AddExerciseCommand>(m_exDb.get());
                        stc->Execute();
                        break;

                    }
                    case 5:
                    {
                        std::unique_ptr<ExercideDbUI::SaveDbToFileCommand> stc = std::make_unique<ExercideDbUI::SaveDbToFileCommand>(m_exDb.get());
                        stc->Execute();
                        break;

                    }
                    case 6:
                    default:
                    {
                        exitMenu = true;
                        return;
                    }
                }

                std::cout << "Please Input Next Operation\n";
                std::cin >> opt;

            } while (!exitMenu);

        }

    };

}

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

    struct ExDescription
    {
        std::string exName;
        int exSets;
        int exWeight;
        int exReps;
    } ;

    class AddExerciseToSection
    {
    private:
        std::shared_ptr<CoreData::BaseEx> m_exercises;
        std::unique_ptr<std::vector<ExDescription>> m_userExercises;

    public:

    };

}


int main()
{
    std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> exh = std::make_unique<ExerciseDbHandling::ExerciseDbFileHandler>();
    if (exh)
    {
        std::unique_ptr<ExerciseDataApp::ExerciseData> db = std::make_unique< ExerciseDataApp::ExerciseData>(std::move(exh));

        if (db)
        {
            std::shared_ptr<ExerciseDataApp::ExerciseData> dbShared = std::move(db);

            if(dbShared)
            {
                dbShared->InitialiseExerciseDb();

                std::unique_ptr<ExercideDbUI::Invoker> ui = std::make_unique < ExercideDbUI::Invoker>(dbShared);
                if (ui)
                {
                    ui->StartUI();
                }
            }
        }
    }

}
