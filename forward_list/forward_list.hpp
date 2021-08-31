#include <memory>

namespace bad {

template <typename T>
class Node;

template <typename T, class Allocator = std::allocator<Node<T>>>
class forward_list {
private:
	struct Node {
		std::shared_ptr<Node>	next_;
		T						data_;
		Node(const T& data): next_(nullptr), data_(data) {}
		Node(std::shared_ptr<Node>& next, const T& data): next_(next), data_(data) {}
		Node(std::shared_ptr<Node>& next, T&& data): next_(next), data_(std::move(data)) {}
	};

	std::shared_ptr<Node>	begin_;
	std::weak_ptr<Node>		last_;
	Allocator				allocator_;

	using AllocatorTraits = std::allocator_traits<Allocator>;

public:
	struct iterator {
	private:
		std::shared_ptr<Node>	now_;
	public:
		iterator(const std::shared_ptr<Node>& now): now_(now) {}
		iterator(const iterator& other): now_(other.now_) {}
		T&		operator*(void) {
			return now_->data_;
		}
		T*		operator->(void) {
			return &now_->data_;
		}
		// Compare
		bool	operator==(const iterator& other) const {
			return now_ == other.now_;
		}
		bool	operator!=(const iterator& other) const {
			return now_ != other.now_;
		}
		// Increment operation
		iterator	operator++(int) {
			iterator	copy(*this);
			now_ = now_->next_;
			return copy;
		};
		iterator&	operator++(void) {
			now_ = now_->next_;
			return *this;
		}
	};

public:
	forward_list(const Allocator& allocator = Allocator()): begin_(nullptr), allocator_(allocator) {}
	forward_list(const forward_list& other): begin_(nullptr), allocator_(other.allocator_) {
		std::shared_ptr<Node>	previous = nullptr;
		std::shared_ptr<Node>	now = nullptr;
		for (auto iter = other.begin(); iter != other.end(); iter++) {
			now = std::make_shared<Node>(*iter);
			if (previous)
				previous->next_ = now;
			if (!begin_)
				begin_ = now;
		}
		last_ = now;
	}
	forward_list(forward_list&& other): begin_(std::move(other.begin_)), last_(std::move(other.last_)), allocator_(other.allocator_) {}
	~forward_list() {}

	// Iterators
	iterator	begin() const {
		return iterator(begin_);
	}

	iterator	end() const {
		return iterator(nullptr);
	}

	// Modifiers
	void	push_front(const T& value) {
		std::shared_ptr<Node> newBegin = std::make_shared<Node>(begin_, value);
		begin_ = newBegin;
		if (!last_.use_count())
			last_ = newBegin;
	}
};

};