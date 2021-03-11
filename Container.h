#pragma once

#include <string>
#include <cstddef>
#include <exception>

template <typename T>
class Container
{
public:
	struct Node
	{
		T data;
		Node * prev, * next;
	};

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

	void set_front(Node * node) { m_front = node; }
	void set_back(Node * node) { m_back = node; }
	void set_size(std::size_t size) { m_size = size; }

	Node * get_front_ptr() const { return m_front; }
	Node * get_back_ptr() const { return m_back; }

	void reverse();

	void clear() { while (m_size > 0) pop_back(); }

	Container & operator= (const Container & other);

	class Iterator
	{
	public:
		Iterator(Node * node) : m_ptr(node) {}
		~Iterator() { m_ptr = nullptr; }

		Iterator & operator++ () 
		{
			if (m_ptr == nullptr) throw Exception("trying to increment an empty iterator");
			if (m_ptr->prev == nullptr) throw Exception("the increment leads to out of bounds");

			m_ptr = m_ptr->prev;
			return *this;
		}

		Iterator operator++ (int) 
		{
			if (m_ptr == nullptr) throw Exception("trying to increment an empty iterator");
			if (m_ptr->prev == nullptr) throw Exception("the increment leads to out of bounds");

			Iterator temp(m_ptr);
			++(*this);
			return temp;
		}

		Iterator & operator-- () 
		{
			if (m_ptr == nullptr) throw Exception("trying to decrement an empty iterator");
			if (m_ptr->next == nullptr) throw Exception("the decrement leads to out of bounds");

			m_ptr = m_ptr->next;
			return *this;
		}

		Iterator operator-- (int) 
		{
			if (m_ptr == nullptr) throw Exception("trying to decrement an empty iterator");
			if (m_ptr->next == nullptr) throw Exception("the decrement leads to out of bounds");

			Iterator temp(m_ptr);
			--(*this);
			return temp;
		}

		T & operator* () { return m_ptr->data; }

		bool operator== (const Iterator & other) const { return (this->m_ptr == other.m_ptr); }
		bool operator!= (const Iterator & other) const { return (this->m_ptr != other.m_ptr); }

	private:
		Node * m_ptr;
	};

	Iterator begin() { return Iterator(m_front); }
	Iterator end() { return Iterator(m_back); }
	Iterator begin() const { return Iterator(m_front); }
	Iterator end() const { return Iterator(m_back); }

	class Exception : public std::exception
	{
	public:
		Exception (std::string error) : m_error(error) {}

		const char * what() const noexcept { return m_error.c_str(); }

	private:
		std::string m_error;
	};

private:
	std::size_t m_size;
	Node * m_front;
	Node * m_back;
};

template <typename T>
Container<T>::Container(const Container & other)
{
	Node * temp = other.m_front;

	while (temp != other.m_back)
	{
		push_back(temp->data);
		temp = temp->prev;
	}
	push_back(temp->data);

	temp = nullptr;
}

template <typename T>
void Container<T>::push_front(const T & data)
{
	if (m_size == 0)
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
		temp->prev = m_front;
		temp->next = nullptr;

		m_front->next = temp;
		m_front = temp;
	}
	++m_size;
}

template <typename T>
void Container<T>::push_back(const T & data)
{
	if (m_size == 0)
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
		temp->prev = nullptr;
		temp->next = m_back;
		
		m_back->prev = temp;
		m_back = temp;
	}
	++m_size;
}

template <typename T>
T & Container<T>::front()
{
	if (m_size != 0) return m_front->data;
	else throw Exception("trying to take an element from an empty container");
}

template <typename T>
T & Container<T>::back()
{
	if (m_size != 0) return m_back->data;
	else throw Exception("trying to take an element from an empty container");
}

template <typename T>
const T & Container<T>::front() const
{
	if (m_size != 0) return m_front->data;
	else throw Exception("trying to take an element from an empty container");
}

template <typename T>
const T & Container<T>::back() const
{
	if (m_size != 0) return m_back->data;
	else throw Exception("trying to take an element from an empty container");
}

template <typename T>
void Container<T>::pop_front()
{
	if (m_size > 0)
	{
		if (m_size == 1)
		{
			delete m_front;
			m_front = m_back = nullptr;
		}
		else
		{
			m_front = m_front->prev;
			delete m_front->next;
			m_front->next = nullptr;
		}
		--m_size;
	}
}

template <typename T>
void Container<T>::pop_back()
{
	if (m_size > 0)
	{
		if (m_size == 1)
		{
			delete m_back;
			m_front = m_back = nullptr;
		}
		else
		{
			m_back = m_back->next;
			delete m_back->prev;
			m_back->prev = nullptr;
		}
		--m_size;
	}
}

template <typename T>
void swap(Container<T> & a, Container<T> & b)
{
	auto temp_front = a.get_front_ptr();
	auto temp_back = a.get_back_ptr();
	std::size_t temp_size = a.size();

	a.set_front(b.get_front_ptr());
	a.set_back(b.get_back_ptr());
	a.set_size(b.size());

	b.set_front(temp_front);
	b.set_back(temp_back);
	b.set_size(temp_size);
}

template <typename T>
void Container<T>::reverse()
{
	Node * ptr = m_front;

	while (ptr != nullptr)
	{
		Node* temp = ptr->prev;
		ptr->prev = ptr->next;
		ptr->next = temp;
		ptr = temp;
	}

	ptr = m_front;

	m_front = m_back;
	m_front->next = nullptr;
	m_front->prev = m_back->prev;
	
	m_back = ptr;
	m_back->next = ptr->next;
	m_back->prev = nullptr;
}

template <typename T>
Container<T> & Container<T>::operator= (const Container & other)
{
	clear();

	Node * temp = other.m_front;

	while (temp != other.m_back)
	{
		push_back(temp->data);
		temp = temp->prev;
	}
	push_back(temp->data);

	temp = nullptr;

	return * this;
}

