/*
 * Clever language
 * Copyright (c) 2010 Clever Team
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * $Id$
 */

#include <string>
#include "opcodes.h"

namespace clever {

/*
 * Dumps an opcode
 */
void Opcode::dump() {
	printf("%-15s | ", getOpName(m_op_type));
	printf("%-15s | ", dumpOp("op1", m_op1));
	printf("%-15s | ", dumpOp("op2", m_op2));
	printf("%-15s\n", dumpOp("result", m_result));
}

/*
 * Returns the opcode name
 */
const char* Opcode::getOpName(Opcodes op) {
#define CASE(x) case x: return #x

	switch (op) {
		CASE(OP_ECHO);
		CASE(OP_PLUS);
		CASE(OP_DIV);
		CASE(OP_MULT);
		CASE(OP_MINUS);
		CASE(OP_NEW_SCOPE);
		CASE(OP_END_SCOPE);
		CASE(OP_VAR_DECL);
		CASE(OP_BW_OR);
		CASE(OP_BW_AND);
		CASE(OP_BW_XOR);
		CASE(OP_PRE_INC);
		CASE(OP_PRE_DEC);
		CASE(OP_POS_INC);
		CASE(OP_POS_DEC);
		CASE(OP_JMPZ);
		CASE(OP_JMP);
		default:
			return "UNKNOWN";
	}
}

const char* Opcode::dumpOp(const char* label, Value* op) {
	if (op) {
		return std::string(std::string(label) + ": " + op->toString()).c_str();
	} else {
		return std::string(std::string(label) + ": UNUSED").c_str();
	}
}

} // clever