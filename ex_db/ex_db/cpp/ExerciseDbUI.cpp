// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#include <iostream>
#include <locale>  // std::locale, std::tolower

#include "../hpp/ExerciseDbUI.h"

namespace ExercideDbUI
{
	DisplayTagsCommand::DisplayTagsCommand(ExerciseDbClass::ExerciseDb* receiver) 
        : m_dataSource(receiver) 
    {
	}

    /* Command to list all the tags in the tags container 
    *  taken from the exercise database
    */
    void DisplayTagsCommand::Execute() const 
    {
        std::cout << "List of current tags\n";
        std::vector<std::string> tags;
        m_dataSource->GetExTags(tags);
        for (auto tg : tags)
        {
            std::cout << tg << std::endl;
        }
    }


    DisplayExercisesCommand::DisplayExercisesCommand(ExerciseDbClass::ExerciseDb* receiver) 
        : m_dataSource(receiver) 
    {
    }

    /* Command to list all the exercises in the exercises container
    *  taken from the exercise database
    */
    void DisplayExercisesCommand::Execute() const
    {
        std::cout << "List of current exercises \n";
        std::vector<CoreData::BaseEx> tags;
        m_dataSource->GetExDb(tags);
        for (auto tg : tags)
        {
            std::cout << tg.exName << std::endl;
        }
    }


    AddTagsCommand::AddTagsCommand(ExerciseDbClass::ExerciseDb* receiver) 
        : m_dataSource(receiver) 
    {
    }

    /* Command to add tags in the tags container
    */
    void AddTagsCommand::Execute() const
    {
        std::string tag = { "" };
        bool stopAdding = { false };
        do {
            std::vector<std::string> tags;
            m_dataSource->GetExTags(tags);
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
    };


    SaveDbToFileCommand::SaveDbToFileCommand(ExerciseDbClass::ExerciseDb* receiver) 
        : m_dataSource(receiver) 
    {
    }

    /* Command to save the tags, exercises and sections
    * to the storage mechanism - from where they were loaded
    */
    void SaveDbToFileCommand::Execute() const
    {
        std::cout << "Saving data to file\n";
        m_dataSource->BackupExercsesToDb();
    }


    AddExerciseCommand::AddExerciseCommand(ExerciseDbClass::ExerciseDb* receiver)
        : m_dataSource(receiver) 
    {
    }

    /* Command to add an exercise in the exercise container
    */
    void AddExerciseCommand::Execute() const  {

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
                m_dataSource->GetExTags(tags);
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
                    if (!m_dataSource->CheckTagExists(tag))
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


    GetExerciseForTagCommand::GetExerciseForTagCommand(ExerciseDbClass::ExerciseDb* receiver)
        : m_dataSource(receiver)
    {
    }

    /* Command to get a list of exercises for a tag
    */
    void GetExerciseForTagCommand::Execute() const {

        try
        {
            std::cout << "Input new tag - fin to finish: \n";
            bool stopAdding = { false };
            std::string tag = { "" };
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
                if (m_dataSource->CheckTagExists(tag))
                {
                    std::vector<std::string> exs;
                    m_dataSource->GetExercisesForTag(tag, exs);

                    for (auto& e : exs)
                    {
                        std::cout << e << "\n";
                    }
                }
                else
                {
                    std::cout << "Tag does not exist\n";
                }
            }

        }
        catch (...)
        {
            std::cout << "Couldn't add exercise \n";
        }
    }


    Invoker::Invoker(std::shared_ptr<ExerciseDbClass::ExerciseDb> receiver)
    {
        m_exDb = std::move(receiver);
    }

    /* Method to create and start the UI
    *  As an option is selected, the relevant command is created and executed
    */
    void Invoker::StartUI() {
        bool exitMenu = { false };
        std::cout << "welcome to exercise database - choose your option : \n";
        int opt = { 0 };
        std::cout << "1: List Exercises\n";
        std::cout << "2: List Tags\n";
        std::cout << "3: Add Tag \n";
        std::cout << "4: Add Exercise \n";
        std::cout << "5: save Exercises to file \n";
        std::cout << "6: list exercises for tag \n";
        std::cout << "7: Exit \n";

        std::cin >> opt;
        do {

            do {
                if ((opt < 1) || (opt > 7))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "Invalid Entry - please enter number between 1 & 7\n";
                    std::cin >> opt;
                }
            } while ((opt < 1) || (opt > 7));

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
            {
                std::unique_ptr<ExercideDbUI::GetExerciseForTagCommand> stc = std::make_unique<ExercideDbUI::GetExerciseForTagCommand>(m_exDb.get());
                stc->Execute();
                break;
            }
            case 7:
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



}