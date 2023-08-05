#ifndef ExerciseDbFactoryBase_H
#define ExerciseDbFactoryBase_H

#include "ExerciseDbHandlerBase.h"

namespace ExerciseDbFactoryBase
{
	/* Interface Class : ExerciseDbFactoryBase
	* Interface for a class to create the exercise database data source(s).
	* A exercise database data source could be a file, database handle OR REST API.
	* 
	* A ExerciseDbFactory would create and initialise on of those databases.
	*/
	class ExerciseDbFactoryBase
	{
	public:
		virtual ~ExerciseDbFactoryBase() {};
		virtual std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> FactoryMethod() = 0;
	};

}
#endif /* !ExerciseDbFactoryBase_H */
