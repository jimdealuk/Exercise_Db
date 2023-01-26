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
    void AddExerciseCommand::Execute() const  
    {
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
    void GetExerciseForTagCommand::Execute() const 
    {
        try
        {
            std::cout << "Input tag name - fin to finish: \n";
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
            std::cout << "Couldn't get exercise list\n";
        }
    }


    CreateWorkoutCommand::CreateWorkoutCommand(ExerciseDbClass::ExerciseDb* receiver, WorkoutBuilder::BuildWorkoutImpl* workoutBuilder)
        : m_dataSource(receiver), m_workoutBuilder(workoutBuilder)
    {

    }

    /* Command to create a workout
    * This creates a workout with a single section & multiple (weight) exercises
    */
    void CreateWorkoutCommand::Execute() const
    {
        try
        {
            std::cout << "Create new workout name - fin to finish: \n";
            bool stopAdding = { false };
            std::string name = { "" };
            do
            {
                std::getline(std::cin, name);
            } while (name.length() == 0);

            if (name == "fin")
            {
                stopAdding = true;
            }


            if (!stopAdding)
            {
                m_workoutBuilder->BuildWorkout(name);

                do
                {
                    bool stopAddingSection = { false };

                    do
                    {
                        bool stopAddingExercise = { false };

                        std::cout << "Section name - fin to finish: \n";
                        std::string sectionName = { "" };
                        do
                        {
                            std::getline(std::cin, sectionName);
                        } while (sectionName.length() == 0);

                        if (sectionName == "fin")
                        {
                            stopAddingSection = true;
                            stopAdding = true;
                        }

                        if (!stopAddingSection)
                        {
                            CoreData::WorkoutComponent* section = new CoreData::WorkoutComponent;
                            section->SetName(sectionName);
                            section->SetCompType(CoreData::sectionTags);
                            m_workoutBuilder->BuildSections(name, section);

                            do
                            {

                                std::cout << "Add exercise to section created - fin to finish: \n";
                                std::string exName = { "" };
                                do
                                {
                                    std::getline(std::cin, exName);
                                } while (exName.length() == 0);

                                if (exName == "fin")
                                {
                                    stopAddingExercise = true;
                                }

                                if (!stopAddingExercise)
                                {
                                    CoreData::WeightEx* ex1 = new CoreData::WeightEx();
                                    ex1->SetName(exName);
                                    ex1->SetCompType(CoreData::tagBase);
                                    m_workoutBuilder->BuildExerciseList(sectionName, ex1);
                                }
                            } while (!stopAddingExercise);
                        }


                    } while (!stopAddingSection);


                } while (!stopAdding);

                m_dataSource->AddWorkout(m_workoutBuilder->GetCreatedWorkout());

            }
        }
        catch (...)
        {
            std::cout << "Couldn't add workout \n";
        }
    }

    ListWorkoutsCommand::ListWorkoutsCommand(ExerciseDbClass::ExerciseDb* receiver)
        : m_dataSource(receiver)
    {

    }

    /* Command to get a list of workout names
    */
    void ListWorkoutsCommand::Execute() const
    {
        try
        {
            std::cout << "List of current workouts \n";
            CoreData::WorkoutComponent wc;
            m_dataSource->GetWorkouts(wc);

            std::list<std::shared_ptr<CoreData::Component>> c = wc.GetChildrenCopy();

            for (auto& w : c)
            {
                std::cout << w->GetName() << "\n";
            }
        }
        catch (...)
        {
            std::cout << "Couldn't get exercise list\n";
        }
    }

    RemoveTagFromTagDbCommand::RemoveTagFromTagDbCommand(ExerciseDbClass::ExerciseDb* receiver)
        : m_dataSource(receiver)
    {
    }

    /* Command to get a list of exercises for a tag
    */
    void RemoveTagFromTagDbCommand::Execute() const
    {
        // first of all list the current tags
        std::cout << "List of current tags\n";
        std::vector<std::string> tags;
        m_dataSource->GetExTags(tags);
        for (auto tg : tags)
        {
            std::cout << tg << std::endl;
        }

        // now - MANUALLY - enter tag to delete...

        // ... the : remove the tag from the tags database...



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
        std::cout << "5: Save Exercises to File \n";
        std::cout << "6: List Exercises for Tag \n";
        std::cout << "7: List Workouts \n";
        std::cout << "8: Create Workout \n";

        std::cout << "9: Exit \n";

        std::cin >> opt;
        do {

            do {
                if ((opt < 1) || (opt > 9))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "Invalid Entry - please enter number between 1 & 9\n";
                    std::cin >> opt;
                }
            } while ((opt < 1) || (opt > 9));

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
            {
                std::unique_ptr<ExercideDbUI::ListWorkoutsCommand> stc = std::make_unique<ExercideDbUI::ListWorkoutsCommand>(m_exDb.get());
                stc->Execute();
                break;
            }
            case 8:
            {
                WorkoutBuilder::BuildWorkoutImpl* wb = new WorkoutBuilder::BuildWorkoutImpl();
                std::unique_ptr<ExercideDbUI::CreateWorkoutCommand> stc = std::make_unique<ExercideDbUI::CreateWorkoutCommand>(m_exDb.get(), wb);
                stc->Execute();
                break;
            }
            case 9:
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