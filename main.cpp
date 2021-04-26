#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Tasks.h"


int getRandomIntNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}


double getRandomDoubleNumber(double min, double max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return (rand() * fraction * (max - min + 1) + min);
}


Task * generateArithmeticTask()
{
	switch (getRandomIntNumber(1, 4))
	{
	case 1:
		return new ArithmeticOperation("add", getRandomDoubleNumber(-100.0, 100.0), '+', getRandomDoubleNumber(-100.0, 100.0));
	case 2:
		return new ArithmeticOperation("sub", getRandomDoubleNumber(-100.0, 100.0), '-', getRandomDoubleNumber(-100.0, 100.0));
	case 3:
		return new ArithmeticOperation("mul", getRandomDoubleNumber(-100.0, 100.0), '*', getRandomDoubleNumber(-100.0, 100.0));
	case 4:
		return new ArithmeticOperation("div", getRandomDoubleNumber(-100.0, 100.0), '/', getRandomDoubleNumber(-100.0, 100.0));
	}
}


int main()
{
	srand(static_cast<unsigned int>(time(0)));

	Container<Task *> tasksList;
	for (std::size_t i = 0; i < 15; ++i)
	{
		AddingTaskToList adding(generateArithmeticTask(), tasksList);
		adding.execute();
	}


	CountingExistingObjects objectCounter;
	objectCounter.execute();
	std::cout << objectCounter.toString() << std::endl << std::endl;


	Container<std::string> infoList;
	while (!tasksList.empty())
	{
		Task* task = tasksList.front();
		task->execute();
		infoList.push_back(task->toString());
		delete task;
		tasksList.pop_front();
	}


	Container<std::string>::Iterator iter = infoList.begin();
	for (; iter != infoList.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}
	std::cout << *iter << std::endl;


	infoList.clear();


	objectCounter.execute();
	std::cout << std::endl << objectCounter.toString() << std::endl;

	return 0;
}