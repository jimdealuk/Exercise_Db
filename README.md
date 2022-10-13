# Exercise_Db
Prototype code : showing C++ 11/14/17 and Dependency Injection &amp;  Command Design pattern usage

This is purely demo code that tries to show usage of some C++ 11/14/17 features as well as integrating some design patterns.

The idea behind the code is to create a basic database of exercises. 
Each exercise "type" consists of a data structure containing an exercise name and a container of "tags".  
(Think of "tags" as exercise categories, such as exercises for Powerlifting, Rowing or Keep fit.)

The method of storage is to use the Dependency Injection pattern to allow different storage mechanisms to used.  
The storage used in the example is a file, But it could be a handle to a database - for example.

The implementation of this pattern is based in class : ExerciseDbFileHandler.

This class also allows :  
the initialisation of the containers of exercises and tags  
the accessing of the container of the exercise data structures, and the container of the database tags  


The ExerciseDbClass class contains the data handler and allows the user of the class to access:  
the container of exercise data structures  
the container of tags  

the addition of exercisesand tags

the writing back to the data storage mechanism


The classes contained in the namespace ExercideDbUI are the "ui" controls

The command pattern interface is defined in the Command interface class

All the "commands" in the UI inherit from the Command class and implement the relevant functionality.
This ranges from displaying a list of exercises, to saving the exercises and tags to the relevant storage.

The UI itself is started from the Invoker class. This class creates and then executes the relevant command.

This code does NOT contain:  
a full ui - it's console based  
unit tests - currently no unit tests created  


