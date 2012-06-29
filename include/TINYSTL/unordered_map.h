/*-
 * Copyright 2012 Matthew Endsley
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TINYSTL_UNORDERED_MAP_H
#define TINYSTL_UNORDERED_MAP_H

#include <TINYSTL/allocator.h>
#include <TINYSTL/buffer.h>
#include <TINYSTL/hash.h>

namespace tinystl {

	template<typename Key, typename Value>
	struct pair
	{
		pair();
		pair(const Key& key, const Value& value);

		Key first;
		Value second;
	};

	template<typename Key, typename Value>
	pair<Key, Value>::pair()
	{
	}

	template<typename Key, typename Value>
	pair<Key, Value>::pair(const Key& key, const Value& value)
		: first(key)
		, second(value)
	{
	}

	template<typename Key, typename Value>
	static inline pair<Key, Value> make_pair(const Key& key, const Value& value)
	{
		return pair<Key, Value>(key, value);
	}


	template<typename Key, typename Value>
	struct unordered_map_node
	{
		unordered_map_node(const Key& key, const Value& value);

		const Key first;
		Value second;
		unordered_map_node* next;
		unordered_map_node* prev;
	};

	template<typename Key, typename Value>
	unordered_map_node<Key, Value>::unordered_map_node(const Key& key, const Value& value)
		: first(key)
		, second(value)
	{
	}

	template<typename Key, typename Value>
	static void unordered_map_node_insert(unordered_map_node<Key, Value>* node, size_t hash, unordered_map_node<Key, Value>** buckets, size_t nbuckets)
	{
		size_t bucket = hash & (nbuckets - 1);

		unordered_map_node<Key, Value>* it = buckets[bucket + 1];
		node->next = it;
		if (it)
		{
			node->prev = it->prev;
			it->prev = node;
			if (node->prev)
				node->prev->next = node;
		}
		else
		{
			size_t newbucket = bucket;
			while (newbucket && !buckets[newbucket])
				--newbucket;

			unordered_map_node<Key, Value>* prev = buckets[newbucket];
			while (prev && prev->next)
				prev = prev->next;

			node->prev = prev;
			if (prev)
				prev->next = node;
		}

		// propagate node through buckets
		for (; it == buckets[bucket]; --bucket)
		{
			buckets[bucket] = node;
			if (!bucket)
				break;
		}
	}

	template<typename Key, typename Value>
	static inline void unordered_map_node_erase(const unordered_map_node<Key, Value>* where, size_t hash, unordered_map_node<Key, Value>** buckets, size_t nbuckets)
	{
		size_t bucket = hash & (nbuckets - 1);

		unordered_map_node<Key, Value>* next = where->next;
		for (; buckets[bucket] == where; --bucket)
		{
			buckets[bucket] = next;
			if (!bucket)
				break;
		}

		if (where->prev)
			where->prev->next = where->next;
		if (next)
			next->prev = where->prev;
	}

	template<typename Node>
	struct unordered_map_iterator
	{
		Node* operator->() const;
		Node& operator*() const;
		Node* node;
	};

	template<typename Node>
	struct unordered_map_iterator<const Node>
	{
		unordered_map_iterator() {}
		unordered_map_iterator(unordered_map_iterator<Node> other)
			: node(other.node)
		{

		}

		const Node* operator->() const;
		const Node& operator*() const;
		const Node* node;
	};

	template<typename LNode, typename RNode>
	static inline bool operator==(const unordered_map_iterator<LNode>& lhs, const unordered_map_iterator<RNode>& rhs)
	{
		return lhs.node == rhs.node;
	}

	template<typename LNode, typename RNode>
	static inline bool operator!=(const unordered_map_iterator<LNode>& lhs, const unordered_map_iterator<RNode>& rhs)
	{
		return lhs.node != rhs.node;
	}

	template<typename Node>
	static inline void operator++(unordered_map_iterator<Node>& lhs)
	{
		lhs.node = lhs.node->next;
	}

	template<typename Node>
	inline Node* unordered_map_iterator<Node>::operator->() const
	{
		return node;
	}

	template<typename Node>
	inline Node& unordered_map_iterator<Node>::operator*() const
	{
		return *node;
	}

	template<typename Node>
	inline const Node* unordered_map_iterator<const Node>::operator->() const
	{
		return node;
	}

	template<typename Node>
	inline const Node& unordered_map_iterator<const Node>::operator*() const
	{
		return *node;
	}

	template<typename Key, typename Value, typename Alloc = TINYSTL_ALLOCATOR>
	class unordered_map
	{
	public:
		unordered_map();
		unordered_map(const unordered_map& other);
		~unordered_map();

		unordered_map& operator=(const unordered_map& other);


		typedef pair<Key, Value> value_type;
		
		typedef unordered_map_iterator<const unordered_map_node<Key, Value> > const_iterator;
		typedef unordered_map_iterator<unordered_map_node<Key, Value> > iterator;

		const_iterator begin() const;
		const_iterator end() const;

		void clear();
		bool empty() const;
		size_t size() const;

		const_iterator find(const Key& key) const;
		iterator find(const Key& key);
		pair<iterator, bool> insert(const pair<Key, Value>& p);
		void erase(const_iterator where);

		Value& operator[](const Key& key);

		void swap(unordered_map& other);

	private:

		typedef unordered_map_node<Key, Value>* pointer;

		size_t m_size;
		tinystl::buffer<pointer, Alloc> m_buckets;
	};

	template<typename Key, typename Value, typename Alloc>
	unordered_map<Key, Value, Alloc>::unordered_map()
		: m_size(0)
	{
		buffer_init<pointer, Alloc>(&m_buckets);
		buffer_resize<pointer, Alloc>(&m_buckets, 9, 0);
	}

	template<typename Key, typename Value, typename Alloc>
	unordered_map<Key, Value, Alloc>::unordered_map(const unordered_map& other)
		: m_size(other.m_size)
	{
		const size_t nbuckets = (size_t)(other.m_buckets.last - other.m_buckets.first);
		buffer_init<pointer, Alloc>(&m_buckets);
		buffer_resize<pointer, Alloc>(&m_buckets, nbuckets, 0);

		for (pointer it = *other.m_buckets.first; it; it = it->next)
		{
			unordered_map_node<Key, Value>* newnode = new(placeholder(), Alloc::static_allocate(sizeof(unordered_map_node<Key, Value>))) unordered_map_node<Key, Value>(it->first, it->second);
			newnode->next = newnode->prev = 0;

			unordered_map_node_insert(newnode, hash(it->first), m_buckets.first, nbuckets - 1);
		}
	}

	template<typename Key, typename Value, typename Alloc>
	unordered_map<Key, Value, Alloc>::~unordered_map()
	{
		clear();
		buffer_destroy<pointer, Alloc>(&m_buckets);
	}

	template<typename Key, typename Value, typename Alloc>
	unordered_map<Key, Value, Alloc>& unordered_map<Key, Value, Alloc>::operator=(const unordered_map<Key, Value, Alloc>& other)
	{
		unordered_map<Key, Value, Alloc>(other).swap(*this);
		return *this;
	}

	template<typename Key, typename Value, typename Alloc>
	inline typename unordered_map<Key, Value, Alloc>::const_iterator unordered_map<Key, Value, Alloc>::begin() const
	{
		const_iterator cit;
		cit.node = *m_buckets.first;
		return cit;
	}

	template<typename Key, typename Value, typename Alloc>
	inline typename unordered_map<Key, Value, Alloc>::const_iterator unordered_map<Key, Value, Alloc>::end() const
	{
		const_iterator cit;
		cit.node = 0;
		return cit;
	}

	template<typename Key, typename Value, typename Alloc>
	inline bool unordered_map<Key, Value, Alloc>::empty() const
	{
		return m_size == 0;
	}

	template<typename Key, typename Value, typename Alloc>
	inline size_t unordered_map<Key, Value, Alloc>::size() const
	{
		return m_size;
	}

	template<typename Key, typename Value, typename Alloc>
	inline void unordered_map<Key, Value, Alloc>::clear()
	{
		pointer it = *m_buckets.first;
		while (it)
		{
			const pointer next = it->next;
			it->~unordered_map_node<Key, Value>();
			Alloc::static_deallocate(it, sizeof(unordered_map_node<Key, Value>));

			it = next;
		}

		m_buckets.last = m_buckets.first;
		buffer_resize<pointer, Alloc>(&m_buckets, 9, 0);
		m_size = 0;
	}

	template<typename Key, typename Value, typename Alloc>
	inline typename unordered_map<Key, Value, Alloc>::iterator unordered_map<Key, Value, Alloc>::find(const Key& key)
	{
		iterator result;

		const size_t bucket = hash(key) & ((size_t)(m_buckets.last - m_buckets.first) - 2);
		for (pointer it = m_buckets.first[bucket], end = m_buckets.first[bucket + 1]; it != end; it = it->next)
		{
			if (it->first == key)
			{
				result.node = it;
				return result;
			}
		}

		result.node = 0;
		return result;
	}

	template<typename Key, typename Value, typename Alloc>
	inline typename unordered_map<Key, Value, Alloc>::const_iterator unordered_map<Key, Value, Alloc>::find(const Key& key) const
	{
		return const_cast<unordered_map<Key, Value, Alloc>*>(this)->find(key);
	}

	template<typename Key, typename Value, typename Alloc>
	inline pair<typename unordered_map<Key, Value, Alloc>::iterator, bool> unordered_map<Key, Value, Alloc>::insert(const pair<Key, Value>& p)
	{
		pair<iterator, bool> result;
		result.second = false;

		result.first = find(p.first);
		if (result.first.node != 0)
		{
			return result;
		}
		
		unordered_map_node<Key, Value>* newnode = new(placeholder(), Alloc::static_allocate(sizeof(unordered_map_node<Key, Value>))) unordered_map_node<Key, Value>(p.first, p.second);
		newnode->next = newnode->prev = 0;

		const size_t nbuckets = (size_t)(m_buckets.last - m_buckets.first);
		unordered_map_node_insert(newnode, hash(p.first), m_buckets.first, nbuckets - 1);

		++m_size;
		if (m_size + 1 > 4 * nbuckets)
		{
			pointer root = *m_buckets.first;
			
			const size_t newnbuckets = ((size_t)(m_buckets.last - m_buckets.first) - 1) * 8;
			m_buckets.last = m_buckets.first;
			buffer_resize<pointer, Alloc>(&m_buckets, newnbuckets + 1, 0);
			unordered_map_node<Key, Value>** buckets = m_buckets.first;

			while (root)
			{
				const pointer next = root->next;
				root->next = root->prev = 0;
				unordered_map_node_insert(root, hash(root->first), buckets, newnbuckets);
				root = next;
			}
		}

		result.first.node = newnode;
		result.second = true;
		return result;
	}

	template<typename Key, typename Value, typename Alloc>
	void unordered_map<Key, Value, Alloc>::erase(const_iterator where)
	{
		unordered_map_node_erase(where.node, hash(where->first), m_buckets.first, (size_t)(m_buckets.last - m_buckets.first) - 1);

		where->~unordered_map_node<Key, Value>();
		Alloc::static_deallocate((void*)where.node, sizeof(unordered_map_node<Key, Value>));
		--m_size;
	}

	template<typename Key, typename Value, typename Alloc>
	Value& unordered_map<Key, Value, Alloc>::operator[](const Key& key)
	{
		return insert(pair<Key, Value>(key, Value())).first->second;
	}

	template<typename Key, typename Value, typename Alloc>
	void unordered_map<Key, Value, Alloc>::swap(unordered_map& other)
	{
		size_t tsize = other.m_size;
		other.m_size = m_size, m_size = tsize;
		buffer_swap(&m_buckets, &other.m_buckets);
	}
}
#endif
