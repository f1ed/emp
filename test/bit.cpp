#include "emp-tool/emp-tool.h"
#include <iostream>
using namespace std;
using namespace emp;

void test_bit() {
	bool b[] = {true, false};
	int p[] = {PUBLIC, ALICE, BOB};

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 3; ++j)
			for(int k = 0; k < 2; ++k)
				for (int l= 0; l < 3; ++l)  {
					{
						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1&b2).reveal(PUBLIC);
						if(res != (b[i] and b[k])) {
							cout<<"AND" <<i<<" "<<j<<" "<<k<<" "<<l<<" "<<res<<endl;
							error("test bit error!");
						}
						res = (b1 & b1).reveal(PUBLIC);
						if (res != b[i]) {
							cout<<"AND" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

						res = (b1 & (!b1)).reveal(PUBLIC);
						if (res) {
							cout<<"AND" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

					}
					{
						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1^b2).reveal(PUBLIC);
						if(res != (b[i] xor b[k])) {
							cout <<"XOR"<<i<<" "<<j<<" "<<k<<" "<<l<< " " <<res<<endl;
							error("test bit error!");
						}

						res = (b1 ^ b1).reveal(PUBLIC);
						if (res) {
							cout<<"XOR" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

						res = (b1 ^ (!b1)).reveal(PUBLIC);
						if (!res) {
							cout<<"XOR" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

					}
				}
	cout <<"success!"<<endl;
}

void test_bit_condNeg()
{
	// bug
	cout<<"Test: bit condNeg"<<endl;
	Bit cond(true, PUBLIC);
	Bit dest(true, PUBLIC);
	Bit src(false, PUBLIC);
	emp::bit_condNeg(cond, dest, src);
	if (dest.reveal(PUBLIC)==true)cout<<"true\n";
	else cout<<"false\n";
}

void test_bit_ifThenElse()
{
	cout<<"Test: bit ifthenelse"<<endl;
	Bit cond(true, PUBLIC);
	Bit dest(false, PUBLIC);
	Bit tsrc(true, PUBLIC);
	Bit fsrc(false, PUBLIC);
	emp::bit_ifThenElse(dest, tsrc, fsrc, cond);
	if (dest.reveal(PUBLIC)==true)cout<<"true\n";
	else cout<<"false\n";
}
int main(int argc, char** argv) {
	setup_plain_prot(false, "");
	test_bit();
	// test_bit_condNeg();
	// test_bit_ifThenElse();
	finalize_plain_prot();
}
