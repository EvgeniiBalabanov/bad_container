#include <gtest/gtest.h>
#include "vector.hpp"
#include <vector>
#include <string>

struct SimpleClass {
	int			i_;
	size_t		st_;
	std::string	s_;

	SimpleClass(int i, size_t st, const std::string& s): i_(i), st_(st), s_(s) {}
	bool	operator==(const SimpleClass& other) const {
		return (
			i_ == other.i_ &&
			st_ == other.st_ &&
			s_ == other.s_);
	}
};

template<typename T>
void	vectorEqually(const bad::vector<T>& my, const std::vector<T>& original) {
	EXPECT_EQ(my.size(), original.size());
	EXPECT_EQ(my.capacity(), original.capacity());
	EXPECT_EQ(my.empty(), original.empty());
	for (size_t counter = 0; counter < std::min(my.size(), original.size()); counter++) {
		EXPECT_EQ(my[counter], original[counter]);
	}
}

TEST(VectorCreate, CreateEmptyVector) {
	bad::vector<int>	my;
	std::vector<int>	original;
	vectorEqually(my, original);
}

TEST(VectorCreate, CopyEmptyVector) {
	bad::vector<int>	my;
	bad::vector<int>	my2(my);
	std::vector<int>	original;
	std::vector<int>	original2(original);
	vectorEqually(my, original);
	vectorEqually(my2, original2);
}

TEST(VectorCreate, CopyVector) {
	bad::vector<int>	my;
	std::vector<int>	original;
	std::vector<int>	nums{0,1,5,16,31,32,33,400};
	for (int num: nums) {
		my.push_back(num);
		original.push_back(num);
	}
	bad::vector<int>	my2(my);
	std::vector<int>	original2(original);
	vectorEqually(my, original);
	vectorEqually(my2, original2);
}

template<typename T>
void	reserveEmpty(size_t newSize) {
	bad::vector<T>	my;
	std::vector<T>	original;

	my.reserve(newSize);
	original.reserve(newSize);
	vectorEqually(my, original);
}

TEST(VectorCreate, ReserveEmpty) {
	std::vector<size_t> sizes{0,1,5,16,31,32,33,400};

	for (size_t newSize: sizes) {
		reserveEmpty<int>(newSize);
		reserveEmpty<long*>(newSize);
	}
}

TEST(VectorAddElement, pushBackInt) {
	std::vector<int>	nums{0,1,5,16,31,32,33,400};
	bad::vector<int>	my;
	std::vector<int>	original;

	for (int num: nums) {
		vectorEqually(my, original);
		my.push_back(num);
		original.push_back(num);
	}
	vectorEqually(my, original);
}

TEST(VectorAddElement, pushBackSimpleClass) {
	std::vector<SimpleClass>	elements{SimpleClass{0,1,"Test"}, SimpleClass{16,31,"Hello"}, SimpleClass{33,400, ""}};
	bad::vector<SimpleClass>	my;
	std::vector<SimpleClass>	original;

	for (const SimpleClass& element: elements) {
		vectorEqually(my, original);
		my.push_back(element);
		original.push_back(element);
	}
	vectorEqually(my, original);
}

TEST(VectorAddElement, emplaceBackInt) {
	std::vector<int>	nums{0,1,5,16,31,32,33,400};
	bad::vector<int>	my;
	std::vector<int>	original;

	for (int num: nums) {
		vectorEqually(my, original);
		my.emplace_back(num);
		original.emplace_back(num);
	}
	vectorEqually(my, original);
}

TEST(VectorAddElement, emplaceBackSimpleClass) {
	std::vector<SimpleClass>	elements{SimpleClass{0,1,"Test"}, SimpleClass{16,31,"Hello"}, SimpleClass{33,400, ""}};
	bad::vector<SimpleClass>	my;
	std::vector<SimpleClass>	original;

	for (SimpleClass element: elements) {
		vectorEqually(my, original);
		my.emplace_back(element.i_, element.st_, element.s_);
		original.emplace_back(element.i_, element.st_, element.s_);
	}
	vectorEqually(my, original);
}

TEST(VectorRemoveElement, popBackInt) {
	std::vector<int>	nums{0,1,5,16,31,32,33,400};
	bad::vector<int>	my;
	std::vector<int>	original;

	for (int num: nums) {
		vectorEqually(my, original);
		my.emplace_back(num);
		original.emplace_back(num);
	}
	while (original.size()) {
		vectorEqually(my, original);
		my.pop_back();
		original.pop_back();
	}
	for (int num: nums) {
		vectorEqually(my, original);
		my.emplace_back(num);
		original.emplace_back(num);
	}
	while (original.size()) {
		vectorEqually(my, original);
		my.pop_back();
		original.pop_back();
	}
	vectorEqually(my, original);
}

TEST(VectorIterator, equality) {
	bad::vector<int>	my;
	EXPECT_EQ(my.begin(), my.end());
}

TEST(VectorIterator, addNum) {
	bad::vector<int>	my;
	EXPECT_EQ(my.begin() + 6, my.end() + 6);
}

void	caseErase(size_t start, size_t end) {
	std::vector<int>	nums{0,1,5,16,31,32,33,400};
	bad::vector<int>	my;
	std::vector<int>	original;

	for (int num: nums) {
		my.emplace_back(num);
		original.emplace_back(num);
	}
	my.erase(my.begin() + start, my.begin() + end);
	original.erase(original.begin() + start, original.begin() + end);
	vectorEqually(my, original);
}

TEST(VectorRemoveElement, eraseInt) {
	caseErase(0, 8);
	caseErase(0, 0);
	caseErase(0, 1);
	caseErase(0, 4);
	caseErase(4, 4);
	caseErase(4, 5);
	caseErase(4, 6);
	caseErase(4, 8);
	caseErase(8, 8);
}