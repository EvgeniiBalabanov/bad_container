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
	void	reserve(size_t newSize) {
		if (capacity_ >= newSize)
			return;
		// Перенос старых данных в новую область памяти
		T* newArray = allocator_.allocate(newSize);
		for (size_t counter = 0; counter < size_; ++counter) {
			AllocatorTraits::construct(allocator_, newArray + counter, std::move_if_noexcept(array_[counter]));
		}
		// Удаление старых данных
		size_t counter = size_;
		while (counter--) {
			AllocatorTraits::destroy(allocator_, array_ + counter);
		}
		AllocatorTraits::deallocate(allocator_, array_, capacity_);
		// Подмена данных
		capacity_ = newSize;
		array_ = newArray;
	}

	// Modifiers
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
	// Non-member functions
	T&	operator[](size_t index) const {
		return array_[index];
	}

};

};