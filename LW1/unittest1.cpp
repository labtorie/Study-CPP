#include "stdafx.h"
#include "CppUnitTest.h"
#include "../PW1R/treemap.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MAP_TEST
{

	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(find_test)
		{
			TreeMap<int, int> map;
			map.insert(1, 11);
			map.insert(2, 22);
			Assert::AreEqual(map.find(1), 11);
			Assert::AreEqual(map.find(2), 22);
		}

		TEST_METHOD(insert_test)
		{
			TreeMap<int, int> map;
			map.insert(1, 11);
			map.insert(2, 22);
			Assert::AreEqual(map.find(1), 11);
			Assert::AreEqual(map.find(2), 22);
		}

		TEST_METHOD(remove_test) {


			auto func = [] {
				TreeMap<int, int> map;
				map.insert(1, 11);
				map.insert(2, 22);
				map.insert(3, 33);
				map.remove(2); map.find(2); };

			Assert::ExpectException<std::invalid_argument>(func);

		}
		TEST_METHOD(clear_test) {
			auto func = [] {
				TreeMap<int, int> map;
				map.insert(1, 11);
				map.insert(2, 22);
				map.insert(3, 33);
				map.clear(); map.find(1); };

			Assert::ExpectException<std::invalid_argument>(func);

		}

		TEST_METHOD(get_keys_test) {
			TreeMap<int, int> map;
			map.insert(1, 11);
			map.insert(2, 22);
			map.insert(3, 33);
			Assert::AreEqual(map.get_keys().at(0), 1);
			Assert::AreEqual(map.get_keys().at(1), 2);
			Assert::AreEqual(map.get_keys().at(2), 3);
		}

		TEST_METHOD(get_values_test) {
			TreeMap<int, int> map;
			map.insert(1, 11);
			map.insert(2, 22);
			map.insert(3, 33);
				Assert::AreEqual(map.get_values().at(0), 11);
				Assert::AreEqual(map.get_values().at(1), 22);
				Assert::AreEqual(map.get_values().at(2), 33);
		}
		TEST_METHOD(print_exception_test) {
			auto func = [] {
				TreeMap<int, int> map;
				map.print();
			};

			Assert::ExpectException<std::out_of_range>(func);
		}
	};
}
