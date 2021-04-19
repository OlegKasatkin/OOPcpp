#pragma once

#include <string>
#include <cstddef>
#include "Container.h"


class Object
{
public:
	Object() { ++sm_counter; }
	virtual std::string toString() const = 0;
	virtual ~Object() { --sm_counter; }

protected:
	static std::size_t sm_counter;
};

std::size_t Object::sm_counter = 0;


class Task : virtual public Object
{
public:
	virtual void execute() = 0;

protected:
	bool m_executed = false;
};


class Named : virtual public Object
{
public:
	Named( std::string const & name ) : m_name( name ) {}

protected:
	std::string m_name;
};


class ArithmeticOperation : public Task, public Named
{
public:
	ArithmeticOperation( std::string const & name, double const leftOperand, char const operation, double const rightOperand )
		: Named( name ), m_leftOperand( leftOperand ), m_rightOperand( rightOperand ), m_operation( operation ) {}

	std::string toString() const override
	{
		if( m_executed )
			return( "Task '" + m_name + "': " + std::to_string( m_leftOperand ) + " " + std::string( 1, m_operation ) + 
				" " +  std::to_string( m_rightOperand ) + " = " + std::to_string( m_result ));
		else
		{
			if( !m_correctTask )
				return( "Incorrect operation: " + std::to_string( m_leftOperand ) + " " +
					std::string( 1, m_operation ) + " " + std::to_string( m_rightOperand ));
			else
				return std::string( "Calculation is not executed yet." );
		}
	}

	void execute() override
	{
		if (m_executed) return;
		switch( m_operation )
		{
		case '+':
			m_result = m_leftOperand + m_rightOperand;
			m_executed = true;
			break;

		case '-':
			m_result = m_leftOperand - m_rightOperand;
			m_executed = true;
			break;

		case '*':
			m_result = m_leftOperand * m_rightOperand;
			m_executed = true;
			break;

		case '/':
			if( m_rightOperand == 0 )
			{
				m_correctTask = false;
				break;
			}
			m_result = m_leftOperand / m_rightOperand;
			m_executed = true;
			break;

		default:
			m_correctTask = false;
			break;
		}
	}

private:
	double m_leftOperand, m_rightOperand, m_result;
	char m_operation;
	bool m_correctTask = true;
};


class AddingTaskToList : public Task
{
public:
	AddingTaskToList( Task & task, Container<Object *> & container )
		: m_task( task ), m_container( container ) {}

	std::string toString() const override
	{
		if( m_executed )
			return std::string( "Task was added in container." );
		else
			return std::string( "Task is not added in container yet." );
	}

	void execute() override
	{
		if (m_executed) return;
		m_container.push_back( & m_task );
		m_executed = true;
	}

private:
	Task & m_task;
	Container<Object *> & m_container;
};


class CountingListObjects : public Task
{
public:
	CountingListObjects( Container<Object *> & container)
		: m_container( container ) {}

	std::string toString() const override
	{
		if( m_executed )
			return( "There are " + std::to_string( m_result ) + " objects in container." );
		else
			return std::string( "Objects in container is not counted yet." );
	}

	void execute() override 
	{
		m_result = m_container.size();
		m_executed = true;
	}

private:
	Container<Object *> & m_container;
	std::size_t m_result;
};


class ClearingListObjects : public Task
{
public:
	ClearingListObjects( Container<Object *> & container )
		: m_container( container ) {}

	std::string toString() const override
	{
		if( m_executed )  
			return std::string( "Container was cleared." );
		else
			return std::string( "Clearing is not executed yet." );
	}

	void execute() override 
	{
		m_container.clear();
		m_executed = true;
	}

private:
	Container<Object *> & m_container;
};


class CountingExistingObjects : public Task
{
public:
	std::string toString() const override
	{
		if( m_executed )
			return( "There are " + std::to_string( m_result ) + " objects in program now." );
		else
			return std::string( "Objects in program is not counted yet." );
	}

	void execute() override
	{
		m_result = sm_counter;
		m_executed = true;
	}

private:
	std::size_t m_result;
};