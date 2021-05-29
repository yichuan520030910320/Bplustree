//
// Created by 18303 on 2021/5/7.
//

#ifndef BPLUSTREE_VECTOR_H
#define BPLUSTREE_VECTOR_H
#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include <iostream>
#include <climits>
#include <cstddef>
#include <cstring> //memset

#define MAX_CAPACITY 20
//attention when allocate the space ,it must operator twice,when delete space it must operator twice too
namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */

    template<typename T>
    class vector {
    public:
        size_t current_size, capacity;
        T **element;

        void doublespace() {
            T **temp;
            capacity *= 2;
            temp = new T *[capacity];
            for (int i = 0; i < current_size; ++i) {
                (temp[i]) = element[i];
            }
            delete[]element;
            element = temp;
        }

        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;

        class iterator {
            friend class vector<T>;

            friend class const_iterator;

        private:
            int pos;
            T **vec;
            /**
		 * TODO add data members
		 *   just add whatever you want.
		 */
        public:
            iterator() = default;

            iterator(T **temp, int temppos) {
                vec = temp;
                pos = temppos;
            }

            iterator(const iterator &other) {
                pos = other.pos;
                vec = other.vec;
            }

            iterator &operator=(const iterator &other) {
                if (this == &other) return *this;
                vec = other.vec;
                pos = other.pos;
                return *this;
            }


            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            int getpos() {
                return pos;
            }

            iterator operator+(const int &n) const {
                return iterator(vec, pos + n);
                //TODO
            }

            iterator operator-(const int &n) const {
                return iterator(vec, pos - n);
                //TODO
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if (!(vec == rhs.vec)) throw invalid_iterator();
                if (pos - rhs.pos > 0) {
                    return pos - rhs.pos;
                } else {
                    return rhs.pos - pos;
                }
                //TODO
            }

            iterator &operator+=(const int &n) {
                pos += n;
                return *this;
                //TODO
            }

            iterator &operator-=(const int &n) {
                pos -= n;
                return *this;
                //TODO
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator temp = this;
                pos++;
                return temp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                pos++;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator temp = this;
                pos--;
                return temp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                pos--;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return *(vec[pos]);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return (pos == rhs.pos && vec == rhs.vec);
            }

            bool operator==(const const_iterator &rhs) const {
                return (pos == rhs.pos && vec == rhs.vec);
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(pos == rhs.pos && vec == rhs.vec);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(pos == rhs.pos && vec == rhs.vec);
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
            friend class vector<T>;

            friend class iterator;

        private:
            T **vec;
            int pos;
        public:
        public:
            const_iterator() = default;

            const_iterator(T **temp, int temppos) {
                vec = temp;
                pos = temppos;
            }

            const_iterator(const iterator &other) {
                pos = other.pos;
                vec = other.vec;
            }

            const_iterator &operator=(const const_iterator &other) {
                if (this == &other) return *this;
                vec = other.vec;
                pos = other.pos;
                return *this;
            }

            const_iterator operator+(const int &n) const {
                return const_iterator(vec, pos + n);
                //TODO
            }

            const_iterator operator-(const int &n) const {
                return const_iterator(vec, pos + n);
                //TODO
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const {
                if (!(vec == rhs.vec)) throw invalid_iterator();
                if (pos - rhs.pos > 0) {
                    return pos - rhs.pos;
                } else {
                    return rhs.pos - pos;
                }
                //TODO
            }

            const_iterator &operator+=(const int &n) {
                pos += n;
                return *this;
                //TODO
            }

            const_iterator &operator-=(const int &n) {
                pos -= n;
                return *this;
                //TODO
            }

            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator temp = this;
                pos++;
                return temp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                pos++;
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator temp = this;
                pos--;
                return temp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                pos--;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return *(vec[pos]);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return (pos == rhs.pos && vec == rhs.vec);
            }

            bool operator==(const const_iterator &rhs) const {
                return (pos == rhs.pos && vec == rhs.vec);
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(pos == rhs.pos && vec == rhs.vec);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(pos == rhs.pos && vec == rhs.vec);
            }
        };

        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        vector() {
            current_size = 0;
            capacity = MAX_CAPACITY;
            element = new T *[capacity];//understand it to allocate space for element T[i] is one floor pointer
            memset(element, 0, capacity * sizeof(T *));
        }

        vector(const vector &other) {
            current_size = other.current_size;
            capacity = other.capacity;
            element = new T *[capacity];
            memset(element, 0, capacity * sizeof(T *));
            for (int i = 0; i < current_size; ++i) {
                element[i] = new T(*other.element[i]);
            }
        }

        /**
         * TODO Destructor
         */
        ~vector() {
            for (int i = 0; i < current_size; ++i)
                delete element[i];
            delete[] element;
            //todo
        }

        /**
         * TODO Assignment operator
         */


        vector &operator=(const vector &other) {

            if (this == &other) return *this;
            for (int i = 0; i < current_size; ++i) {
                delete element[i];
            }
            delete[] element;
            current_size = other.current_size;
            capacity = other.capacity;
            element = new T *[capacity];
            for (int i = 0; i < current_size; ++i) {
                element[i] = new T(*other.element[i]);
            }
            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T &at(const size_t &pos) {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return *(element[pos]);
        }

        const T &at(const size_t &pos) const {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return *(element[pos]);
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos) {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return *(element[pos]);
        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= current_size) throw index_out_of_bound();
            return *(element[pos]);
        }

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const {
            if (current_size == 0) throw container_is_empty();
            return *(element[0]);
        }

        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T &back() const {
            if (current_size == 0) throw container_is_empty();
            return *(element[current_size - 1]);
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
//	    iterator temp;
//	    temp.pos=0;
//	    temp.vec=element;
//        return temp;
            return iterator(element, 0);
        }

        const_iterator cbegin() const {
            const_iterator temp;
            temp.pos = 0;
            temp.vec = element;
            return temp;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            iterator temp;
            temp.pos = current_size;
            temp.vec = element;
            return temp;
        }

        const_iterator cend() const {
            const_iterator temp;
            temp.pos = current_size;
            temp.vec = element;
            return temp;
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return current_size == 0;
        }

        /**
         * returns the number of elements
         */
        size_t size() const {
            return current_size;
        }

        /**
         * clears the contents
         */
        void clear() {
            for (int i = 0; i < current_size; ++i) {
                delete element[i];
            }
            delete[] element;
            current_size = 0;
            capacity = MAX_CAPACITY;
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            if (current_size == capacity) doublespace();
            current_size++;
            for (int i = current_size - 1; i > pos.pos; --i) {
                element[i] = element[i - 1];
            }
            element[pos.pos] = new T(value);
            return iterator(element, pos.pos);
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > current_size) throw index_out_of_bound();
            if (current_size == capacity) doublespace();
            current_size++;
            for (int i = current_size - 1; i > ind; --i) {
                element[i] = element[i - 1];
            }
            element[ind] = new T(value);
            return iterator(element, ind);
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
//        if (pos.vec!=element){return ;}
            current_size--;
            delete element[pos.pos];
            for (int i = pos.pos; i < current_size; ++i) {
                element[i] = element[i + 1];
            }
            element[current_size] = nullptr;
            iterator temp;
            temp.vec = element;
            temp.pos = ++pos.pos;
            return temp;
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind >= current_size) throw index_out_of_bound();
            current_size--;
            delete element[ind];
            for (int i = ind; i < current_size; ++i) {
                element[i] = element[i + 1];
            }
            element[current_size] = nullptr;
            int pos1 = ind;
            iterator temp;
            temp.vec = element;
            temp.pos = ++pos1;
            return temp;
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            //std::cout<<current_size<<"*******^^^^"<<std::endl;
            if (current_size == capacity) doublespace();
            element[current_size] = new T(value);
            current_size++;
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if (current_size == 0) throw container_is_empty();
            delete element[current_size - 1];
            element[current_size - 1] = nullptr;
            current_size--;
        }
    };


}

#endif
#endif //BPLUSTREE_VECTOR_H
