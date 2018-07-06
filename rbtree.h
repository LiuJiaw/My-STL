/*
 * rbtree.h
 *
 *  Created on: 2018年7月1日
 *      Author: garvin
 */

#ifndef RBTREE_H_
#define RBTREE_H_

#include "iterator_rbtree.h"
#include "construct.h"
#include "allocator.h"
#include "pair.h"

namespace MySTL{
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rbtree{
protected:
	typedef void* void_pointer;
	typedef _rbtree_node_base* base_ptr;
	typedef _rbtree_node<Value> rbtree_node;
	typedef simple_alloc<rbtree_node, Alloc> rbtree_node_allocator;

public:
	typedef Key key_type;
	typedef Value value_type;
	typedef Value* pointer;
	typedef Value& reference;
	typedef const Value* const_pointer;
	typedef const Value& const_reference;
	typedef rbtree_node* link_type;
	typedef _rbtree_color_type color_t;

	typedef size_t size_type;
	typedef ptrdiff_t diference_type;

protected:
	link_type getnode(){
		return rbtree_node_allocator::allocate();
	}
	void put_node(link_type p){
		rbtree_node_allocator::deallocate(p);
	}

	link_type create_node(const value_type& x){
		link_type tmp = getnode();
		construct(&tmp->value_field, x);
		return tmp;
	}
	link_type clone_node(link_type x){
		link_type tmp = create_node(x->value_field);
		tmp->color=x->color;
		tmp->left = NULL;
		tmp->right = NULL;
		return tmp;
	}
	void destroy_node(link_type x){
		destroy(&x->value_field);
		put_node(x);
	}

protected:
	size_type node_count;
	link_type header;
	Compare key_compare;

	link_type& root() const {
		return (link_type&)header->parent;
	}
	link_type& leftmost() const{
		return (link_type&)header->left;
	}
	link_type& rightmost() const{
		return (link_type&)header->right;
	}

	static link_type& left(link_type x){
		return x->left;
	}
	static link_type& right(link_type x){
		return x->right;
	}
	static link_type& parent(link_type x){
		return x->parent;
	}
	static color_t& color(link_type x){
		return (color_t&)x->color;
	}
	static reference value(link_type x){
		return x->value_field;
	}
	static const Key& key(link_type x){
		return KeyOfValue() (value(x));
	}

	static link_type& left(base_ptr x) {
		return (link_type&) (x->left);
	}
	static link_type& right(base_ptr x) {
		return (link_type&) (x->right);
	}
	static link_type& parent(base_ptr x) {
		return (link_type&) (x->parent);
	}
	static reference value(base_ptr x) {
		return ((link_type) x)->value_field;
	}
	static const Key& key(base_ptr x) {
		return KeyOfValue()(value(link_type(x)));
	}
	static color_t& color(base_ptr x) {
		return (color_t&) (link_type(x)->color);
	}

	static link_type minimum(link_type x){
		return (link_type) _rbtree_node_base::minimum(x);
	}
	static link_type maximum(link_type x){
		return (link_type) _rbtree_node_base::maximum(x);
	}

public:
	typedef _rbtree_iterator<value_type, reference, pointer> iterator;
	typedef _rbtree_iterator<value_type, const_reference, const_pointer> const_iterator;

private:
	iterator _insert(base_ptr x, base_ptr y, const Value& V){
		//正在研究......
	}
	void init(){
		header = getnode();
		color(header) = _rbtree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}

public:
	rbtree(const Compare& comp = Compare()) :node_count(0), key_compare(comp) {
		init();
	}
	~rbtree(){
		clear();
		put_node(header);
	}
	rbtree<Key, Value, KeyOfValue, Compare, Alloc>& operator = (const rbtree<Key, Value, KeyOfValue, Compare, Alloc>& x);

public:
	iterator begin(){
		return leftmost();
	}
	iterator end(){
		return rightmost();
	}
	bool empty(){
		return node_count == 0;
	}
	size_type size() const{
		return node_count;
	}
	Compare key_comp() const{
		return key_compare;
	}
	void clear(){
		while(node_count--){
			destroy_node(begin());
		}
	}

	iterator insert_equal(const Value& v){
		link_type y =header;
		link_type x = root();
		while( x!=NULL ){
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
		}
		return _insert(x, y, v);
	}

	pair<iterator, bool> insert_unique(const Value& v){
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while( x!=NULL ){
			y =x;
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}
		iterator j = iterator(y);
		if(comp){
			if(j == begin())
				return pair<iterator, bool>(_insert(x, y, v), true);
			else
				j--;
		}
		if(key_compare(key(j.node), KeyOfValue)(v))
			return pair<iterator, bool>(_insert<x, y, v>, true);
		return pair<iterator, bool>(j, false);
	}
};
}



#endif /* RBTREE_H_ */
