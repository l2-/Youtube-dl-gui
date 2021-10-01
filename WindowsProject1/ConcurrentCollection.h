#pragma once

#include <list>
#include <mutex>

template <typename T> class ConcurrentCollection
{
	std::list<T> collection;
	std::mutex mutex;
public:
	template <class Predicate>
	void remove_if(Predicate pred) { std::lock_guard<std::mutex> lock(mutex); collection.remove_if(pred); }
	void push_back(T item) { std::lock_guard<std::mutex> lock( mutex ); collection.push_back(item); }
	void clear() { std::lock_guard<std::mutex> lock( mutex ); collection.clear(); }
};
