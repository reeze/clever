/**
 * Clever programming language
 * Copyright (c) 2011-2012 Clever Team
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
 */

#include <sstream>
#include "compiler/value.h"
#include "types/type.h"

namespace clever {

void Value::initialize() {
	clever_assert_not_null(getTypePtr());

	if (getTypePtr() == CLEVER_INT) {
		setInteger(0);
	}
	else if (getTypePtr() == CLEVER_DOUBLE) {
		setDouble(0.0);
	}
	else if (getTypePtr() == CLEVER_BOOL) {
		setBoolean(false);
	}
	else if (getTypePtr() == CLEVER_STR) {
		setString(CACHE_PTR(CLEVER_EMPTY_STR, ""));
	}
	else if (getTypePtr() == CLEVER_BYTE) {
		setByte(0);
	} else {
		/**
		 * @TODO: need to initialize non-primitive types without
		 * that method call
		 */
		TypeVector tv;

		const Method* ctor = getTypePtr()->getMethod(
			CACHE_PTR(CLEVER_CTOR, CLEVER_CTOR_NAME), &tv);

		if (ctor) {
			ctor->call(NULL, this, this);
		}
	}
}

bool Value::getValueAsBool() const {
	clever_assert_not_null(m_type_ptr);

	if (m_type_ptr == CLEVER_INT) {
		return getInteger();
	} else if (m_type_ptr == CLEVER_DOUBLE) {
		return getDouble();
	} else if (m_type_ptr == CLEVER_STR) {
		return !getString().empty();
	} else if (m_type_ptr == CLEVER_BOOL) {
		return getBoolean();
	} else if (m_type_ptr == CLEVER_BYTE) {
		return getByte();
	}

	return false;
}

void Value::copy(const Value* const value) {
	clever_assert_not_null(value);

	// Decrement the current internal value reference
	if (isUserValue() && getDataValue()) {
		getDataValue()->delRef();
	} else if (isPrimitive() && isString() && m_data.s_value
		&& !m_data.s_value->isInterned()) {
		const_cast<CString*>(m_data.s_value)->delRef();
	}

	// ValueData shallow-copy (in case of non-primitive)
	std::memcpy(&m_data, value->getData(), sizeof(ValueData));
	m_type_ptr = value->getTypePtr();
	m_type = value->getType();

	// Increment the new internal value reference
	if (isUserValue() && getDataValue()) {
		getDataValue()->addRef();
	} else if (isPrimitive() && isString() && m_data.s_value
		&& !m_data.s_value->isInterned()) {
		const_cast<CString*>(m_data.s_value)->addRef();
	}
}

void Value::deepCopy(const Value* const value) {
	clever_assert_not_null(value);

	std::memcpy(&m_data, value->getData(), sizeof(ValueData));
	m_type_ptr = value->getTypePtr();
	m_type = value->getType();

	if (isUserValue()) {
		m_data.dv_value = m_type_ptr->clone(value);
	}
}

const CString& Value::toString() {
	if (isPrimitive()) {
		std::ostringstream str;

		if (isInteger()) {
			str << getInteger();
		} else if (isDouble()) {
			str << getDouble();
		} else if (isBoolean()) {
			return *CSTRING(getBoolean() ? "true" : "false");
		} else if (isString()) {
			return getString();
		} else if (isByte()) {
			str << "0x" << std::hex << uint32_t(getByte());
		}

		return *CSTRING(str.str());
	} else if (getTypePtr() == CLEVER_VOID) {
		return *CSTRING("Void");
	} else {
		Value ret;
		CLEVER_INTERNAL_MCALL(this, "toString", NULL, NULL, &ret);
		const CString& str = ret.getString();
		return str;
	}
}

} // clever