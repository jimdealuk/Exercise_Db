
#include <locale>  // std::locale, std::tolower

#include "../hpp/ExerciseDb.h"
#include "../hpp/ExerciseDbFileHandler.h"

namespace ExerciseDbClass
{

    ExerciseDb::ExerciseDb(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle)
    {
        m_exercisesBase = std::move(dbHandle);
    }

    bool ExerciseDb::LoadExercisesFromDb()
    {
        m_exercises = std::make_unique< std::vector<CoreData::BaseEx> >();
        m_tags = std::make_unique< std::vector<std::string> >();
        m_workoutSectiontags = std::make_unique< std::vector<std::string> >();

        bool ret = m_exercisesBase->ReadExDb();

        m_exercisesBase->GetExDb(*(m_exercises.get()));
        m_exercisesBase->GetExTags(*(m_tags.get()));
        m_exercisesBase->GetWorkoutSection(*(m_workoutSectiontags.get()));

        return ret;
    }

    bool ExerciseDb::BackupExercsesToDb()
    {
        return m_exercisesBase->WriteExDb();
    }


    bool ExerciseDb::CheckExerciseExists(std::string exStr)
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

    bool ExerciseDb::CheckTagExists(std::string exStr)
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

    bool ExerciseDb::AddExercise(CoreData::BaseEx ex)
    {
        m_exercises->push_back(ex);
        m_exercisesBase->SetExercises(std::move(m_exercises));

        m_exercises = std::make_unique< std::vector<CoreData::BaseEx> >();
        m_exercisesBase->GetExDb(*(m_exercises.get()));

        return true;
    }

    bool ExerciseDb::AddTag(std::string tag)
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

    void ExerciseDb::GetExTags(std::vector<std::string>& tagsHandle)
    {
        m_exercisesBase->GetExTags(tagsHandle);
    }

    void ExerciseDb::GetWorkoutSection(std::vector<std::string>& tagsHandle)
    {
        m_exercisesBase->GetWorkoutSection(tagsHandle);
    }

    void ExerciseDb::GetExDb(std::vector<CoreData::BaseEx>& exDbHandle)
    {
        m_exercisesBase->GetExDb(exDbHandle);
    }
}

