#pragma once

#include <functional>

using namespace std;

template <typename T, typename Comparable = T, T NONE = 0>
class Tree {
	struct Node {
		T elem;
		Node* left;
		Node* right;

		Node(T elem) : elem(elem), left(nullptr), right(nullptr) {}
	};

	Node* root;
	int len;
	function<Comparable(T)> key;

	void destroy(Node* node) {
		if (node != nullptr) {
			destroy(node->left);
			destroy(node->right);
			delete node;
		}
	}

	Node* add(Node* node, T elem) {
		if (node == nullptr) {
			node = new Node(elem);
		}
		else if (key(elem) < key(node->elem)) {
			node->left = add(node->left, elem);
		}
		else {
			node->right = add(node->right, elem);
		}
		return node;
	}

	T find(Node* node, Comparable val) {
		if (node == nullptr) {

			return NONE;
		}
		else if (val == key(node->elem)) {
			return node->elem;
		}
		else if (val < key(node->elem)) {
			return find(node->left, val);
		}
		else {
			return find(node->right, val);
		}
	}

	void find2(Node* node, Comparable val, function<void(T)> proc) {
		if (node != nullptr) {
			find2(node->left, val, proc);
			if (val == key(node->elem)) {
				proc(node->elem);
			}
			find2(node->right, val, proc);
		}

	}

	void inorder(Node* node, function<void(T)> proc) {
		if (node != nullptr) {
			inorder(node->left, proc);
			proc(node->elem);
			inorder(node->right, proc);
		}
	}

	void reverseinorder(Node* node, function<void(T)> proc) {
		if (node != nullptr) {
			reverseinorder(node->right, proc);
			proc(node->elem);
			reverseinorder(node->left, proc);
		}
	}



public:
	Tree(function<Comparable(T)> key = [](T a) {return a; })
		: root(nullptr), len(0), key(key) {}
	~Tree() {
		destroy(root);
	}
	int size() {
		return len;
	}
	void add(T elem) {
		root = add(root, elem);
	}
	//Búsqueda
	T find(Comparable val) {
		return find(root, val);
	}
	void find2(Comparable val, function<void(T)> proc) {
		find2(root, val, proc);
	}

	// Ordenamiento

	void inorder(function<void(T)> proc) {
		inorder(root, proc);
	}
	//para orden descendente
	void reverseinorder(function<void(T)> proc) {
		reverseinorder(root, proc);
	}




};