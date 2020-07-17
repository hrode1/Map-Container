#include <utility>
#include <cstddef>
#include <stdexcept>
#include <iostream>
double RandomNumber()
{
	double number=(double)rand()/((double)RAND_MAX+1);
	return number;
}

namespace cs540{	
	template <typename Key_T, typename Mapped_T>
	class MapEntry
	{
		public:
		typedef std::pair<const Key_T, Mapped_T> ValueType;
		ValueType* value;
		MapEntry* up;
		MapEntry* down;
		MapEntry* left;
		MapEntry* right;
		MapEntry() : up(NULL), down(NULL), left(NULL), right(NULL) {};
		MapEntry(const ValueType& val) : value(new std::pair<const Key_T, Mapped_T>(val)), up(NULL), down(NULL), left(NULL), right(NULL) {};
		Mapped_T getValue() { return value->second; }
		Key_T getKey() { return value->first; }
		Mapped_T setValue(Mapped_T val) 
		{
			Mapped_T oldValue = value->second;
			value->second = val;
			return oldValue;
		}
		~MapEntry()
		{
			// if(value)
				// delete value;
			up = NULL;
			down = NULL;
			left = NULL;
			right = NULL;
		}
	};
	
	template <typename Key_T, typename Mapped_T>
	class Map {
		public:
		
			MapEntry<Key_T,Mapped_T>* head; // First element of the top level
			MapEntry<Key_T,Mapped_T>* tail; // Last element of the top level
			size_t n; // number of entries in the Skip List
			int h; // Height
			double r; // Coin toss
		
			typedef std::pair<const Key_T, Mapped_T> ValueType;
			
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
			
			Map() // Constructor
			{
				MapEntry<Key_T,Mapped_T>* p1 = new MapEntry<Key_T,Mapped_T>();
				MapEntry<Key_T,Mapped_T>* p2 = new MapEntry<Key_T,Mapped_T>();				
				head = p1;
				tail = p2;
				p1->right = p2;
				p2->left = p1;
				n = 0; // No entries in Skip List
				h = 0;	// Height is 0
			}
			
			Map(const Map& map) // Copy constructor
			{
				MapEntry<Key_T,Mapped_T>* p1 = new MapEntry<Key_T,Mapped_T>();
				MapEntry<Key_T,Mapped_T>* p2 = new MapEntry<Key_T,Mapped_T>();				
				head = p1;
				tail = p2;
				p1->right = p2;
				p2->left = p1;
				n = 0; // No entries in Skip List
				h = 0;	// Height is 0				
				for(auto i = map.begin(); i!= map.end(); i++)
				{
					insert(* i);
				}
			}
			
			Map &operator=(const Map& map) // Copy assignment operator
			{
				// check for self-assignment
				if(&map == this)
					return *this;
				
				MapEntry<Key_T,Mapped_T>* p1 = new MapEntry<Key_T,Mapped_T>();
				MapEntry<Key_T,Mapped_T>* p2 = new MapEntry<Key_T,Mapped_T>();				
				head = p1;
				tail = p2;
				p1->right = p2;
				p2->left = p1;
				n = 0; // No entries in Skip List
				h = 0;	// Height is 0
				
				for(auto i = map.begin(); i!= map.end(); i++)
				{
					insert(* i);
				}
				return *this;
			}
			
			Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> init_list) // Initializer list constructor
			{
				MapEntry<Key_T,Mapped_T>* p1 = new MapEntry<Key_T,Mapped_T>();
				MapEntry<Key_T,Mapped_T>* p2 = new MapEntry<Key_T,Mapped_T>();				
				head = p1;
				tail = p2;
				p1->right = p2;
				p2->left = p1;
				n = 0; // No entries in Skip List
				h = 0;	// Height is 0
				for(auto i = init_list.begin(); i!= init_list.end(); i++)
				{
					insert(* i);
				}
			}
			
			~Map() // Destructor
			{
				MapEntry<Key_T,Mapped_T>* map_head = head;
				MapEntry<Key_T,Mapped_T>* temp;
				MapEntry<Key_T,Mapped_T>* temp2;
				
				while(map_head->down!=NULL)
					map_head=map_head->down;
				
				while(map_head != tail)
				{
					if(map_head==NULL)
						break;
					temp = map_head->right;
					while(map_head->up!=NULL)
					{
						temp2 = map_head->up;
						delete map_head;
						map_head = temp2;
					}
					delete map_head;
					map_head = temp;
				}
				delete map_head;
			}
			
			size_t size() const { return n; }
			bool empty() const { return (n == 0); }
															
			Iterator begin() // Returns an Iterator pointing to the first element
			{
				MapEntry<Key_T,Mapped_T>* p;
				p = head;
				while ( p->down != NULL )
				{
					p = p->down;
				}
				return Map<Key_T, Mapped_T>::Iterator(p->right);
			}
			
			Iterator end() // Returns an Iterator pointing one past the last element
			{
				MapEntry<Key_T,Mapped_T>* p;
				p = tail;
				while ( p->down != NULL )
				{
					p = p->down;
				}
				return Map<Key_T, Mapped_T>::Iterator(p);
			}
			
			ConstIterator begin() const // Returns a ConstIterator pointing to the first element
			{
				MapEntry<Key_T,Mapped_T>* p;
				p = head;
				while ( p->down != NULL )
				{
					p = p->down;
				}
				return Map<Key_T, Mapped_T>::ConstIterator(p->right);
			}
			
			ConstIterator end() const // Returns a ConstIterator pointing one past the last element
			{
				MapEntry<Key_T,Mapped_T>* p;
				p = tail;
				while ( p->down != NULL )
				{
					p = p->down;
				}
				return Map<Key_T, Mapped_T>::ConstIterator(p);
			}
			
			ReverseIterator rbegin() // Returns an ReverseIterator to the first element in reverse order, which is the last element in normal order
			{
				MapEntry<Key_T,Mapped_T>* p;
				p = tail;
				while ( p->down != NULL )
				{
					p = p->down;
				}
				return Map<Key_T, Mapped_T>::ReverseIterator(p->left);
			}
			
			ReverseIterator rend() // Returns an ReverseIterator pointing to one before the first element in normal order
			{
				MapEntry<Key_T,Mapped_T>* p;
				p = head;
				while ( p->down != NULL )
				{
					p = p->down;
				}
				return Map<Key_T, Mapped_T>::ReverseIterator(p);
			}
						
			Iterator findEntry(const Key_T& key)
			{
				// Start at "head"
				MapEntry<Key_T,Mapped_T>* p = head;
				while(true)
				{
					while ( !(p->right->right == tail->right) && ( (p->right->value->first < key) || (p->right->value->first == key) ) )
					{
					   p = p->right; // Moving to right
					}
					
					// Going down one level if possible
					if ( p->down != NULL )
					{  
					   p = p->down; // Going downwards
					}
					else
					{
					   break; // LOWEST level reached
					}
				}
				return Map<Key_T, Mapped_T>::Iterator(p);
			}
			
			ConstIterator findEntry(const Key_T& key) const
			{
				// Start at "head"
				MapEntry<Key_T,Mapped_T>* p = head;
				while(true)
				{
					while ( !(p->right->right == tail->right) && ( (p->right->value->first < key) || (p->right->value->first == key) ) )
					{
					   p = p->right; // Moving to right
					}
					
					// Going down one level if possible
					if ( p->down != NULL )
					{  
					   p = p->down; // Going downwards
					}
					else
					{
					   break; // LOWEST level reached
					}
				}
				return Map<Key_T, Mapped_T>::ConstIterator(p);
			}
			
			Iterator find(const Key_T& key)
			{
				// Start at "head"
				MapEntry<Key_T,Mapped_T>* p = head;
				while(true)
				{
					while ( !(p->right->right == tail->right) && ( (p->right->value->first < key) || (p->right->value->first == key) ) )
					{
					   p = p->right; // Moving to right
					}
					
					// Going down one level if possible
					if ( p->down != NULL )
					{  
					   p = p->down; // Going downwards
					}
					else
					{
					   break; // LOWEST level reached
					}
				}
				if(p->value->first == key)
					return Map<Key_T, Mapped_T>::Iterator(p);
				else
					return end();
			}
			
			
			ConstIterator find(const Key_T& key) const
			{
				// Start at "head"
				MapEntry<Key_T,Mapped_T>* p = head;
				while(true)
				{
					while ( !(p->right->right == tail->right) && ( (p->right->value->first < key) || (p->right->value->first == key) ) )
					{
					   p = p->right; // Moving to right
					}
					
					// Going down one level if possible
					if ( p->down != NULL )
					{  
					   p = p->down; // Going downwards
					}
					else
					{
					   break; // LOWEST level reached
					}
				}
				if(p->value->first == key)
					return Map<Key_T, Mapped_T>::ConstIterator(p);
				else
					return end();
			}
			
			Mapped_T &at(const Key_T& key)
			{
				if(findEntry(key).getCurrent()->value->first == key){
					return findEntry(key).getCurrent()->value->second;
				}
				else 
					throw std::out_of_range("out of range");
			}
			
			const Mapped_T &at(const Key_T& key) const
			{
				if(findEntry(key).getCurrent() == NULL){
					throw std::out_of_range("out of range");
				}
				else 
					return findEntry(key).getCurrent()->value->second;
			}
			
			Mapped_T &operator[](const Key_T& key)
			{
				/* If key is in the map, return a reference to the corresponding mapped object */
				if(findEntry(key).getCurrent()->value != tail->value)
				{
					if (findEntry(key).getCurrent()->value->first == key)
					{
						return findEntry(key).getCurrent()->value->second;
					}
				}
				/* If it is not, value initialize a mapped object for that key and returns a reference to it (after insert) */
				auto insert_ele = insert(std::make_pair(key, Mapped_T())).first;
				return insert_ele.getCurrent()->value->second;
			}
					
			std::pair<Iterator, bool> insert(const ValueType& data)
			{
				// Check if key is present in the map
				auto iter = findEntry(data.first);
				
				/* If the key already exists, no insertion is performed nor is the mapped object changed, 
				   and it returns an iterator pointing to the element with the same key, and false */
				if(iter.getCurrent()->value)
				{
					if (iter.getCurrent()->value->first == data.first)
					{
						return std::make_pair(iter, false);
					}
				}	
					
				/* If the key does not already exist in the map, it returns an iterator pointing to the new element, 
				   and true */
				MapEntry<Key_T,Mapped_T>* p;
				p = iter.getCurrent();
				
				// Insert new entry (k,v)
				MapEntry<Key_T,Mapped_T>* q = new MapEntry<Key_T,Mapped_T>(data);
				q->left = p;
				q->right = p->right;
				p->right->left = q;
				p->right = q;
				MapEntry<Key_T,Mapped_T>* ret_ptr;
				ret_ptr = q;
				int i = 0; // Current level = 0
								
				while ( RandomNumber() < 0.5 )
				{
					// Coin flip success: add one more level
					if ( i >= h )
					{
						MapEntry<Key_T,Mapped_T>* p1 = new MapEntry<Key_T,Mapped_T>();
						MapEntry<Key_T,Mapped_T>* p2 = new MapEntry<Key_T,Mapped_T>();
						h = h + 1;
						p1->right = p2;
						p1->down  = head;
						p2->left = p1;
						p2->down = tail;
						head->up = p1;
						tail->up = p2;
						head = p1;
						tail = p2;
					}

					// Scan backwards
					while ( p->up == NULL )
					{
						p = p->left;
					}

					p = p->up;
					
					// Add one more (k,v) to the column
					MapEntry<Key_T,Mapped_T>* e = new MapEntry<Key_T,Mapped_T>(data);
				
					e->left = p;
					e->right = p->right;
					e->down = q;
					p->right->left = e;
					p->right = e;
					q->up = e;

					q = e;		// Set q up for the next iteration
					i = i + 1;	// Current level increased by 1
				}
				n = n + 1;
				return std::make_pair(Map<Key_T, Mapped_T>::Iterator(ret_ptr), true);
			}

			template <typename IT_T>
			void insert(IT_T range_beg, IT_T range_end)
			{
				
				for(auto i = range_beg; i!= range_end; ++i)
				{
					insert(* i);
				}
			}
			
			void erase(Iterator pos)
			{
				auto key = pos.getCurrent()->value->first;
				erase(key);				
			}
			
			void erase(const Key_T& key)
			{
				MapEntry<Key_T,Mapped_T>* p;
				Map<Key_T, Mapped_T>::Iterator iter = findEntry(key);
				p = iter.getCurrent();
				if (key == p->getKey())
				{
					while(p->up != NULL)
					{
						p=p->up;
					}
					while(p->down != NULL)
					{
						p->left->right = p->right;
						p->right->left = p->left;
						p=p->down;
					}
					p->left->right = p->right;
					p->right->left = p->left;

					//Memory freeing
					MapEntry<Key_T,Mapped_T>* map_head = p;
					MapEntry<Key_T,Mapped_T>* temp2;
					
					while(map_head->up!=NULL)
					{
						temp2 = map_head->up;
						delete map_head;
						map_head = temp2;
					}
					delete map_head;
					// temp2 = nullptr;
					// iter=nullptr;
					// delete p;;
					
					// Decreasing map size by 1
					n=n-1;
				}
				else
				{
					throw std::out_of_range("out of range");
				}
			}
			
			void clear()
			{
				MapEntry<Key_T,Mapped_T>* p1 = new MapEntry<Key_T,Mapped_T>();
				MapEntry<Key_T,Mapped_T>* p2 = new MapEntry<Key_T,Mapped_T>();				
				head = p1;
				tail = p2;
				p1->right = p2;
				p2->left = p1;
				n = 0; // No entries in Skip List
				h = 0;	// Height is 0
			}
					
			template <typename Key_T1, typename Mapped_T1>
			friend bool operator==(const Map<Key_T1, Mapped_T1>& m1, const Map<Key_T1, Mapped_T1>& m2);
			
			template <typename Key_T1, typename Mapped_T1>
			friend bool operator!=(const Map<Key_T1, Mapped_T1>& m1, const Map<Key_T1, Mapped_T1>& m2);
			
			template <typename Key_T1, typename Mapped_T1>
			friend bool operator<(const Map<Key_T1, Mapped_T1>& m1, const Map<Key_T1, Mapped_T1>& m2);
						
			friend bool operator==(const Iterator &i1, const Iterator &i2)
			{
				return (i1.getCurrent() == i2.getCurrent())? true : false;
			}
			friend bool operator==(const ConstIterator &c1, const ConstIterator &c2)
			{
				return (c1.getCurrent() == c2.getCurrent())? true : false;
			}
			friend bool operator==(const Iterator &i, const ConstIterator &c)
			{
				return (i.getCurrent() == c.getCurrent())? true : false;
			}
			friend bool operator==(const ConstIterator &c, const Iterator &i)
			{
				return (c.getCurrent() == i.getCurrent())? true : false;
			}
			friend bool operator!=(const Iterator &i1, const Iterator &i2)
			{
				return (i1.getCurrent() != i2.getCurrent())? true : false;
			}
			friend bool operator!=(const ConstIterator &c1, const ConstIterator &c2)
			{
				return (c1.getCurrent() != c2.getCurrent())? true : false;
			}
			friend bool operator!=(const Iterator &i, const ConstIterator &c)
			{
				return (i.getCurrent() != c.getCurrent())? true : false;
			}
			friend bool operator!=(const ConstIterator &c, const Iterator &i)
			{
				return (c.getCurrent() != i.getCurrent())? true : false;
			}
			friend bool operator==(const ReverseIterator &r1, const ReverseIterator &r2)
			{
				return (r1.getCurrent() == r2.getCurrent())? true : false;
			}
			friend bool operator!=(const ReverseIterator &r1, const ReverseIterator &r2)
			{
				return (r1.getCurrent() != r2.getCurrent())? true : false;
			}
			
			class Iterator {
				public:
					Iterator(const Iterator& iter) : current(iter.getCurrent()){}
					
					Iterator(MapEntry<Key_T, Mapped_T>* map_entry) : current(map_entry){}
					
					~Iterator()
					{
						current = NULL;
					}
					
					Iterator& operator=(const Iterator& iter){
						current = iter.getCurrent();
						return *this;
					}
					
					Iterator& operator++() // Pre-increment
					{
						current = current->right;
						return *this;
					}
					
					Iterator operator++(int) // Post-increment
					{
						Map<Key_T, Mapped_T>::Iterator retval = current;
						current = current->right;
						return retval;
					}
					
					Iterator &operator--() // Pre-decrement
					{
						current = current->left;
						return *this;
					}
					
					Iterator operator--(int) // Post-decrement
					{
						Map<Key_T, Mapped_T>::Iterator retval = current;
						current = current->left;
						return retval;
					}
					
					ValueType &operator*() const
					{
						return *current->value;
					}
					
					ValueType *operator->() const
					{
						return current->value;
					}
					
					MapEntry<Key_T, Mapped_T>* getCurrent() const{return current;}
					
				private:
					MapEntry<Key_T, Mapped_T> *current;
			};
			
			class ConstIterator {
				public:
					ConstIterator(const ConstIterator &const_iter) : current(const_iter.getCurrent()) {}
					
					ConstIterator(const Iterator &iter) : current(iter.getCurrent()) {}
					
					ConstIterator(MapEntry<Key_T, Mapped_T>* map_entry) : current(map_entry){}
					
					~ConstIterator()
					{
						current = NULL;
					}
					
					ConstIterator& operator=(const ConstIterator &const_iter)
					{
						current = const_iter.getCurrent();
						return *this;
					}
					
					ConstIterator &operator++()
					{
						current = current->right;
						return *this;
					}
					
					ConstIterator operator++(int)
					{
						Map<Key_T, Mapped_T>::ConstIterator retval = current;
						current = current->right;
						return retval;
					}
					
					ConstIterator &operator--()
					{
						current = current->left;
						return *this;
					}
					
					ConstIterator operator--(int)
					{
						Map<Key_T, Mapped_T>::ConstIterator retval = current;
						current = current->right;
						return retval;
					}
					
					const ValueType &operator*() const
					{
						return *current->value;
					}
					const ValueType *operator->() const
					{
						return current->value;
					}
					
					MapEntry<Key_T, Mapped_T>* getCurrent() const{return current;}
					
				private:
					MapEntry<Key_T, Mapped_T> *current;
			};
			
			class ReverseIterator {
				public:
					ReverseIterator(const ReverseIterator &rev_iter) : current(rev_iter.getCurrent()) {}
					
					ReverseIterator(MapEntry<Key_T, Mapped_T>* map_entry) : current(map_entry){}
					
					~ReverseIterator()
					{
						current = NULL;
					}
					
					ReverseIterator& operator=(const ReverseIterator &rev_iter)
					{
						current = rev_iter.getCurrent();
						return *this;
					}
					
					ReverseIterator &operator++()
					{
						current = current->left;
						return *this;
					}
					
					ReverseIterator operator++(int)
					{
						Map<Key_T, Mapped_T>::ReverseIterator retval = current;
						current = current->left;
						return retval;
					}
					
					ReverseIterator &operator--()
					{
						current = current->right;
						return *this;
					}
					
					ReverseIterator operator--(int)
					{
						Map<Key_T, Mapped_T>::ReverseIterator retval = current;
						current = current->right;
						return retval;
					}
					
					ValueType &operator*() const
					{
						return *current->value;
					}
					
					ValueType *operator->() const
					{
						return current->value;
					}
					
					MapEntry<Key_T, Mapped_T>* getCurrent() const{return current;}
					
				private:
					MapEntry<Key_T, Mapped_T> *current;
			};
	};
	
	template <typename Key_T, typename Mapped_T>
	bool operator==(const Map<Key_T, Mapped_T>& m1, const Map<Key_T, Mapped_T>& m2)
	{
		// comparing map sizes
		if(m1.size() != m2.size()) 
		{
			return false;
		}
		auto m1_iter = m1.begin();
		auto m2_iter = m2.begin();
		while(m1_iter != m1.end() && m2_iter != m2.end()){
			if(*m1_iter != *m2_iter){
				return false;
			}
			++m1_iter;
			++m2_iter;
		}
		return true;
	}
	
	template <typename Key_T, typename Mapped_T>
	bool operator!=(const Map<Key_T, Mapped_T>& m1, const Map<Key_T, Mapped_T>& m2)
	{
		return (m1==m2?false:true);
	}
	
	template <typename Key_T, typename Mapped_T>
	bool operator<(const Map<Key_T, Mapped_T>& m1, const Map<Key_T, Mapped_T>& m2)
	{	
		// comparing map sizes
		if(m1.size() < m2.size())
			return true;
		if(m2.size() < m1.size()) 
			return false;
		
		// comparing maps element by element
		auto m1_iter = m1.begin();
		auto m2_iter = m2.begin();
		bool compare1, compare2;
		
		while(m1_iter != m1.end() && m2_iter != m2.end()){
			compare1 = (*m1_iter).first < (*m2_iter).first;
			compare2 = (*m2_iter).first < (*m1_iter).first;
			if(compare1) 
				return true;
			if(compare2) 
				return false;
			++m1_iter;
			++m2_iter;
		}
		
		return false;
	}
}