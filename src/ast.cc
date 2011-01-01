/*
 * Clever language
 * Copyright (c) 2011 Clever Team
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

#include <sstream>
#include "ast.h"
#include "compiler.h"
#include "opcode.h"
#include "vm.h"
#include "irbuilder.h"

namespace clever { namespace ast {

/*
 * Clear the AST nodes
 */
void TreeNode::clear(void) throw() {
	TreeNode::nodeList::const_iterator it = nodes.begin(), end(nodes.end());

	/* Remove the AST nodes */
	while (it != end) {
		(*it)->delRef();
		++it;
	}
}

/*
 * LogicExpression
 */
LogicExpression::LogicExpression(int op, Expression* lhs, Expression* rhs)
	: m_op(op), m_lhs(lhs), m_rhs(rhs), m_value(NULL) {
	Value* tmp_lhs = lhs->get_value();
	Value* tmp_rhs = rhs->get_value();

	m_rhs->addRef();
	m_lhs->addRef();

	if (!Compiler::checkCompatibleTypes(tmp_lhs, tmp_rhs)) {
		Compiler::error("Type mismatch!");
	}

	/* Checking if we can perform constant folding optimization */
	if (tmp_lhs->isConst() && tmp_rhs->isConst()) {
		m_value = Compiler::constantFolding(m_op, tmp_lhs, tmp_rhs);
	}
	if (m_value) {
		/* No opcode must be generated */
		set_optimized(true);

		m_rhs->delRef();
		m_lhs->delRef();
		m_rhs = m_lhs = NULL;
	} else {
		m_result = new TempValue();
	}
}

/*
 * BinaryExpression
 */
BinaryExpression::BinaryExpression(char op_, Expression* lhs, Expression* rhs)
		: m_op(op_), m_lhs(lhs), m_rhs(rhs), m_result(NULL), m_value(NULL) {
	Value* tmp_lhs;
	Value* tmp_rhs = rhs->get_value();

	/* Check if it is an unary operation */
	if (m_lhs == NULL) {
		m_lhs = lhs = new ast::NumberLiteral(int64_t(0));
	}

	tmp_lhs = lhs->get_value();

	m_rhs->addRef();
	m_lhs->addRef();

	if (!Compiler::checkCompatibleTypes(tmp_lhs, tmp_rhs)) {
		Compiler::error("Type mismatch!");
	}

	/* Checking if we can perform constant folding optimization */
	if (tmp_lhs->isConst() && tmp_rhs->isConst()) {
		m_value = Compiler::constantFolding(m_op, tmp_lhs, tmp_rhs);
	}
	if (m_value) {
		/* No opcode must be generated */
		set_optimized(true);

		m_rhs->delRef();
		m_lhs->delRef();
		m_rhs = m_lhs = NULL;
	} else {
		m_result = new TempValue();
	}
}

}} // clever::ast