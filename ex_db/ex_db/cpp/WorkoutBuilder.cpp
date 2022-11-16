// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#include "..\hpp\WorkoutBuilder.h"
#include "..\hpp\CoreData.h"


namespace WorkoutBuilder
{


    BuildWorkoutImpl::BuildWorkoutImpl()
    {
        m_workouts = std::make_shared<CoreData::WorkoutComponent>();
    }

    void BuildWorkoutImpl::BuildWorkout(std::string& workoutName)
    {
        m_workouts->SetName(workoutName);
        m_workouts->SetCompType(CoreData::workoutName);
    }

    void BuildWorkoutImpl::BuildSections(std::string& workoutName, CoreData::Component* section)
    {
        if (workoutName == m_workouts->GetName())
        {
            m_workouts->Add(section);
        }
    }

    void BuildWorkoutImpl::BuildExerciseList(std::string& sectionName, CoreData::Component* ex)
    {
        m_workouts->AddExerciseToSection(sectionName, ex);
    }

}

