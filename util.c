#include "util.h"

s8 s8_trim_space(s8 s)
{
	while (s.len > 0 && ISSPACE(s.data[0])) { s.len--; s.data++; }
	while (s.len > 0 && ISSPACE(s.data[s.len - 1])) s.len--;
	return s;
}

void s8_split(s8 in, u8 c, s8 *lhs, s8 *rhs)
{
	ix i;
	s8 s = in;

	for (i = 0; i < s.len && s.data[i] != c; i++);
	s.len = i;

	if (lhs) *lhs = s;

	if (rhs) {
		i32 inc   = (s.len == in.len) ? 0 : 1;
		rhs->len  = in.len  - s.len - inc;
		rhs->data = in.data + s.len + inc;
	}
}

b32 s8_case_ignore_equal(s8 a, s8 b)
{
	b32 result = a.len == b.len;
	for (ix i = 0; result && i < a.len; i++)
		result &= (a.data[i] & ~0x20u) == (b.data[i] & ~0x20u);
	return result;
}

i64 s8_to_i64(s8 s)
{
	i64 result = 0, sign = 1;
	if (s.len && s.data[0] == '-') {
		sign = -1;
		s.data++;
		s.len--;
	}
	for (ix i = 0; i < s.len; i++) {
		if (!ISDIGIT(s.data[i]))
			break;
		result *= 10;
		result += s.data[i] - '0';
	}
	return sign * result;
}

u32 s8_hex_to_u32(s8 s)
{
	u32 res = 0;
	/* NOTE: clamp length to something valid for a u32 */
	s.len = MIN(s.len, 8);
	for (; s.len > 0; s.len--, s.data++) {
		res <<= 4;
		if (ISDIGIT(*s.data)) {
			res |= *s.data - '0';
		} else if (ISHEX(*s.data)) {
			/* NOTE: convert to lowercase first then convert to value */
			u8 byte = *s.data;
			byte |= 0x20u;
			res  |= byte - 0x57u;
		} else {
			/* NOTE: do nothing (treat invalid value as 0) */
		}
	}
	return res;
}
