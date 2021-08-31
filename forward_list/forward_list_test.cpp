#include "forward_list.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <forward_list>

TEST(ForwardListCreate, DefaultCreate) {
	bad::forward_list<int>	list;
}

TEST(ForwardListAdd, PushFront) {
	std::forward_list<int>	orig;
	bad::forward_list<int>	list;
	std::vector<int> nums{12, 54, 21, 34, 9, 23};
	for (int num: nums) {
		orig.push_front(num);
		list.push_front(num);
	}
	auto iterOrig = orig.begin();
	auto iterList = list.begin();
	for (; iterOrig != orig.end() && iterList != list.end();) {
		EXPECT_EQ(*iterOrig, *iterList);
		iterList++;
		iterOrig++;
	}
}

TEST(ForwardListCopy, CopyContructor) {
	std::forward_list<int>	orig;
	bad::forward_list<int>	list;
	std::vector<int> nums{12, 54, 21, 34, 9, 23};
	for (int num: nums) {
		orig.push_front(num);
		list.push_front(num);
	}
	std::forward_list<int>	origCopy(orig);
	bad::forward_list<int>	listCopy(list);
	auto iterOrig = origCopy.begin();
	auto iterList = listCopy.begin();
	for (; iterOrig != origCopy.end() && iterList != listCopy.end();) {
		EXPECT_EQ(*iterOrig, *iterList);
		iterList++;
		iterOrig++;
	}
}