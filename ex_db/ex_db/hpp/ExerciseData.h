#ifndef ExerciseData_H
#define ExerciseData_H

#include <memory>
#include <vector>

#include "ExerciseDb.h"
#include "ExerciseDbHandlerBase.h"

namespace ExerciseDataApp
{
    /* Wrapper round the exercise database :
    *  creates database and handles commands to-
    *  load / initialise the database
    *  handle commands to extract, update & save exercises
    */
    class ExerciseData
    {
    private:
        // a handle to the exercise database storage
        std::unique_ptr <ExerciseDbClass::ExerciseDb> m_exDb;

    public:
        ExerciseData() = default;
        ExerciseData(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle);

        // initialise the exercise, tags and sections containers from
        // the database handler
        bool InitialiseExerciseDb();

        // save the exercise, tags and sections containers to
        // the database handler
        bool SaveExerciseDb();

        // Get the exercises and tags containers from the database
        void GetTagsDb(std::vector<std::string>& tags);
        void GetExDb(std::vector<CoreData::BaseEx>& dbHandle);

        // Add a new tag to the tags database
        bool AddTag(std::string tag);

        // Add a new exercise to the exercise database
        bool AddExercise(CoreData::BaseEx ex);

        // Check whether an exercise or a tag already exists
        bool CheckExerciseExists(std::string ex);
        bool CheckTagsExists(std::string ex);
    };

}


#endif /* !ExerciseData_H */
