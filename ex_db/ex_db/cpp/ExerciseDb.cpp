// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#include <locale>  // std::locale, std::tolower

#include "../hpp/ExerciseDb.h"
#include "../hpp/ExerciseDbFileHandler.h"

namespace ExerciseDbClass
{
    /* ExerciseDb constructor
    *  pass in db handle and use it to set the class db handle
    */
    ExerciseDb::ExerciseDb(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle)
    {
        m_exercisesBase = std::move(dbHandle);
    }

    /* LoadExercisesFromDb
    *  create the exercise, tags and sections containers
    *  read the data in to the database
    *  populate the exercise, tags and sections containers from the database
    */
    bool ExerciseDb::LoadExercisesFromDb()
    {
        m_exercises = std::make_unique< std::vector<CoreData::BaseEx> >();
        m_tags = std::make_unique< std::vector<std::string> >();

        bool ret = m_exercisesBase->ReadExDb();

        m_exercisesBase->GetExDb(*(m_exercises.get()));
        m_exercisesBase->GetExTags(*(m_tags.get()));

        return ret;
    }

    /* BackupExercsesToDb
    *  Call relevant method on database handle to write all the data to 
    *  the relevant storage mechanism
    */
    bool ExerciseDb::BackupExercsesToDb()
    {
        return m_exercisesBase->WriteExDb();
    }

    /* CheckExerciseExists
    *  Check to see if an exercise already exists
    */
    bool ExerciseDb::CheckExerciseExists(const std::string& exStr)
    {
        bool ret = { false };

        try
        {
            auto it = std::find_if(m_exercises->begin(), m_exercises->end(), [exStr](CoreData::BaseEx& f) { return f.exName == exStr; });
            if (it != m_exercises->end())
            {
                ret = true;
            }
        }
        catch (...)
        {
            // ret already set
        }

        return ret;
    }

    /* CheckTagExists
    *  Check to see if an tag already exists
    */
    bool ExerciseDb::CheckTagExists(const std::string& exStr)
    {
        bool ret = { false };

        try
        {
            auto it = std::find_if(m_tags->begin(), m_tags->end(), [exStr](std::string& f) { return f == exStr; });
            if (it != m_tags->end())
            {
                ret = true;
            }
        }
        catch (...)
        {
            // ret already set
        }
        return ret;
    }

    /* AddExercise
    *  Add an exercise to the exercise container
    *  Replace the exercise container in the database
    *  to ensure consistency with the database replace current
    *  exercise container with the one in the database
    *  NOTE: it is NOT anticipated that there will be millions of
    *  exercises in the exercise database : if there were this may be
    *  a heavy handed approach to esuring consistency of data between
    *  the database and the rest of the application
    */
    bool ExerciseDb::AddExercise(const CoreData::BaseEx& ex)
    {
        m_exercises->push_back(ex);
        m_exercisesBase->SetExercises(std::move(m_exercises));

        m_exercises = std::make_unique< std::vector<CoreData::BaseEx> >();
        m_exercisesBase->GetExDb(*(m_exercises.get()));

        return true;
    }

    /* AddTag
    *  Add an tag to the tags container
    *  Replace the tags container in the database
    *  to ensure consistency with the database replace current
    *  tag container with the one in the database
    *  NOTE: it is NOT anticipated that there will be millions of
    *  tags in the tags database : if there were this may be
    *  a heavy handed approach to esuring consistency of data between
    *  the database and the rest of the application
    */
    bool ExerciseDb::AddTag(const std::string& tag)
    {
        bool ret = { false };

        try
        {
            std::locale loc;
            std::string exTag = { "" };
            for (auto elem : tag)
            {
                char c = std::tolower(elem, loc);
                if (isalpha(c))
                {
                    exTag.append(1, c);
                }
            }

            auto it = std::find(m_tags->begin(), m_tags->end(), exTag);
            if (it == m_tags->end())
            {
                m_tags->push_back(exTag);
                m_exercisesBase->SetExTags(std::move(m_tags));

                m_tags = std::make_unique< std::vector<std::string> >();
                m_exercisesBase->GetExTags(*(m_tags.get()));

                ret = true;
            }
        }
        catch (...)
        {
            // ret already set
        }

        return ret;
    }

    // set reference to tags container from the tags container in the database
    void ExerciseDb::GetExTags(std::vector<std::string>& tagsHandle)
    {
        m_exercisesBase->GetExTags(tagsHandle);
    }

 
    // set reference to sections container from the sections container in the database
    void ExerciseDb::GetExDb(std::vector<CoreData::BaseEx>& exDbHandle)
    {
        m_exercisesBase->GetExDb(exDbHandle);
    }
}

