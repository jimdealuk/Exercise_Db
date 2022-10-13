

#include "../hpp/ExerciseData.h"

namespace ExerciseDataApp
{

    ExerciseData::ExerciseData(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle)
    {
        std::unique_ptr<ExerciseDbClass::ExerciseDb> db = std::make_unique<ExerciseDbClass::ExerciseDb>(std::move(dbHandle));
        m_exDb = std::move(db);
    }

    bool ExerciseData::InitialiseExerciseDb()
    {
        return m_exDb->LoadExercisesFromDb();
    }

    bool ExerciseData::SaveExerciseDb()
    {
        return m_exDb->BackupExercsesToDb();
    }

    void ExerciseData::GetTagsDb(std::vector<std::string>& tags)
    {
        m_exDb->GetExTags(tags);
    }

    void ExerciseData::GetExDb(std::vector<CoreData::BaseEx>& dbHandle)
    {
        m_exDb->GetExDb(dbHandle);
    }

    bool ExerciseData::AddTag(std::string tag)
    {
        return m_exDb->AddTag(tag);
    }

    bool ExerciseData::AddExercise(CoreData::BaseEx ex)
    {
        return m_exDb->AddExercise(ex);
    }

    bool ExerciseData::CheckExerciseExists(std::string ex)
    {
        return m_exDb->CheckExerciseExists(ex);
    }
    bool ExerciseData::CheckTagsExists(std::string ex)
    {
        return m_exDb->CheckTagExists(ex);
    }
}
