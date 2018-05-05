/**
*	@author Joonatan Kuosa <joonatan.kuosa@gmail.com>
*	@date 2018-05
*	@file fifo.hpp
*
*	Under a copyleft.
*/

#ifndef FIFO_HPP
#define FIFO_HPP

#include <atomic>
#include <cassert>

/**	@class fifo
 *	@desc Non locking thread safe queue (first in, first out buffer)
 *	Thread safe as long as one reader and one writer and the responsibilities never change.
 *	If you need both input and output between two threads you need two fifos.
 *
 *	Non locking is implemented by having one thread read from one end and other one push to the other
 *	we use a pointer between these two to mark the read point.
 *	The read pointer is atomic since it might be accessed from both threads.
*/
template<typename T>
class fifo
{
private:
    /// Linked list structure
	struct node
	{
		node(T d)
			: data(d)
			, next(nullptr)
		{}

		node()
			: data()
			, next(nullptr)
		{}

		T data;
		node *next;
	};

public:
    /// Constructor
	fifo()
	{
		// on purpose a single statement, they all point to the same object
		front = back = divider = new node;
	}

    /// Destructor
	~fifo()
	{
		while(front.load() != nullptr)
		{
			node *tmp = front.load();
			front.store(tmp->next);
			delete tmp;
		}
	}

    /// @brief push data to back
    /// @param data element to push
    /// @throws never
	void push(T data)
	{
		assert(front.load() != nullptr);
		assert(divider.load() != nullptr);
		assert(back != nullptr);

		back->next = new node(data);
		back = back->next;
		
		// lazy delete, we don't modify divider here
		// and pop doesn't modify front so we are all good
		while (front.load() != divider.load())
		{
			node *tmp = front.load();
			front.store(tmp->next);
			delete tmp;
		}
	}

    /// @brief pop data from front
    /// @return the popped element
    /// @throws on empty buffer
	T pop()
	{
		if(empty())
		{
			throw std::string("empty");
		}
		
		// We can't delete here, so we just move the divider
		node *tmp = divider.load()->next;
		assert(tmp != nullptr);
		divider.store(tmp);
		return tmp->data;
	}

    /// @brief is this buffer empty
    /// @return true if empty, false otherwise
	bool empty() const
	{
		assert(divider.load() != nullptr);
		return divider.load()->next == nullptr;
	}

private:
	std::atomic< node *> divider;
    // @todo front doesn't need to be atomic
	std::atomic< node *> front;
	node * back;
};

#endif  // FIFO_HPP