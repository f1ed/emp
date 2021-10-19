#include <typeinfo>
#include "emp-tool/emp-tool.h"
#include <iostream>
using namespace std;
using namespace emp;

template<typename Op, typename Op2>
void test_int(int party, int range1 = 1<<25, int range2 = 1<<25, int runs = 1000) {
	PRG prg;
	for(int i = 0; i < runs; ++i) {
		long long ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		ia %= range1;
		ib %= range2;
		while( Op()(int(ia), int(ib)) != Op()(ia, ib) ) {
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			ia %= range1;
			ib %= range2;
		}

		Integer a(32, ia, ALICE);
		Integer b(32, ib, BOB);

		Integer res = Op2()(a,b);

		if (res.reveal<int32_t>(PUBLIC) != Op()(ia,ib)) {
			cout << a.reveal<int32_t>()<<endl;
			cout << ia <<"\t"<<ib<<"\t"<<Op()(ia,ib)<<"\t"<<res.reveal<int32_t>(PUBLIC)<<endl;
		}
		assert(res.reveal<int32_t>(PUBLIC) == Op()(ia,ib));
	}
	cout << typeid(Op2).name()<<"\t\t\tDONE"<<endl;
}

void scratch_pad() {
	Integer a(32, 19, ALICE);
	Integer b(32, 11, ALICE);
	cout <<a.reveal<int32_t>(PUBLIC)<<endl;
	cout <<b.reveal<int32_t>(PUBLIC)<<endl;
	cout <<(a+b).reveal<int32_t>(PUBLIC)<<endl;
//	cout << "LZ "<<a.leading_zeros().reveal<int64_t>(PUBLIC)<<endl;
}

void test_if_else(){
	// OK;
	cout<<"Test: ifThenElse"<<endl;
	emp::Integer A(2, 3, 0);
	emp::Integer B(2, 0, 0);
	emp::Integer D(2, 9, 0);
	Bit cond(true, 0);
	emp::ifThenElse(D.bits.data(), A.bits.data(), B.bits.data(), 2, cond);
	cout<<D.reveal<uint32_t>(PUBLIC)<<endl;
	if(D.reveal<int32_t>(PUBLIC)==3)cout<<"true\n";
	else cout<<"false\n";
}

void test_condNeg(){
	cout<<"Test: test condNeg"<<endl;
	Integer A(32, (int32_t)3, 0);
	Integer B(32, (int32_t)100, 0);
	cout<<"A="<<A.reveal<int32_t>(PUBLIC)<<endl;
	cout<<"B="<<B.reveal<int32_t>(PUBLIC)<<endl;
	Bit cond(true, 0);
	emp::condNeg(cond, B.bits.data(), A.bits.data(), 32);
	cout<<"B="<<B.reveal<int32_t>(PUBLIC)<<endl;
	if (B.reveal<int32_t>(PUBLIC)==3)cout<<"true\n";
	else cout<<"false\n";
}

void test_negCond(){
	// TODO: bug !
	cout<<"Test: test new condNeg"<<endl;
	Integer A(32, (uint32_t)3, 0);
	Integer B(32, (uint32_t)100, 0);
	cout<<"A="<<A.reveal<uint32_t>(PUBLIC)<<endl;
	cout<<"B="<<B.reveal<uint32_t>(PUBLIC)<<endl;
	Bit cond(true, 0);
	emp::negCond(cond, B.bits.data(), A.bits.data(), 32);
	cout<<"B="<<B.reveal<uint32_t>(PUBLIC)<<endl;
	if (B.reveal<uint32_t>(PUBLIC)==3)cout<<"true\n";
	else cout<<"false\n";
}

int main(int argc, char** argv) {
	// int party = PUBLIC;
	setup_plain_prot(false, "");

//	scratch_pad();return 0;
	// test_int<std::plus<int>, std::plus<Integer>>(party);
	// test_int<std::minus<int>, std::minus<Integer>>(party);
	// test_int<std::multiplies<int>, std::multiplies<Integer>>(party);
	// test_int<std::divides<int>, std::divides<Integer>>(party);
	// test_int<std::modulus<int>, std::modulus<Integer>>(party);

	// test_int<std::bit_and<int>, std::bit_and<Integer>>(party);
	// test_int<std::bit_or<int>, std::bit_or<Integer>>(party);
	// test_int<std::bit_xor<int>, std::bit_xor<Integer>>(party);
	// cout << CircuitExecution::circ_exec->num_and()<<endl;
	// test_if_else();
	test_condNeg();
	finalize_plain_prot();
}
