// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <string>
#include <locale>  // std::locale, std::tolower
#include <cassert> // for assert()

#include <vector>

#include <filesystem>

namespace CoreData
{
    const std::string filePath = { "../exercisedb/" };
    const std::string file = { "../exercisedb/exercises.db" };

    const std::string dataIn = { "{" };
    const std::string dataOut = { "}" };
    const std::string lineSep = { "\n" };

    const std::string sepIn = { "(" };
    const std::string sepOut = { ")" };
    const std::string exSep = { ":" };
    const std::string tagSep = { "," };

    const std::string tagBase = { "Exercises" };
    const std::string tagTags = { "Tags" };

}

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

/* Manipulate the exercise data source 
*  In the implementation given - to & from a local file
*  It's intended that the ExerciseDbHandlerBase class
*  could be extended to use an SQL database (for example)
*/
namespace ExerciseDbHandling
{

    /* Base Class : ExerciseDbHandling
    *  Interface class to be inherited from for handling
    *  the exercise data source.
    *  This structure is based on the Dependency Injection
    *  design pattern.
    *  The idea here is that different storage mechanisms
    *  could be used to store the exercise data.
    */
    class ExerciseDbHandlerBase
    {
    public:
        virtual bool ReadExDb() = 0;
        virtual bool WriteExDb() = 0;
        virtual void GetExDb(std::vector<BaseEx>& exDbHandle) = 0;
        virtual void GetExTags(std::vector<std::string>& tagsHandle) = 0;

        virtual void SetExTags(std::unique_ptr<std::vector<std::string>> tags) = 0;

        virtual ~ExerciseDbHandlerBase() = default;
    };


    /* Class : ExerciseDbFileHandler
    *  Implementation of ExerciseDbHandlerBase
    *  This class handles accessing & storing data from a file.
    */
    class ExerciseDbFileHandler : public ExerciseDbHandlerBase
    {
        std::unique_ptr<std::vector<BaseEx>> m_exercises;
        std::unique_ptr<std::vector<std::string>> m_tags;

    public:
        ExerciseDbFileHandler();
        ExerciseDbFileHandler(ExerciseDbFileHandler& exDb) = delete;
        virtual bool ReadExDb();
        virtual bool WriteExDb();

        virtual void GetExDb(std::vector<BaseEx>& exDbHandle);
        virtual void GetExTags(std::vector<std::string>& tagsHandle);

        virtual void SetExTags(std::unique_ptr<std::vector<std::string>> tags);

        virtual ~ExerciseDbFileHandler() = default;

    };


    ExerciseDbFileHandler::ExerciseDbFileHandler()
        :ExerciseDbHandlerBase()
    {
        m_exercises = std::make_unique< std::vector<BaseEx> >();
        m_tags = std::make_unique< std::vector<std::string> >();
    }


    bool ExerciseDbFileHandler::ReadExDb()
    {
        bool ret = { false };
        if (!std::filesystem::is_directory(CoreData::filePath))
        {
            try
            {
                std::filesystem::create_directories(CoreData::filePath);
            }
            catch (...)
            {
                // for some reason directory doesn't exist OR can't be created
                // stop now.
                return false;
            }
        }

        std::fstream fs;
        try {
            fs.open(CoreData::file);

            bool tagsProc = { false };
            bool exProc = { false };

            std::string line = { "" };
            std::string ex = { "" };
            std::string tag = { "" };
            std::string allTags = { "" };

            if (fs.is_open())
            {
                while (std::getline(fs, line))
                {
                    if (line == CoreData::tagBase)
                    {
                        exProc = true;
                        tagsProc = false;
                    }
                    else if (line == CoreData::tagTags)
                    {
                        exProc = false;
                        tagsProc = true;
                    }
                    else
                    {
                        //do nothing
                    }
                    if ((exProc) && (line.find(CoreData::sepIn) != std::string::npos) && (line.find(CoreData::sepOut) != std::string::npos))
                    {
                        size_t p = line.find(CoreData::exSep);
                        ex = line.substr(1, p - 1);

                        allTags = line.substr(p + 1, (line.size() - (p + 2)));
                        std::vector<std::string> tagVec;
                        do {
                            size_t cp = allTags.find(CoreData::tagSep);
                            tag = allTags.substr(0, cp);
                            tagVec.push_back(tag);
                            allTags.erase(0, cp);
                            if (allTags.find(CoreData::tagSep) == 0)
                            {
                                allTags.erase(0, 1);
                            }
                        } while (allTags.size() > 0);

                        BaseEx exToAdd = { ex, tagVec };

                        m_exercises->push_back(exToAdd);
                    }
                    if ((tagsProc) && (line.find(CoreData::dataIn) == std::string::npos) && (line.find(CoreData::dataOut) == std::string::npos))
                    {
                        if (line != CoreData::tagTags)
                        {
                            size_t cp = line.find(CoreData::tagSep);
                            std::string tag = line.substr(0, cp);

                            m_tags->push_back(tag);
                        }
                    }
                }
                ret = true;
            }
        }
        catch (...)
        {
            // something's gone wrong with getting the exercise data
            ret = false;
        }
        fs.close();
        return ret;
    }

    bool ExerciseDbFileHandler::WriteExDb()
    {
        bool ret = { false };
        if (!std::filesystem::is_directory(CoreData::filePath))
        {
            try
            {
                std::filesystem::create_directories(CoreData::filePath);
            }
            catch (...)
            {
                // for some reason directory doesn't exist OR can't be created
                // stop now.
                return false;
            }
        }

        std::fstream fs;
        try {
            fs.open(CoreData::file, std::fstream::out);
            fs << CoreData::dataIn << CoreData::lineSep << CoreData::tagBase << CoreData::lineSep;

            std::string dataStr = { "" };

            for (auto ex : *m_exercises.get())
            {
                fs << CoreData::sepIn;
                dataStr = ex.exName;
                fs << dataStr;
                fs << CoreData::exSep;
                size_t numTags = { ex.exTags.size() };
                for (std::string tag : ex.exTags)
                {
                    fs << tag;
                    if (numTags > 1)
                    {
                        fs << CoreData::tagSep;
                    }
                    numTags--;
                }
                fs << CoreData::sepOut << CoreData::lineSep;
            }
            fs << CoreData::dataOut << CoreData::lineSep;

            fs << CoreData::dataIn << CoreData::lineSep << CoreData::tagTags << CoreData::lineSep;

            dataStr = { "" };
            size_t numTagsTags = { m_tags->size() };
            for (auto tg : *m_tags.get())
            {
                fs << tg;
                if (numTagsTags > 1)
                {
                    fs << CoreData::tagSep;
                }
                numTagsTags--;
                fs << CoreData::lineSep;
            }
            fs << CoreData::dataOut;
            ret = true;
        }
        catch (...)
        {
            // something's gone wrong with writing the exercise data
            ret = false;
        }
        fs.close();

        return ret;

    }
    void ExerciseDbFileHandler::GetExDb(std::vector<BaseEx>& exDbHandle)
    {
        exDbHandle = *(m_exercises.get());
    }
    void ExerciseDbFileHandler::GetExTags(std::vector<std::string>& tagsHandle)
    {
        tagsHandle = *(m_tags.get());
    }

    void ExerciseDbFileHandler::SetExTags(std::unique_ptr<std::vector<std::string>> tags)
    {
        m_tags = std::move(tags);
    }

}


/* Wrapper round the exercise data class :
*  Processes commands
*  - extract exercise data for display 
*  - on exercise data addition it ensures: 
*   no duplicates are stored
*   updates the stored exercise data
*/
namespace ExerciseDbClass
{
    class ExerciseDb
    {
    private:
        std::unique_ptr<std::vector<BaseEx>> m_exercises;
        std::unique_ptr<std::vector<std::string>> m_tags;
        std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> m_exercisesBase;

    public:
        ExerciseDb() = default;
        ExerciseDb(std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle);

        bool LoadExercisesFromDb();
        bool BackupExercsesToDb();
        ~ExerciseDb() = default;

        void GetExTags(std::vector<std::string>& tagsHandle);
        void GetExDb(std::vector<BaseEx>& exDbHandle);

        bool AddTag(std::string tag);

        bool CheckExerciseExists(std::string exStr);
        bool CheckTagExists(std::string exStr);
        bool AddExercise(BaseEx ex);
    };

    ExerciseDb::ExerciseDb(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle)
    {
        m_exercisesBase = std::move(dbHandle);
    }

    bool ExerciseDb::LoadExercisesFromDb()
    {
        m_exercises = std::make_unique< std::vector<BaseEx> >();
        m_tags = std::make_unique< std::vector<std::string> >();

        bool ret =  m_exercisesBase->ReadExDb();

        m_exercisesBase->GetExDb(*(m_exercises.get()));
        m_exercisesBase->GetExTags(*(m_tags.get()));

        return ret;
    }

    bool ExerciseDb::BackupExercsesToDb()
    {
        return m_exercisesBase->WriteExDb();
    }


    bool ExerciseDb::CheckExerciseExists(std::string exStr)
    {
        bool ret = { false };

        auto it = std::find_if(m_exercises->begin(), m_exercises->end(), [exStr](BaseEx& f) { return f.exName == exStr; });
        if (it != m_exercises->end())
        {
            ret = true;
        }
        return ret;
    }

    bool ExerciseDb::CheckTagExists(std::string exStr)
    {
        bool ret = { false };

        auto it = std::find_if(m_tags->begin(), m_tags->end(), [exStr](std::string& f) { return f == exStr; });
        if (it != m_tags->end())
        {
            ret = true;
        }
        return ret;
    }

    bool ExerciseDb::AddExercise(BaseEx ex)
    {
        m_exercises->push_back(ex);

        return true;
    }

    bool ExerciseDb::AddTag(std::string tag)
    {
        bool ret = { false };

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

        return ret;
    }

    void ExerciseDb::GetExTags(std::vector<std::string>& tagsHandle)
    {
        m_exercisesBase->GetExTags(tagsHandle);
    }

    void ExerciseDb::GetExDb(std::vector<BaseEx>& exDbHandle)
    {
        m_exercisesBase->GetExDb(exDbHandle);
    }
}


namespace ExerciseDataApp
{

    class ExerciseData 
    {
    private:
            std::unique_ptr <ExerciseDbClass::ExerciseDb> m_exDb;

    public:
        ExerciseData() = default;
        ExerciseData(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle);
        bool InitialiseExerciseDb();
        bool SaveExerciseDb();

        void GetTagsDb(std::vector<std::string>& tags);
        void GetExDb(std::vector<BaseEx>& dbHandle);

        bool AddTag(std::string tag);


        bool CheckExerciseExists(std::string ex);
        bool CheckTagsExists(std::string ex);
        bool AddExercise(BaseEx ex);


    };

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

    void ExerciseData::GetExDb(std::vector<BaseEx>& dbHandle)
    {
        m_exDb->GetExDb(dbHandle);
    }


    bool ExerciseData::AddTag(std::string tag)
    {
        return m_exDb->AddTag(tag);
    }

    bool ExerciseData::AddExercise(BaseEx ex)
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

namespace ExercideDbUI
{

    class Command {
    public:
        virtual ~Command() {
        }
        virtual void Execute() const = 0;
    };

    class DisplayTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayTagsCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver){
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {
            std::cout << "List of current tags\n";
            std::vector<std::string> tags;
            m_dataSource->GetTagsDb(tags);
            for (auto tg : tags)
            {
                std::cout << tg << std::endl;
            }
        }
    };

    class AddTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddTagsCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {

            std::string tag = { "" };
            bool stopAdding = { false };
            do {
                std::vector<std::string> tags;
                m_dataSource->GetTagsDb(tags);
                for (auto tg : tags)
                {
                    std::cout << tg << std::endl;
                }
                std::cout << "Input new tag - fin tp finish: \n";
                std::getline(std::cin, tag);
                if (tag == "fin")
                {
                    stopAdding = true;
                }

                if (!stopAdding)
                {
                    if (!m_dataSource->AddTag(tag))
                    {
                        std::cout << "tag not added \n";
                    }
                }
                std::cout << "*************************** \n";

            } while (!stopAdding);
        }
    };


    class SaveDbToFileCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        SaveDbToFileCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {
            std::cout << "Saving data to file\n";
            m_dataSource->SaveExerciseDb();
        }
    };

    class AddExerciseCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddExerciseCommand(ExerciseDataApp::ExerciseData* receiver) : m_dataSource(receiver) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {

            std::vector<BaseEx> exs;
            m_dataSource->GetExDb(exs);

            for (auto tg : exs)
            {
                std::cout << tg.exName << std::endl;
            }

            std::string exercise;
            std::cout << "Add exercise name " << std::endl;
            std::getline(std::cin, exercise);

            std::locale loc;
            std::string exTag = { "" };
            for (auto elem : exercise)
            {
                char c = std::tolower(elem, loc);
                if (isalpha(c))
                {
                    exTag.append(1, c);
                }
            }

            if (exTag == "fin")
            {
                return;
            }

            if (m_dataSource->CheckExerciseExists(exTag))
            {
                return;
            }

            // new exercise - havent finished..
            BaseEx exToAdd = { exTag };

            std::string tag = { "" };
            bool stopAdding = { false };
            do {
                std::cout << "Current tags : \n";
                std::vector<std::string> tags;
                m_dataSource->GetTagsDb(tags);
                for (auto tg : tags)
                {
                    std::cout << tg << std::endl;
                }
                std::cout << "Input new tag - fin to finish: \n";
                std::getline(std::cin, tag);
                if (tag == "fin")
                {
                    stopAdding = true;
                }

                if (!stopAdding)
                {
                    if (m_dataSource->CheckTagsExists(tag))
                    {
                        std::cout << "tag not added \n";
                    }
                    else
                    {
                        m_dataSource->AddTag(tag);
                    }
                    
                    auto it = std::find(exToAdd.exTags.begin(), exToAdd.exTags.end(), tag);
                    if (it == exToAdd.exTags.end())
                    {
                        exToAdd.exTags.push_back(tag);
                    }
                }

                std::cout << "*************************** \n";

            } while (!stopAdding);
            m_dataSource->AddExercise(exToAdd);
        }


    };

}


int main()
{
    std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> exh = std::make_unique<ExerciseDbHandling::ExerciseDbFileHandler>();

    std::unique_ptr<ExerciseDataApp::ExerciseData> db = std::make_unique< ExerciseDataApp::ExerciseData>(std::move(exh));
    db->InitialiseExerciseDb();

    //std::unique_ptr<ExercideDbUI::DisplayTagsCommand> dtc = std::make_unique<ExercideDbUI::DisplayTagsCommand>(db.get());
    //dtc->Execute();

    //std::unique_ptr<ExercideDbUI::AddTagsCommand> atc = std::make_unique<ExercideDbUI::AddTagsCommand>(db.get());
    //atc->Execute();

    //std::unique_ptr<ExercideDbUI::SaveDbToFileCommand> stc = std::make_unique<ExercideDbUI::SaveDbToFileCommand>(db.get());
    //stc->Execute();

    std::unique_ptr<ExercideDbUI::AddExerciseCommand> stc = std::make_unique<ExercideDbUI::AddExerciseCommand>(db.get());
    stc->Execute();

}
