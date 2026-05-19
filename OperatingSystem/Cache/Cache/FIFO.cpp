#include <unordered_map>
#include <list>

template<typename K, typename V>
struct Node {
	K key;
	V value;

	Node() : key(), value() {}
	Node(K _key, V _value) : key(_key), value(_value) {}
};

template<typename K, typename V>
class FIFOCache {
private:
	using NodeType = Node<K, V>;
	using ListIter = typename std::list<Node<K, V>>::iterator;

	int capacity;
	std::list<NodeType> NodeList;
	std::unordered_map<K, ListIter> key_to_iter;

public:
	FIFOCache(int _capacity) : capacity(_capacity) {}

	V get(K key) {
		if (!key_to_iter.count(key)) {
			return V();
		}
		ListIter tIter = key_to_iter[key];
		return tIter->value;
	}

	void put(K key, V value) {
		if (capacity <= 0) {
			return;
		}
		if (!key_to_iter.count(key)) {
			if (key_to_iter.size() + 1 > capacity) {
				key_to_iter.erase(NodeList.back().key);
				NodeList.pop_back();
			}
			NodeList.push_front(NodeType(key, value));
			key_to_iter[key] = NodeList.begin();
		}
		else {
			ListIter tIter = key_to_iter[key];
			tIter->value = value;
		}
	}
};