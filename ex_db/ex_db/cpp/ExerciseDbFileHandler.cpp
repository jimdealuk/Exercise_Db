// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#include <iostream>
#include <fstream>

#include <string>
#include <locale>  // std::locale, std::tolower
#include <cassert> // for assert()

#include <filesystem>

#include "..\hpp\ExerciseDbFileHandler.h"

namespace ExerciseDbHandling
{
    /* Constructor to create the class
    *  also creates the unique_ptr's that contain the data
    */
    ExerciseDbFileHandler::ExerciseDbFileHandler()
        :ExerciseDbHandlerBase()
    {
        m_exercises = std::make_unique< std::vector<CoreData::BaseEx> >();
        m_tags = std::make_unique< std::vector<std::string> >();
        m_workouts = std::make_unique< std::vector<CoreData::Workout> >();

        m_workoutComponents = std::make_unique<CoreData::WorkoutComponent>();
        m_workoutComponents->SetName("class workouts");
    }

    /* ReadExDb
    *  This method uses the data from the core data to read (possibly create)
    *  the file that the all the data for the app is stored in:
    *  exercises
    *  tags
    *  sections
    */
    bool ExerciseDbFileHandler::ReadExDb()
    {
        bool ret = { false };
        // if directory containing the exercise data file is not there - create it
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
            bool workoutProc = { false };

            std::string line = { "" };
            std::string ex = { "" };
            std::string tag = { "" };
            std::string allTags = { "" };

            std::vector<CoreData::BaseEx> tempExcerises;
            std::vector<std::string> tempTags;
            std::vector<CoreData::Workout> tempWorkouts;

            CoreData::WorkoutComponent* topWorkout = new CoreData::WorkoutComponent();
            topWorkout->SetName(CoreData::topName);

            if (fs.is_open())
            {
                // if more data left in the file...
                while (std::getline(fs, line))
                {
                    // prep which container the data is being read in to
                    if (line == CoreData::tagBase)
                    {
                        exProc = true;
                        tagsProc = false;
                        workoutProc = false;
                    }
                    else if (line == CoreData::tagTags)
                    {
                        exProc = false;
                        tagsProc = true;
                        workoutProc = false;
                    }
                    else if (line == CoreData::workoutTags)
                    {
                        exProc = false;
                        tagsProc = false;
                        workoutProc = true;
                    }
                    else
                    {
                        //do nothing
                    }
                    // process exercise data : exercise + it's associated tags
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
                    // process tags data
                    if ((tagsProc) && (line.find(CoreData::dataIn) == std::string::npos) && (line.find(CoreData::dataOut) == std::string::npos))
                    {
                        if (line != CoreData::tagTags)
                        {
                            size_t cp = line.find(CoreData::tagSep);
                            std::string tag = line.substr(0, cp);

                            tempTags.push_back(tag);
                        }
                    }
                    // process workout data
                    if (workoutProc)
                    {
                        // we've found "Workout" tag - Extract the sections & exercises
                        if (line.find(CoreData::exSep) != std::string::npos)
                        {
                            size_t cpI = line.find(CoreData::sepIn);
                            size_t cpO = line.find(CoreData::exSep);
                            std::string name = line.substr(cpI+1, cpO-1); // get 

                            // create name workout
                            CoreData::Workout workout = { name };

                            CoreData::WorkoutComponent* wc = new CoreData::WorkoutComponent();
                            wc->SetCompType(CoreData::workoutName);
                            wc->SetName(name);

                            line = line.erase(0, cpO+2); // remove "<name>:(" from string
                            cpI = line.find(CoreData::exSep);

                            // find Sections - section
                            std::string sectionTag = line.substr(0, cpI); 
                            if (sectionTag == CoreData::sectionTags)
                            {
                                line = line.erase(0, cpI + 1); // remove "Section:(" from string

                                size_t exI = line.find(CoreData::exIn);
                                size_t exO = line.find(CoreData::exOut);

                                // TODO - take out test code 
                                std::string tempLine = line;

                                std::vector<CoreData::WorkoutSection> sections;
                                ReadSectionFromWorkout(line, sections);

                                // TODO - take out test code 
                                ReadExerciseComponentsFromWorkout(tempLine, *wc);

                                workout.sections = sections;                                
                            }
                            tempWorkouts.push_back(workout);

                            topWorkout->Add(wc);
                        }
                    }
                }
                // if we're here - the exercise data *should* have been read in to the relevant container
                ret = true;
                // add exercises in to container attribute *after* all of them have been retreived
                using std::begin, std::end;
                m_exercises->insert(end(*m_exercises), begin(tempExcerises), end(tempExcerises));
                m_tags->insert(end(*m_tags), begin(tempTags), end(tempTags));
                m_workouts->insert(end(*m_workouts), begin(tempWorkouts), end(tempWorkouts));

                m_workoutComponents->Add(std::move(topWorkout));

                // TODO - take out test code 
                std::cout << "\n\n";
                m_workoutComponents->print();
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

    /* ReadSectionFromWorkout
    *  This method reads the section from the workout data in
    */
    bool ExerciseDbFileHandler::ReadSectionFromWorkout(std::string& sectionStr, std::vector<CoreData::WorkoutSection>& sections)
    {
        bool ret = { false };
        std::string section = { "" };

        try
        {
            size_t exO = sectionStr.find(CoreData::exOut);

            do {
                // remove the current section
                size_t endSec = sectionStr.find(CoreData::exOut);

                // get section
                section = sectionStr.substr(0, endSec + 1);

                // get section name
                size_t secNameEnd = section.find(CoreData::exSep);
                std::string secName = section.substr(1, secNameEnd - 1);

                CoreData::WorkoutSection sectionStore;
                sectionStore.name = secName;

                size_t en = section.size() - secNameEnd;
                std::string exs = section.substr(secNameEnd + 1, en - 2);

                std::string exsCP = exs;

                std::vector<CoreData::ExDescription> exercises;
                if (!ReadExercisesFromSection(exs, exercises))
                {
                    throw "Bad Exercises Read";
                }

                // insert the exercises in this section
                sectionStore.excercises.insert(end(sectionStore.excercises), begin(exercises), end(exercises));

                // remove the current section
                endSec = sectionStr.find(CoreData::exOut);
                sectionStr = sectionStr.erase(0, (endSec + 1));

                exO = sectionStr.find(CoreData::exOut);

                sections.push_back(sectionStore);
            } while (exO != std::string::npos);

        }
        catch (...)
        {
            //false already set :: TODO - this may falsely look like a reading sections problem
        }

        return ret;
    }

    /* ReadExercisesFromSection
    *  This method reads the exercises from the section data in
    */
    bool ExerciseDbFileHandler::ReadExercisesFromSection(std::string& sectionStr, std::vector<CoreData::ExDescription>& exercises)
    {
        bool ret = { false };
        try
        {
            do {
                size_t exEnd = sectionStr.find(CoreData::sepOut);
                std::string ex = sectionStr.substr(1, exEnd - 1);
                sectionStr = sectionStr.erase(0, (exEnd + 1));

                CoreData::ExDescription exd = { ex };
                exercises.push_back(exd);
            } while (sectionStr.size() > 0);
            ret = true;
        }
        catch (...)
        {
            // return already set to false
        }

        return ret;
    }


    /* ReadExerciseComponentsFromSection
    *  Reads exercise components from section string
    */
    bool ExerciseDbFileHandler::ReadExerciseComponentsFromSection(std::string& sectionStr, CoreData::WorkoutComponent& sections)
    {
        bool ret = { false };
        try
        {
            do {
                size_t exEnd = sectionStr.find(CoreData::sepOut);
                std::string ex = sectionStr.substr(1, exEnd - 1);
                sectionStr = sectionStr.erase(0, (exEnd + 1));

                CoreData::ExBase* exd = new CoreData::ExBase();
                exd->SetCompType(CoreData::tagBase);
                exd->SetName(ex);

                sections.Add(exd);

            } while (sectionStr.size() > 0);
            ret = true;
        }
        catch (...)
        {
            // return already set to false
        }
        return ret;
    }


    /* ReadExerciseComponentsFromWorkout
    *  Reads exercise components from workout string
    */
    bool ExerciseDbFileHandler::ReadExerciseComponentsFromWorkout(std::string& sectionStr, CoreData::WorkoutComponent& sections)
    {
        bool ret = { false };

        std::string section = { "" };

        try
        {
            size_t exO = sectionStr.find(CoreData::exOut);

            do {
                // remove the current section
                size_t endSec = sectionStr.find(CoreData::exOut);

                // get section
                section = sectionStr.substr(0, endSec + 1);

                // get section name
                size_t secNameEnd = section.find(CoreData::exSep);
                std::string secName = section.substr(1, secNameEnd - 1);

                size_t en = section.size() - secNameEnd;
                std::string exs = section.substr(secNameEnd + 1, en - 2);

                CoreData::WorkoutComponent* wc = new CoreData::WorkoutComponent();
                wc->SetName(secName);
                wc->SetCompType(CoreData::sectionTags);
                ReadExerciseComponentsFromSection(exs, *wc);

                // TODO - take out test code 
                wc->print();

                // remove the current section
                endSec = sectionStr.find(CoreData::exOut);
                sectionStr = sectionStr.erase(0, (endSec + 1));

                exO = sectionStr.find(CoreData::exOut);

                sections.Add(wc);

            } while (exO != std::string::npos);

        }
        catch (...)
        {
            //false already set :: TODO - this may falsely look like a reading sections problem
        }

        return ret;
    }


    /* WriteExercisesToFile
    *  This method write the exercises to the data file
    */
    bool ExerciseDbFileHandler::WriteExercisesToFile(std::fstream& fs)
    {
        bool ret = { false };
        try
        {
            // set up the "header" for the exercise section
            fs << CoreData::lineSep << CoreData::tagBase << CoreData::lineSep;

            std::string dataStr = { "" };

            for (auto& ex : *m_exercises.get())
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
            ret = true;
        }
        catch (...)
        {
            // ret already set false
        }
        return ret;
    }


    /* WriteExerciseComponentsToFile
    *  Write workout components to file
    */
    bool ExerciseDbFileHandler::WriteExerciseComponentsToFile(std::fstream& fs)
    {
        bool ret = { false };
        try
        {
            // set up the "header" for the sections
            fs << CoreData::dataIn << CoreData::lineSep << CoreData::workoutTags << CoreData::lineSep;


            std::list<std::shared_ptr<CoreData::Component>> wc = m_workoutComponents->GetChildrenCopy();

            if (wc.size() == 1) // there should only be one component at this level
            {
                std::list<std::shared_ptr<CoreData::Component>> wcNext = wc.front()->GetChildrenCopy();

                for (auto& workouts : wcNext)
                {
                    fs << CoreData::sepIn; // (<name>:(
                    workouts->SendToFile(fs);
                    fs << CoreData::sepOut;
                    fs << CoreData::sepOut;
                    fs << CoreData::lineSep;
                }
            }

            fs << CoreData::lineSep;

            fs << CoreData::dataOut << CoreData::lineSep;
            ret = true;
        }
        catch (...)
        {
            // ret already set false
        }
        return ret;
    }


    /* WriteTagsToFile
    *  This method write the tags to the data file
    */
    bool ExerciseDbFileHandler::WriteTagsToFile(std::fstream& fs)
    {
        bool ret = { false };

        try 
        {
            // set up the "header" for the tags
            fs << CoreData::dataIn << CoreData::lineSep << CoreData::tagTags << CoreData::lineSep;

            std::string dataStr = { "" };
            size_t numTagsTags = { m_tags->size() };
            for (auto& tg : *m_tags.get())
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
            fs << CoreData::lineSep;
            ret = true;
        }
        catch (...)
        {
            // ret already set false
        }

        return ret;
    }

    /* WriteWorkoutsToFile
    *  This method write the workouts to the data file
    */
    bool ExerciseDbFileHandler::WriteWorkoutsToFile(std::fstream& fs)
    {
        bool ret = { false };

        try
        {
            // set up the "header" for the sections
            fs << CoreData::dataIn << CoreData::lineSep << CoreData::workoutTags << CoreData::lineSep;

            std::string dataStr = { "" };

            size_t numWorkoutsTags = { m_workouts->size() };
            for (auto& wo : *m_workouts.get())
            {
                fs << CoreData::sepIn; // (
                dataStr = wo.name;
                fs << dataStr; // (<name>
                fs << CoreData::exSep; // (<name>:
                fs << CoreData::sepIn; // (<name>:(
                fs << CoreData::sectionTags; // (<name>:(Section
                fs << CoreData::exSep; // (<name>:(Section:

                std::vector<CoreData::WorkoutSection> woSec = wo.sections;
                std::string secName = { "" };
                size_t numSections = { woSec.size() };
                for (auto& ws : woSec)
                {
                    fs << CoreData::exIn;
                    secName = ws.name;
                    fs << secName;
                    fs << CoreData::exSep;

                    std::string ex = { "" };
                    std::vector<CoreData::ExDescription> exs = ws.excercises;
                    for (auto ex : exs)
                    {
                        fs << CoreData::sepIn;
                        fs << ex.exName;
                        fs << CoreData::sepOut;
                    }

                    fs << CoreData::exOut;// (<name>:(Section:[(<ex>]

                }
                fs << CoreData::sepOut; // (<name>:(Section:[  ])
                fs << CoreData::sepOut; // (<name>:(Section:[  ]))
                fs << CoreData::lineSep;
            }
            ret = true;
        }
        catch (...)
        {
            // ret already set false
        }

        return ret;
    }

    /* WriteExDb 
    *  Write the exerce, tag and section data from their relevant containers
    *  into the file
    */
    bool ExerciseDbFileHandler::WriteExDb()
    {
        bool ret = { false };
        // if directory containing the exercise data file is not there - create it
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

        // TODO - take out test code 
        std::fstream fsTest;

        try {
            fs.open(CoreData::file, std::fstream::out);

            fsTest.open(CoreData::tempfilealt, std::fstream::out);

            fs << CoreData::dataIn;
            if (!WriteExercisesToFile(fs))
            {
                throw "Bad Exercises Write";
            }

            if (!WriteTagsToFile(fs))
            {
                throw "Bad Tags Write";
            }

            // TODO - take out test code 
            WriteExerciseComponentsToFile(fsTest);
            fsTest.close();

            if (!WriteWorkoutsToFile(fs))
            {
                throw "Bad Workouts Write";
            }

            fs << CoreData::lineSep;
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

    // Set return reference parameter to the exercises container
    void ExerciseDbFileHandler::GetExDb(std::vector<CoreData::BaseEx>& exDbHandle)
    {
        exDbHandle = *(m_exercises.get());
    }

    // Set return reference parameter to the tags container
    void ExerciseDbFileHandler::GetExTags(std::vector<std::string>& tagsHandle)
    {
        tagsHandle = *(m_tags.get());
    }

    // Set return reference parameter to the sections container
    void ExerciseDbFileHandler::GetWorkout(std::vector<CoreData::Workout>& tagsHandle)
    {
        tagsHandle = *(m_workouts.get());
    }

    // replace the current tags container with the one in the parameter
    void ExerciseDbFileHandler::SetExTags(std::unique_ptr<std::vector<std::string>> tags)
    {
        m_tags = std::move(tags);
    }

    // replace the current exercises container with the one in the parameter
    void ExerciseDbFileHandler::SetExercises(std::unique_ptr<std::vector<CoreData::BaseEx>> tags)
    {
        m_exercises = std::move(tags);
    }

    // replace the current sections container with the one in the parameter
    void ExerciseDbFileHandler::SetWorkout(std::unique_ptr<std::vector<CoreData::Workout>> tags)
    {
        m_workouts = std::move(tags);
    }
}
