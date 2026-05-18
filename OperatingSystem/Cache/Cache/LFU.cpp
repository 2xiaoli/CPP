#include <unordered_map>
#include <list>

template<typename K, typename V>
struct Node {
	K key;
	V value;
	int freq;

	Node() : key(), value(), freq(0) {}
	Node(K _key, V _value, int _freq) : key(_key), value(_value), freq(_freq) {}
};

template<typename K, typename V>
class LFUCache {
private:
	using NodeType = Node<K, V>;
	using ListIter = typename std::list<NodeType>::iterator;

	int capacity;
	int min_freq;
	std::unordered_map<int, std::list<NodeType>> freq_to_list;
	std::unordered_map<K, ListIter> key_to_iter;

	void updateFreq(ListIter nodeIter) {
		K tKey = nodeIter->key;
		V tValue = nodeIter->value;
		int tFreq = nodeIter->freq;
		freq_to_list[tFreq].erase(nodeIter);
		if (freq_to_list[tFreq].empty()) {
			freq_to_list.erase(tFreq);
			if (min_freq == tFreq) {
				min_freq++;
			}
		}
		freq_to_list[tFreq + 1].push_front(NodeType(tKey, tValue, tFreq + 1));
		key_to_iter[tKey] = freq_to_list[tFreq + 1].begin();
	}

public:
	LFUCache(int _capacity) : capacity(_capacity) {}

	V get(K key) {
		if (!key_to_iter.count(key)) {
			return V();
		}
		ListIter tIter = key_to_iter[key];
		V tValue = tIter->value;
		updateFreq(tIter);
		return tValue;
	}

	void put(K key, V value) {
		if (capacity <= 0) {
			return;
		}
		if (!key_to_iter.count(key)) {
			if (key_to_iter.size() + 1 > capacity) {
				key_to_iter.erase(freq_to_list[min_freq].back().key);
				freq_to_list[min_freq].pop_back();
				if (freq_to_list[min_freq].empty()) {
					freq_to_list.erase(min_freq);
				}
			}
			freq_to_list[1].push_front(NodeType(key, value, 1));
			key_to_iter[key] = freq_to_list[1].begin();
			min_freq = 1;
		}
		else {
			ListIter tIter = key_to_iter[key];
			tIter->value = value;
			updateFreq(tIter);
		}
	}
};