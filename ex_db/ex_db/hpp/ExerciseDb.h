#ifndef ExerciseDbHandling_H
#define ExerciseDbHandling_H

#include <memory>
#include <vector>

#include "CoreData.h"
#include "ExerciseDbHandlerBase.h"

namespace ExerciseDbClass
{
    /* Wrapper round the exercise data class :
    *  Processes commands
    *  - extract exercise data for display
    *  - on exercise data addition it ensures:
    *   no duplicates are stored
    *   updates the stored exercise data
    */
    class ExerciseDb
    {
    private:
        std::unique_ptr<std::vector<CoreData::BaseEx>> m_exercises;
        std::unique_ptr<std::vector<std::string>> m_tags;

        std::unique_ptr<std::vector<std::string>> m_workoutSectiontags;

        std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> m_exercisesBase;

    public:
        ExerciseDb() = default;
        ExerciseDb(std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle);

        bool LoadExercisesFromDb();
        bool BackupExercsesToDb();
        ~ExerciseDb() = default;

        void GetExTags(std::vector<std::string>& tagsHandle);
        void GetExDb(std::vector<CoreData::BaseEx>& exDbHandle);
        void GetWorkoutSection(std::vector<std::string>& tagsHandle);

        bool AddTag(std::string tag);

        bool CheckExerciseExists(std::string exStr);
        bool CheckTagExists(std::string exStr);
        bool AddExercise(CoreData::BaseEx ex);
    };

}


#endif /* !ExerciseDbHandling_H */
