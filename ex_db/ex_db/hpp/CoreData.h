// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef CoreData_H
#define CoreData_H

#include <string>
#include <vector>
#include <list>


#include <iostream>
#include <fstream>


namespace CoreData
{
    /* These are strings that are used throughout the application
    */
    const std::string filePath = { "../exercisedb/" };
    const std::string file = { "../exercisedb/exercises.db" };

    const std::string tempfile = { "../exercisedb/tempexercises.db" };
    const std::string tempfilealt = { "../exercisedb/tempexercisesalt.db" };


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
    const std::string workoutName = { "WorkoutName" };
    const std::string topName = { "WorkoutStore" };


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


    /* Component based structure
    *  This will replace above data structure(s)
    *  This is based on the Composite Design pattern
    */
    class Component {
    protected:
        std::string m_name;
        std::string m_componentType;

    public:
        virtual ~Component() {}

        virtual void Add(Component* component) {}
        virtual void Remove(Component* component) {}

        virtual bool IsComposite() const { return false; }

        virtual void SetName(std::string name) { m_name = name; }
        virtual std::string GetName() { return m_name; }

        virtual void SetCompType(std::string compType) { m_componentType = compType; }
        virtual std::string GetCompType() { return m_componentType; }

        virtual void print() {}

        virtual void SendToFile(std::fstream& fs) { fs << m_name; }

        virtual std::list<std::shared_ptr<Component>> GetChildrenCopy() { return std::list<std::shared_ptr<Component>>(); }
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

        void SendToFile(std::fstream& fs) override
            { fs << m_name << " " << exSets << " " << exWeight << " " << exReps; }

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

        void SendToFile(std::fstream& fs) override
        {
            fs << distanceInKm << " " << timeInMins << " " << difficulty;
        }
    };


    /* Stretch exercise
    *  diffculty : light <> deep stretch
    */
    struct StretchEx : public ExBase
    {
    public:
        int difficulty = 0;

        void SendToFile(std::fstream& fs) override
        {
            fs << difficulty;
        }
    };

    /* WorkoutComponent
    *  implementation based on Component structure
    *  As with Composite pattern - Components are interchangeable
    */
    class WorkoutComponent : public Component {
        std::list<std::shared_ptr<Component>> m_children;


    public:
        WorkoutComponent() = default;

        WorkoutComponent(WorkoutComponent& wc):
            Component()
        {
            m_name = wc.GetName();
            m_componentType = wc.GetCompType();
            m_children = wc.GetChildrenCopy();
        }

        WorkoutComponent ResetComponent(WorkoutComponent& wc)
        {
            m_name = wc.GetName();
            m_componentType = wc.GetCompType();
            m_children = wc.GetChildrenCopy();
            return *this;
        }



        void Add(Component* component) override {
            this->m_children.push_back(std::shared_ptr<Component>(std::move(component)));
        }

        void AddExerciseToSection(std::string sec, Component* com)
        {
            for (auto& l : m_children)
            {
                std::string secName = l->GetName();

                if (secName == sec)
                {
                    l->Add(com);
                }
            }
        }


        void Remove(Component* component) override {
            this->m_children.remove(std::shared_ptr<Component>(std::move(component)));
        }

        bool IsComposite() const override {
            return true;
        }

        void SendToFile(std::fstream& fs) override
        {
            fs << m_name;

            if (m_componentType == workoutName)
            {
                fs << CoreData::exSep << CoreData::sepIn << CoreData::sectionTags << CoreData::exSep;
                for (auto& c : m_children)
                {
                    fs << CoreData::exIn;
                    c->SendToFile(fs);
                    fs << CoreData::exOut;
                }
            }
            else if (m_componentType == sectionTags)
            {
                fs << CoreData::exSep;
                for (auto& c : m_children)
                {
                    fs << CoreData::sepIn;
                    c->SendToFile(fs);
                    fs << CoreData::sepOut;
                }
            }
            else
            {
                for (auto& c : m_children)
                {
                    c->SendToFile(fs);
                }
            }
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

        virtual std::list<std::shared_ptr<Component>> GetChildrenCopy() { return m_children; }
    };

}

#endif /* !CoreData_H */