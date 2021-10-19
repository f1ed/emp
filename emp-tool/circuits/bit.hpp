inline Bit::Bit(bool b, int party) {
	if (party == PUBLIC)
		bit = CircuitExecution::circ_exec->public_label(b);
	else ProtocolExecution::prot_exec->feed(&bit, party, &b, 1); 
}

inline Bit Bit::select(const Bit & select, const Bit & new_v) const{
	Bit tmp = *this;
	tmp = tmp ^ new_v;
	tmp = tmp & select;
	return *this ^ tmp;
}

template<typename O>
inline O Bit::reveal(int party) const {
	O res;
	ProtocolExecution::prot_exec->reveal(&res, party, &bit, 1);
	return res;
}

template<>
inline string Bit::reveal<string>(int party) const {
	bool res;
	ProtocolExecution::prot_exec->reveal(&res, party, &bit, 1);
	return res ? "true" : "false";
}

inline Bit Bit::operator==(const Bit& rhs) const {
	return !(*this ^ rhs);
}

inline Bit Bit::operator!=(const Bit& rhs) const {
	return (*this) ^ rhs;
}

inline Bit Bit::operator &(const Bit& rhs) const{
	Bit res;
	res.bit = CircuitExecution::circ_exec->and_gate(bit, rhs.bit);
	return res;
}
inline Bit Bit::operator ^(const Bit& rhs) const{
	Bit res;
	res.bit = CircuitExecution::circ_exec->xor_gate(bit, rhs.bit);
	return res;
}
inline Bit Bit::operator ^=(const Bit& rhs) {
	this->bit = CircuitExecution::circ_exec->xor_gate(bit, rhs.bit);
	return (*this);
}

inline Bit Bit::operator |(const Bit& rhs) const{
	return (*this ^ rhs) ^ (*this & rhs);
}

inline Bit Bit::operator!() const {
	return CircuitExecution::circ_exec->not_gate(bit);
}

// add bit condition
inline void bit_condNeg(Bit cond, Bit & dest, const Bit src)
{
	// TODO: bug
	Bit c = cond;
	dest = src ^ cond;
	Bit t = dest ^ c;
	c = c & dest;
	dest = t;
	dest = cond ^ c^ src;
}

inline void bit_ifThenElse(Bit &dest, const Bit tsrc, const Bit fsrc, Bit cond)
{
	Bit x, a;
	x = tsrc ^ fsrc;
	a = cond & x;
	dest = a ^ fsrc;
}
