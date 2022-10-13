// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#include "../hpp/ExerciseData.h"

namespace ExerciseDataApp
{
    // ExerciseData ctor : create class exercise db passing in exercise db handle
    ExerciseData::ExerciseData(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle)
    {
        std::unique_ptr<ExerciseDbClass::ExerciseDb> db = std::make_unique<ExerciseDbClass::ExerciseDb>(std::move(dbHandle));
        m_exDb = std::move(db);
    }

    // load exercises, tags and sections from the relevant storage mechanism
    bool ExerciseData::InitialiseExerciseDb()
    {
        return m_exDb->LoadExercisesFromDb();
    }

    // save exercises, tags and sections to the relevant storage mechanism
    bool ExerciseData::SaveExerciseDb()
    {
        return m_exDb->BackupExercsesToDb();
    }

    // set reference to tags container from the tags container in the database
    void ExerciseData::GetTagsDb(std::vector<std::string>& tags)
    {
        m_exDb->GetExTags(tags);
    }

    // set reference to exercise container from the exercise container in the database
    void ExerciseData::GetExDb(std::vector<CoreData::BaseEx>& dbHandle)
    {
        m_exDb->GetExDb(dbHandle);
    }

    // add an tag to the tag database
    bool ExerciseData::AddTag(std::string tag)
    {
        return m_exDb->AddTag(tag);
    }

    // add an exercise to the exercise database
    bool ExerciseData::AddExercise(CoreData::BaseEx ex)
    {
        return m_exDb->AddExercise(ex);
    }

    // Check to see if an exercise already exists
    bool ExerciseData::CheckExerciseExists(std::string ex)
    {
        return m_exDb->CheckExerciseExists(ex);
    }

    // Check to see if an tag already exists
    bool ExerciseData::CheckTagsExists(std::string ex)
    {
        return m_exDb->CheckTagExists(ex);
    }
}
