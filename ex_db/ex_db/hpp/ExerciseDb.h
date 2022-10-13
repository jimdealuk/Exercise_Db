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
        // copies of the exercise, tag and section containers populated from the 
        // exercises database
        std::unique_ptr<std::vector<CoreData::BaseEx>> m_exercises;
        std::unique_ptr<std::vector<std::string>> m_tags;
        std::unique_ptr<std::vector<std::string>> m_workoutSectiontags;

        // a handle to the exercise database storage
        std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> m_exercisesBase;

    public:
        ExerciseDb() = default;
        ExerciseDb(std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle);
        ~ExerciseDb() = default;

        // Retreive and store the containers of exercises, tags and sections data
        // from the database storage
        bool LoadExercisesFromDb();
        bool BackupExercsesToDb();

        // get a copy of the exercise, tag and sections containers
        void GetExTags(std::vector<std::string>& tagsHandle);
        void GetExDb(std::vector<CoreData::BaseEx>& exDbHandle);
        void GetWorkoutSection(std::vector<std::string>& tagsHandle);

        // Add a new tag to the tags database
        bool AddTag(std::string tag);

        // Add a new exercise to the exercise database
        bool AddExercise(CoreData::BaseEx ex);

        // Check whether an exercise or a tag already exists
        bool CheckExerciseExists(std::string exStr);
        bool CheckTagExists(std::string exStr);
    };

}


#endif /* !ExerciseDbHandling_H */
