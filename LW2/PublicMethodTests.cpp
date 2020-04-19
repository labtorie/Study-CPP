#include "stdafx.h"
#include "CppUnitTest.h"
#include "../practical_work_2/huffman_coder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HuffmanTest
{		
	TEST_CLASS(CoderTests)
	{
	public:
		
		TEST_METHOD(getPriorityListTest)
		{
			// The only useful thing of publicity of this method is printing. 
			HuffmanCoder("boop beep beer").getPriorityList().printPriorityList();
		}

		TEST_METHOD(makeTreeTest) {
			// Same story
			HuffmanCoder("boop beep beer").makeTree().printTree();
		}

		TEST_METHOD(decodeTest) {
			string _test = HuffmanCoder("AAKKTb ").decode("01110001001100010001101100");
			string _expect = "A KAK KAKATb"; 
			for (size_t i = 0; _test[i] != '\0'; i++) {
				Assert::AreEqual(_test[i], _expect[i]);
			}
		}

		TEST_METHOD(encodeTest) {
			string _test = HuffmanCoder("AAKKTb ").encode();
			string _expect = "0101000010110011";
			for (size_t i = 0; _test[i] != '\0'; i++) {
				Assert::AreEqual(_test[i], _expect[i]);
			}
		}
	};
}