#pragma once

#include <string>
#include <cstddef>
#include <exception>

template <typename T>
class Container
{
private:
	struct Node
	{
		T data;
		Node * prev, * next;
	};

	std::size_t m_size;
	Node* m_front;
	Node* m_back;

public:
	Container() : m_size(0), m_front(nullptr), m_back(nullptr) {};
	Container(const Container & other);
	~Container() { clear(); }

	void push_front(const T & data);
	void push_back(const T & data);

	void pop_front();
	void pop_back();

	T & front();
	T & back();
	const T & front() const;
	const T & back() const;

	std::size_t size() const { return m_size; }
	bool empty() const { return (m_size == 0); }

	void swap(Container & other);
	void reverse();
	void clear() { while (m_size > 0) pop_back(); }

	Container & operator= (const Container & other);

	class Iterator
	{
	public:
		Iterator(Container::Node * node) : m_ptr(node) {}
		~Iterator() { m_ptr = nullptr; }

		Iterator & operator++ () 
		{
			if (m_ptr == nullptr) throw Exception("trying to increment an empty iterator");
			if (m_ptr->next == nullptr) throw Exception("the increment leads to out of bounds");

			m_ptr = m_ptr->next;
			return * this;
		}

		Iterator operator++ (int) 
		{
			Iterator temp(m_ptr);
			++(* this);
			return temp;
		}

		Iterator & operator-- () 
		{
			if (m_ptr == nullptr) throw Exception("trying to decrement an empty iterator");
			if (m_ptr->prev == nullptr) throw Exception("the decrement leads to out of bounds");

			m_ptr = m_ptr->prev;
			return * this;
		}

		Iterator operator-- (int) 
		{
			Iterator temp(m_ptr);
			--(* this);
			return temp;
		}

		T & operator* () { return m_ptr->data; }

		bool operator== (const Iterator & other) const { return (this->m_ptr == other.m_ptr); }
		bool operator!= (const Iterator & other) const { return (this->m_ptr != other.m_ptr); }

	private:
		Node * m_ptr;
	};

	class reverse_Iterator
	{
	public:
		reverse_Iterator(Container::Node * node) : m_ptr(node) {}
		~reverse_Iterator() { m_ptr = nullptr; }

		reverse_Iterator& operator++ ()
		{
			if (m_ptr == nullptr) throw Exception("trying to increment an empty iterator");
			if (m_ptr->prev == nullptr) throw Exception("the increment leads to out of bounds");

			m_ptr = m_ptr->prev;
			return * this;
		}

		reverse_Iterator operator++ (int)
		{
			reverse_Iterator temp(m_ptr);
			++(* this);
			return temp;
		}

		reverse_Iterator& operator-- ()
		{
			if (m_ptr == nullptr) throw Exception("trying to decrement an empty iterator");
			if (m_ptr->next == nullptr) throw Exception("the decrement leads to out of bounds");

			m_ptr = m_ptr->next;
			return * this;
		}

		reverse_Iterator operator-- (int)
		{
			reverse_Iterator temp(m_ptr);
			--(* this);
			return temp;
		}

		T& operator* () { return m_ptr->data; }

		bool operator== (const reverse_Iterator& other) const { return (this->m_ptr == other.m_ptr); }
		bool operator!= (const reverse_Iterator& other) const { return (this->m_ptr != other.m_ptr); }

	private:
		Node * m_ptr;
	};

	Iterator begin() { return Iterator(m_front); }
	Iterator end() { return Iterator(m_back); }
	Iterator begin() const { return Iterator(m_front); }
	Iterator end() const { return Iterator(m_back); }

	reverse_Iterator rbegin() { return reverse_Iterator(m_back); }
	reverse_Iterator rend() { return reverse_Iterator(m_front); }
	reverse_Iterator rbegin() const { return reverse_Iterator(m_back); }
	reverse_Iterator rend() const { return reverse_Iterator(m_front); }

	class Exception : public std::exception
	{
	public:
		Exception (std::string error) : m_error(error) {}

		const char * what() const noexcept { return m_error.c_str(); }

	private:
		std::string m_error;
	};
};

template <typename T>
Container<T>::Container(const Container & other)
{
	Node * temp = other.m_front;

	while (temp != other.m_back)
	{
		push_back(temp->data);
		temp = temp->next;
	}
	push_back(temp->data);

	temp = nullptr;
}

template <typename T>
void Container<T>::push_front(const T & data)
{
	if (empty())
	{
		m_front = m_back = new Node;

		m_front->prev = nullptr;
		m_front->next = nullptr;
		m_front->data = data;
	}
	else
	{
		Node * temp = new Node;

		temp->data = data;
		temp->next = m_front;
		temp->prev = nullptr;

		m_front->prev = temp;
		m_front = temp;
	}
	++m_size;
}

template <typename T>
void Container<T>::push_back(const T & data)
{
	if (empty())
	{
		m_front = m_back = new Node;

		m_back->prev = nullptr;
		m_back->next = nullptr;
		m_back->data = data;
	}
	else
	{
		Node * temp = new Node;

		temp->data = data;
		temp->next = nullptr;
		temp->prev = m_back;
		
		m_back->next = temp;
		m_back = temp;
	}
	++m_size;
}

template <typename T>
T & Container<T>::front()
{
	if (empty())
		throw Exception("trying to take an element from an empty container");
	return m_front->data;
}

template <typename T>
T & Container<T>::back()
{
	if (empty())
		throw Exception("trying to take an element from an empty container");
	return m_back->data;
}

template <typename T>
const T & Container<T>::front() const
{
	if (empty())
		throw Exception("trying to take an element from an empty container");
	return m_front->data;
}

template <typename T>
const T & Container<T>::back() const
{
	if (empty())
		throw Exception("trying to take an element from an empty container");
	return m_back->data;
}

template <typename T>
void Container<T>::pop_front()
{
	if (empty()) 
		return;

	if (m_size == 1)
	{
		delete m_front;
		m_front = m_back = nullptr;
	}
	else
	{
		m_front = m_front->next;
		delete m_front->prev;
		m_front->prev = nullptr;
	}
	--m_size;
}

template <typename T>
void Container<T>::pop_back()
{
	if (empty())
		return;

	if (m_size == 1)
	{
		delete m_back;
		m_front = m_back = nullptr;
	}
	else
	{
		m_back = m_back->prev;
		delete m_back->next;
		m_back->next = nullptr;
	}
	--m_size;
}

template <typename T>
void Container<T>::swap(Container<T> & other)
{
	std::size_t temp_size = this->m_size;
	Node* temp_front = this->m_front;
	Node* temp_back = this->m_back;

	this->m_size = other.m_size;
	this->m_front = other.m_front;
	this->m_back = other.m_back;

	other.m_size = temp_size;
	other.m_front = temp_front;
	other.m_back = temp_back;
}

template <typename T>
void Container<T>::reverse()
{
	Node * ptr = m_front;

	while (ptr != nullptr)
	{
		Node* temp = ptr->next;
		ptr->next = ptr->prev;
		ptr->prev = temp;
		ptr = temp;
	}

	ptr = m_front;

	m_front = m_back;
	m_front->prev = nullptr;
	m_front->next = m_back->next;
	
	m_back = ptr;
	m_back->prev = ptr->prev;
	m_back->next = nullptr;
}

template <typename T>
Container<T> & Container<T>::operator= (const Container & other)
{
	clear();

	Node * temp = other.m_front;

	while (temp != other.m_back)
	{
		push_back(temp->data);
		temp = temp->next;
	}
	push_back(temp->data);

	temp = nullptr;

	return * this;
}

