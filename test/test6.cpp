#include <iostream>

using namespace std;

struct buf_page_t_ {
	int page_x;
};

struct buf_block_t {
	buf_page_t_ page;
	int page_y;
};

int main()
{
	buf_block_t obj;                 // 真正的对象
	//buf_page_t_* buf_page;
	buf_block_t* buf_block = &obj;
	//buf_page -> page_x = 111;
	buf_block -> page . page_x = 333;
	buf_block -> page_y = 222;
	buf_page_t_* block = (buf_page_t_*) buf_block;

	cout << block -> page_x << endl;
	return 0;
}