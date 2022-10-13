#include <iostream>
#include <fstream>

#include <string>
#include <locale>  // std::locale, std::tolower
#include <cassert> // for assert()

#include <filesystem>

#include "..\hpp\ExerciseDbFileHandler.h"

namespace ExerciseDbHandling
{

    ExerciseDbFileHandler::ExerciseDbFileHandler()
        :ExerciseDbHandlerBase()
    {
        m_exercises = std::make_unique< std::vector<CoreData::BaseEx> >();
        m_tags = std::make_unique< std::vector<std::string> >();
        m_workoutSectiontags = std::make_unique< std::vector<std::string> >();
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
            bool secProc = { false };

            std::string line = { "" };
            std::string ex = { "" };
            std::string tag = { "" };
            std::string allTags = { "" };

            std::vector<CoreData::BaseEx> tempExcerises;
            std::vector<std::string> tempTags;
            std::vector<std::string> tempSections;

            if (fs.is_open())
            {
                while (std::getline(fs, line))
                {
                    if (line == CoreData::tagBase)
                    {
                        exProc = true;
                        tagsProc = false;
                        secProc = false;
                    }
                    else if (line == CoreData::tagTags)
                    {
                        exProc = false;
                        tagsProc = true;
                        secProc = false;
                    }
                    else if (line == CoreData::sectionTags)
                    {
                        exProc = false;
                        tagsProc = false;
                        secProc = true;
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

                        CoreData::BaseEx exToAdd = { ex, tagVec };

                        tempExcerises.push_back(exToAdd);
                    }
                    if ((tagsProc) && (line.find(CoreData::dataIn) == std::string::npos) && (line.find(CoreData::dataOut) == std::string::npos))
                    {
                        if (line != CoreData::tagTags)
                        {
                            size_t cp = line.find(CoreData::tagSep);
                            std::string tag = line.substr(0, cp);

                            tempTags.push_back(tag);
                        }
                    }
                    if ((secProc) && (line.find(CoreData::dataIn) == std::string::npos) && (line.find(CoreData::dataOut) == std::string::npos))
                    {
                        if (line != CoreData::sectionTags)
                        {
                            size_t cp = line.find(CoreData::tagSep);
                            std::string tag = line.substr(0, cp);

                            tempSections.push_back(tag);
                        }
                    }

                }
                ret = true;
            }
            // add exercises in to container attribute *after* all of them have been retreived
            using std::begin, std::end;
            m_exercises->insert(end(*m_exercises), begin(tempExcerises), end(tempExcerises));
            m_tags->insert(end(*m_tags), begin(tempTags), end(tempTags));
            m_workoutSectiontags->insert(end(*m_workoutSectiontags), begin(tempSections), end(tempSections));

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

            // exercise tags
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

            // workout sections
            fs << CoreData::dataIn << CoreData::lineSep << CoreData::sectionTags << CoreData::lineSep;

            dataStr = { "" };
            size_t numSecTags = { m_workoutSectiontags->size() };
            for (auto tg : *m_workoutSectiontags.get())
            {
                fs << tg;
                if (numSecTags > 1)
                {
                    fs << CoreData::tagSep;
                }
                numSecTags--;
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

    void ExerciseDbFileHandler::GetExDb(std::vector<CoreData::BaseEx>& exDbHandle)
    {
        exDbHandle = *(m_exercises.get());
    }

    void ExerciseDbFileHandler::GetExTags(std::vector<std::string>& tagsHandle)
    {
        tagsHandle = *(m_tags.get());
    }

    void ExerciseDbFileHandler::GetWorkoutSection(std::vector<std::string>& tagsHandle)
    {
        tagsHandle = *(m_workoutSectiontags.get());
    }

    void ExerciseDbFileHandler::SetExTags(std::unique_ptr<std::vector<std::string>> tags)
    {
        m_tags = std::move(tags);
    }

    void ExerciseDbFileHandler::SetExercises(std::unique_ptr<std::vector<CoreData::BaseEx>> tags)
    {
        m_exercises = std::move(tags);
    }

    void ExerciseDbFileHandler::SetWorkoutSection(std::unique_ptr<std::vector<std::string>> tags)
    {
        m_workoutSectiontags = std::move(tags);
    }
}
