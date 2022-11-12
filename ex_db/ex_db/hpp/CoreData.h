// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef CoreData_H
#define CoreData_H

#include <string>
#include <list>


#include <iostream>

namespace CoreData
{
    /* These are strings that are used throughout the application
    */
    const std::string filePath = { "../exercisedb/" };
    const std::string file = { "../exercisedb/exercises.db" };

    const std::string tempfile = { "../exercisedb/tempexercises.db" };


    const std::string dataIn = { "{" };
    const std::string dataOut = { "}" };
    const std::string lineSep = { "\n" };

    const std::string sepIn = { "(" };
    const std::string sepOut = { ")" };
    const std::string exIn = { "[" };
    const std::string exOut = { "]" };
    const std::string exSep = { ":" };
    const std::string tagSep = { "," };

    const std::string tagBase = { "Exercises" };
    const std::string tagTags = { "Tags" };
    const std::string sectionTags = { "Section" };
    const std::string workoutTags = { "Workouts" };

    /* Data Type : BaseEx
    *  Structure containing exercise name and it's associated tags
    *  Tags here can be considered categories : what kind of workouts
    *  would you associate this exercise with.
    *  ex: the Squat would be associated with "Power" and "General" types
    *  of workout
    */
    struct BaseEx
    {
        std::string exName;
        std::vector<std::string> exTags;
    };


    /* Data Type : ExDescription
    *  Structure containing exercise name
    */
    struct ExDescription
    {
        std::string exName;
    };

    /* Data Type : WeightExercise
    *  Structure containing exercise name add
    *  weight to be used
    *  number of repetitions
    *  number of sets
    */
    struct WeightExercise : ExDescription
    {
        int exSets = 0;
        int exWeight = 0;
        int exReps = 0;
    };

    /* Data Type : RowExercise
    *  Structure containing exercise name add
    *  distance : distance to be rowed
    *  time : duration of exercise
    *  difficulty : at the moment - an integer (range unspecified)
    */
    struct RowExercise : ExDescription
    {
        int distanceInKm = 0;
        int timeInMins = 0;
        int difficulty = 0;
    };


// https://stackoverflow.com/questions/4915970/c-casting-to-derived-and-parent-structs


    /* Data Type : WorkoutSection
    *  Structure containing section name and
    *  exercises contained in this section
    */
    struct WorkoutSection 
    {
        std::string name = { "" };
        std::vector<ExDescription> excercises;
    };

    /* Data Type : WorkoutBase
    *  Structure containing Workout name 
    */
    struct WorkoutBase {
        std::string name = { "" };

        bool operator==(const WorkoutBase& rhs) const
        {
            return name == rhs.name;
        }

        bool operator!=(const WorkoutBase& rhs) const
        {
            return name != rhs.name;
        }

    };

    /* Data Type : Workout
    *  Structure containing Workout name and
    *  sections contained in this workout
    */
    struct Workout : WorkoutBase
    {
        std::vector<WorkoutSection> sections;

        //bool operator==(const Workout& rhs) const
        //{
        //    return name == rhs.name;
        //}


    };

    /* Data Type : QuickWorkout
    *  This structure may be used for a rehab program that does not require sections
    *  Structure containing Workout name and
    *  exercises contained in this workout
    */
    struct QuickWorkout : WorkoutBase
    {
        std::vector<ExDescription> excercises;
    };



    /* Component based structure
    *  This will replace above data structure(s)
    *  This is based on the Composite Design pattern
    */
    class Component {
    protected:
        std::string m_name;
    public:
        virtual ~Component() {}

        virtual void Add(Component* component) {}
        virtual void Remove(Component* component) {}

        virtual bool IsComposite() const {
            return false;
        }

        virtual void SetName(std::string name) { m_name = name; }
        virtual std::string GetName() { return m_name; }

        virtual void print() {}

    };


    /* Place holder class that allows different kinds of exercise 
    *  to be created - but also added to different workout structures
    */
    class ExBase : public Component {

    };


    /* Free weight exercise
    *  Data needed : weight / sets / reps per set
    */
    class WeightEx : public ExBase
    {
    public:
        int exSets = 0;
        int exWeight = 0;
        int exReps = 0;
    };

    /* Rowing exercise
    *  Data needed : 
    *  distance to row 
    *  time spent / target time (?)
    *  diffculty : bad conditions / rowing machine difficulty setting (?)
    */
    struct RowEx : public ExBase
    {
    public:
        int distanceInKm = 0;
        int timeInMins = 0;
        int difficulty = 0;
    };


    /* WorkoutComponent
    *  implementation based on Component structure
    *  As with Composite pattern - Components are interchangeable
    */
    class WorkoutComponent : public Component {

    protected:
        std::list<std::unique_ptr<Component>> m_children;

    public:
        void Add(Component* component) override {
            this->m_children.push_back(std::unique_ptr<Component>(std::move(component)));
        }

        void Remove(Component* component) override {
            this->m_children.remove(std::unique_ptr<Component>(std::move(component)));
        }

        bool IsComposite() const override {
            return true;
        }

        // TEST METHOD - for dev
        void print() override
        {
            std::cout << GetName() << std::endl;

            for (auto& w : m_children)
            {
                std::cout << w->GetName() << std::endl;
            }
        }

        virtual void SetName(std::string name)
        {
            m_name = name;
        }
        virtual std::string GetName()
        {
            return m_name;
        }

    };



}

#endif /* !CoreData_H */