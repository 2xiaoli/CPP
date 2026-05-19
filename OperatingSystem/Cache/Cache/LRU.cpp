#include <unordered_map>

template <typename K, typename V>
struct DLinkedNode {
	K key;
	V value;
	DLinkedNode* pre, * next;

	DLinkedNode() : key(), value(), pre(nullptr), next(nullptr) {}
	DLinkedNode(K _key, V _value) : key(_key), value(_value), pre(nullptr), next(nullptr) {}
};

template <typename K, typename V>
class LRUCache {
private:
	using NodePtr = DLinkedNode<K, V>*;

	std::unordered_map<K, NodePtr> cache;
	NodePtr head, tail;
	int capacity, size;

	void headAddNode(NodePtr node) {
		node->next = head->next;
		node->pre = head;
		head->next->pre = node;
		head->next = node;
	}

	void removeNode(NodePtr node) {
		node->pre->next = node->next;
		node->next->pre = node->pre;
	}

	void moveToHead(NodePtr node) {
		removeNode(node);
		headAddNode(node);
	}

	NodePtr removeTail() {
		NodePtr tNode = tail->pre;
		removeNode(tNode);
		return tNode;
	}

public:
	LRUCache(int _capacity) : capacity(_capacity), size(0) {
		head = new DLinkedNode<K, V>();
		tail = new DLinkedNode<K, V>();
		head->next = tail;
		tail->pre = head;
	}

	V get(K key) {
		if (!cache.count(key)) {
			return V();
		}
		NodePtr tNode = cache[key];
		moveToHead(tNode);
		return tNode->value;
	}

	void put(K key, V value) {
		if (capacity <= 0) {
			return;
		}
		if (!cache.count(key)) {
			if (++size > capacity) {
				NodePtr remove = removeTail();
				cache.erase(remove->key);
				delete remove;
				size--;
			}
			NodePtr tNode = new DLinkedNode<K, V>(key, value);
			cache[key] = tNode;
			headAddNode(tNode);
		}
		else {
			NodePtr tNode = cache[key];
			tNode->value = value;
			moveToHead(tNode);
		}
	}
};