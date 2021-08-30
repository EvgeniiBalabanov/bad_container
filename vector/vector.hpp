#include <memory>

namespace bad {

template <typename T, class Allocator = std::allocator<T>>
class vector {
private:
	T*			array_;
	size_t		capacity_;
	size_t		size_;
	Allocator	allocator_;

	using AllocatorTraits = std::allocator_traits<Allocator>;

public:
	class iterator {
	private:
		T*		pointer_;
	public:
		iterator(T* pointer): pointer_(pointer) {}
		iterator(const iterator& other): pointer_(other.pointer_) {}
		T&	operator*(void) const {
			return *pointer_;
		}
		T*	operator->(void) const {
			return pointer_;
		}
		// Increment operation
		iterator&	operator++(void) {
			++pointer_;
			return *this;
		}
		iterator	operator++(int) {
			iterator copy = *this;
			++pointer_;
			return copy;
		}
		iterator&	operator--(void) {
			--pointer_;
			return *this;
		}
		iterator	operator--(int) {
			iterator copy = *this;
			--pointer_;
			return copy;
		}
		// Math operation
		iterator	operator+(size_t count) {
			return iterator(pointer_ + count);
		}
		iterator	operator-(size_t count) {
			return iterator(pointer_ - count);
		}
		iterator&	operator+=(size_t count) {
			pointer_ += count;
			return *this;
		}
		iterator&	operator-=(size_t count) {
			pointer_ -= count;
			return *this;
		}
		size_t	operator-(const iterator& other) const {
			return pointer_ - other.pointer_;
		}
		// Compare
		bool	operator==(const iterator& other) const {
			return pointer_ == other.pointer_;
		}
		bool	operator!=(const iterator& other) const {
			return pointer_ != other.pointer_;
		}
		bool	operator<(const iterator& other) const {
			return pointer_ < other.pointer_;
		}
	};

public:
	// Member functions
	vector(const Allocator& allocator = Allocator()): array_(nullptr), capacity_(0), size_(0), allocator_(allocator) {};
	vector(const vector<T, Allocator>& other): array_(nullptr), capacity_(other.capacity_), size_(other.size_), allocator_(other.allocator_) {
		array_ = AllocatorTraits::allocate(allocator_, capacity_);
		for (size_t counter = 0; counter < size_; ++counter) {
			AllocatorTraits::construct(allocator_, array_ + counter, other[counter]);
		}
	}
	vector(vector<T, Allocator>&& other): array_(other.array_), capacity_(other.capacity_), size_(other.size_), allocator_(other.allocator_) {
		other.array_ = nullptr;
		other.capacity_ = 0;
		other.size_ = 0;
	}
	~vector() {
		size_t counter = size_;
		while (counter--) {
			AllocatorTraits::destroy(allocator_, array_ + counter);
		}
		AllocatorTraits::deallocate(allocator_, array_, capacity_);
	}

	// Element access
	T&	operator[](size_t index) const {
		return array_[index];
	}

	// Iterators
	iterator	begin() const {
		return iterator(array_);
	}
	iterator	end() const {
		return iterator(array_ + size_);
	}
	// reverse_iterator	rbegin() const {
	// 	return iterator(array_ + size_ - 1);
	// }
	// reverse_iterator	rend() const {
	// 	return iterator(array_ - 1);
	// }

	// Capacity
	size_t	size(void) const {
		return size_;
	}
	size_t	capacity(void) const {
		return capacity_;
	}
	bool	empty(void) const {
		return !size_;
	};

	/*
	Производит аллокацию нового массива данных размера newSize
	и переносит/копирует старые данные в новый массив
	*/
	void	reserve(size_t newCapacity) {
		if (capacity_ >= newCapacity)
			return;
		// Перенос старых данных в новую область памяти
		T* newArray = allocator_.allocate(newCapacity);
		for (size_t counter = 0; counter < size_; ++counter) {
			AllocatorTraits::construct(allocator_, newArray + counter, std::move_if_noexcept(array_[counter]));
		}
		size_t saveSize = size_;
		// Удаление старых данных
		clear();
		// Подмена данных
		array_ = newArray;
		capacity_ = newCapacity;
		size_ = saveSize;
	}

	// Modifiers
	void	clear(void) {
		size_t counter = size_;
		while (counter--) {
			AllocatorTraits::destroy(allocator_, array_ + counter);
		}
		AllocatorTraits::deallocate(allocator_, array_, capacity_);
		array_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}

	void	push_back(const T& element) {
		if (capacity_ == size_)
			reserve(capacity_ ? capacity_ * 2 : 1);
		AllocatorTraits::construct(allocator_, array_ + size_, element);
		++size_;
	}
	template <typename ...Args>
	void	emplace_back(Args&&... args) {
		if (capacity_ == size_)
			reserve(capacity_ ? capacity_ * 2 : 1);
		AllocatorTraits::construct(allocator_, array_ + size_, std::forward<Args>(args)...);
		++size_;
	}
	void	pop_back(void) {
		if (size_ == 0)
			return ;
		AllocatorTraits::destroy(allocator_, array_ + --size_);
	}
	void	erase(iterator begin, iterator end) {
		size_t count = end - begin;
		iterator iter = begin;
		while (iter < this->end()) {
			AllocatorTraits::destroy(allocator_, &(*iter));
			if (iter + count < this->end())
				AllocatorTraits::construct(allocator_, &(*iter), std::move_if_noexcept(*(iter + count)));
			iter++;
		}
		size_ -= count;
	}
	// Non-member functions

};

};