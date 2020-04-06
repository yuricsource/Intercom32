#include "HttpParser.h"
#include <cstdint>
#include <array>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <cctype>
#include <cstring>

namespace Protocol
{

constexpr unsigned char HttpParser::http_header_state[];
constexpr unsigned char HttpParser::http_chunk_state[];

HttpParser::HttpHeaderStatus HttpParser::httpParseHeaderChar(HttpRoundtripperState &state, char ch)
{
	int code = 0;
	switch (ch)
	{
	case '\t':
		code = 1;
		break;
	case '\n':
		code = 2;
		break;
	case '\r':
		code = 3;
		break;
	case ' ':
		code = 4;
		break;
	case ',':
		code = 5;
		break;
	case ':':
		code = 6;
		break;
	}

	int nextState = http_header_state[static_cast<uint8_t>(state) * 8 + code];

	state = static_cast<HttpRoundtripperState>(nextState & 0xF);

	switch (nextState)
	{
	case 0xC0:
		return HttpHeaderStatus::http_header_status_done;
	case 0xC1:
		return HttpHeaderStatus::http_header_status_done;
	case 0xC4:
		return HttpHeaderStatus::http_header_status_store_keyvalue;
	case 0x80:
		return HttpHeaderStatus::http_header_status_version_character;
	case 0x81:
		return HttpHeaderStatus::http_header_status_code_character;
	case 0x82:
		return HttpHeaderStatus::http_header_status_status_character;
	case 0x84:
		return HttpHeaderStatus::http_header_status_key_character;
	case 0x87:
		return HttpHeaderStatus::http_header_status_value_character;
	case 0x88:
		return HttpHeaderStatus::http_header_status_value_character;
	case 0x92:
		return HttpHeaderStatus::http_header_status_code_done;
	case 0xA1:
		return HttpHeaderStatus::http_header_version_done;
	}

	return HttpHeaderStatus::http_header_status_continue;
}

int HttpParser::httpParseChunked(HttpRoundtripperState &state, int &size, char ch)
{
	int code = 0;
	switch (ch)
	{
	case '\n':
		code = 1;
		break;
	case '\r':
		code = 2;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		code = 3;
		break;
	}

	int nextState = http_chunk_state[static_cast<uint8_t>(state) * 4 + code];

	state = static_cast<HttpRoundtripperState>(nextState & 0xF);

	switch (nextState)
	{
	case 0xC0:
		return size != 0;

	case 0xC1: /* error */
		size = -1;
		return 0;

	case 0x01: /* initial char */
		size = 0;
		/* fallthrough */
	case 0x81: /* size char */
		if (ch >= 'a')
			size = size * 16 + (ch - 'a' + 10);
		else if (ch >= 'A')
			size = size * 16 + (ch - 'A' + 10);
		else
			size = size * 16 + (ch - '0');
		break;

	case 0x83:
		return size == 0;
	}

	return 1;
}

bool HttpParser::growScratch(struct HttpParserRoundtripper *rt, int size)
{
	if (size > rt->nscratch)
		return false;

	return true;
}

int HttpParser::HttpProcessData(struct HttpParserRoundtripper *rt, const char *data, int size, int *read)
{
	const int initial_size = size;
	while (size)
	{
		switch (rt->state)
		{
		case HttpRoundtripperState::http_roundtripper_header:
		{
			switch (httpParseHeaderChar(rt->parsestate, *data))
			{
			case HttpHeaderStatus::http_header_status_continue:
			{
			}
			break;

			case HttpHeaderStatus::http_header_status_status_character:
			{
			}
			break;

			case HttpHeaderStatus::http_header_status_version_character:
			{
				if (growScratch(rt, rt->nkey + 1) == false)
				{
					rt->state = HttpRoundtripperState::http_roundtripper_start_line_is_invalid;
					return false;
				}

				rt->scratch[rt->nkey] = *data;
				++rt->nkey;
			}
			break;

			case HttpHeaderStatus::http_header_version_done:
			{
				if (rt->nkey <= 4 || rt->scratch[0] != 'H' || rt->scratch[1] != 'T' || rt->scratch[2] != 'T' || rt->scratch[3] != 'P')
				{
					rt->state = HttpRoundtripperState::http_roundtripper_start_line_is_invalid;
					return false;
				}
				rt->nkey = 0;
				rt->nvalue = 0;
			}
			break;

			case HttpHeaderStatus::http_header_status_code_done:
			{
				if (processCode(rt->opaque, rt->code) == false)
				{
					rt->state = HttpRoundtripperState::http_roundtripper_code_is_invalid;
					return false;
				}
			}
			break;

			case HttpHeaderStatus::http_header_status_done:
			{
				if (rt->parsestate != HttpRoundtripperState::http_roundtripper_header)
					rt->state = HttpRoundtripperState::http_roundtripper_error;
				else if (rt->chunked)
				{
					rt->contentlength = 0;
					rt->state = HttpRoundtripperState::http_roundtripper_chunk_header;
				}
				else if (rt->contentlength == 0)
					rt->state = HttpRoundtripperState::http_roundtripper_close;
				else if (rt->contentlength > 0)
					rt->state = HttpRoundtripperState::http_roundtripper_raw_data;
				else if (rt->contentlength == -1)
					rt->state = HttpRoundtripperState::http_roundtripper_unknown_data;
				else
					rt->state = HttpRoundtripperState::http_roundtripper_error;
			}
			break;

			case HttpHeaderStatus::http_header_status_code_character:
				rt->code = rt->code * 10 + *data - '0';
				break;

			case HttpHeaderStatus::http_header_status_key_character:
				if (growScratch(rt, rt->nkey + 1) == false)
				{
					rt->state = HttpRoundtripperState::http_roundtripper_error_scrach_exceeded;
					break;
				}
				rt->scratch[rt->nkey] = tolower(*data);
				++rt->nkey;
				break;

			case HttpHeaderStatus::http_header_status_value_character:
				if (growScratch(rt, rt->nkey + rt->nvalue + 1) == false)
				{
					rt->state = HttpRoundtripperState::http_roundtripper_error_scrach_exceeded;
					break;
				}
				rt->scratch[rt->nkey + rt->nvalue] = *data;
				++rt->nvalue;
				break;

			case HttpHeaderStatus::http_header_status_store_keyvalue:
			{
				if (rt->nkey == 17 && strncmp(rt->scratch, "transfer-encoding", rt->nkey) == 0)
				{
					rt->chunked = (rt->nvalue == 7 && strncmp(rt->scratch + rt->nkey, "chunked", rt->nvalue) == 0);
				}
				else if (rt->nkey == 14 && strncmp(rt->scratch, "content-length", rt->nkey) == 0)
				{
					int ii, end;
					rt->contentlength = 0;
					for (ii = rt->nkey, end = rt->nkey + rt->nvalue; ii != end; ++ii)
					{
						rt->contentlength = rt->contentlength * 10 + rt->scratch[ii] - '0';
					}
				}

				if (processHeader(rt->opaque, rt->scratch, rt->nkey, rt->scratch + rt->nkey, rt->nvalue) == false)
				{
					rt->state = HttpRoundtripperState::http_roundtripper_header_is_invalid;
					return false;
				}

				rt->nkey = 0;
				rt->nvalue = 0;
			}
			break;

			} // switch(httpParseHeaderChar(...))
			if (rt->parsestate == HttpRoundtripperState::http_roundtripper_close)
			{
				rt->endOfHeadersDetected = true;
			}
			--size;
			++data;
		}
		break;

		case HttpRoundtripperState::http_roundtripper_chunk_header:
		{
			if (!httpParseChunked(rt->parsestate, rt->contentlength, *data))
			{
				if (rt->contentlength == -1)
					rt->state = HttpRoundtripperState::http_roundtripper_error;
				else if (rt->contentlength == 0)
					rt->state = HttpRoundtripperState::http_roundtripper_close;
				else
					rt->state = HttpRoundtripperState::http_roundtripper_chunk_data;
			}
			--size;
			++data;
		}
		break;

		case HttpRoundtripperState::http_roundtripper_chunk_data:
		{
			if (rt->contentlength < 0)
				rt->contentlength = 0;
			const int chunksize = min(size, rt->contentlength);
			if (appendBody(rt, data, chunksize) == false)
			{
				rt->state = HttpRoundtripperState::http_roundtripper_application_data_is_invalid;
				break;
			}
			rt->contentlength -= chunksize;
			size -= chunksize;
			data += chunksize;
			if (rt->contentlength == 0)
			{
				rt->contentlength = 1;
				rt->state = HttpRoundtripperState::http_roundtripper_chunk_header;
			}
		}
		break;

		case HttpRoundtripperState::http_roundtripper_raw_data:
		{
			if (rt->contentlength < 0)
				rt->contentlength = 0;
			const int chunksize = min(size, rt->contentlength);
			if (appendBody(rt, data, chunksize) == false)
			{
				rt->state = HttpRoundtripperState::http_roundtripper_application_data_is_invalid;
				break;
			}
			rt->contentlength -= chunksize;
			size -= chunksize;
			data += chunksize;
			if (rt->contentlength == 0)
			{
				rt->state = HttpRoundtripperState::http_roundtripper_close;
			}
		}
		break;

		case HttpRoundtripperState::http_roundtripper_unknown_data:
		{
			if (size == 0)
			{
				rt->state = HttpRoundtripperState::http_roundtripper_close;
			}
			else
			{
				if (appendBody(rt, data, size) == false)
				{
					rt->state = HttpRoundtripperState::http_roundtripper_application_data_is_invalid;
					break;
				}
				size -= size;
				data += size;
			}
		}
		break;

		case HttpRoundtripperState::http_roundtripper_close:
		{
			// This is end of the headers.
		}
		break;

		case HttpRoundtripperState::http_roundtripper_error:
		case HttpRoundtripperState::http_roundtripper_error_scrach_exceeded:
		case HttpRoundtripperState::http_roundtripper_header_is_invalid:
		case HttpRoundtripperState::http_roundtripper_code_is_invalid:
		case HttpRoundtripperState::http_roundtripper_application_data_is_invalid:
		case HttpRoundtripperState::http_roundtripper_start_line_is_invalid:
		{
			// Error condition.
		}
		break;

		} // switch (rt->state)

		if (rt->state == HttpRoundtripperState::http_roundtripper_error ||
			rt->state == HttpRoundtripperState::http_roundtripper_close ||
			rt->state == HttpRoundtripperState::http_roundtripper_error_scrach_exceeded ||
			rt->state == HttpRoundtripperState::http_roundtripper_header_is_invalid ||
			rt->state == HttpRoundtripperState::http_roundtripper_code_is_invalid ||
			rt->state == HttpRoundtripperState::http_roundtripper_application_data_is_invalid)
		{
			*read = initial_size - size;

			int errorno = 0;
			if (rt->state == HttpRoundtripperState::http_roundtripper_error)
				errorno = 1;
			else if (rt->state == HttpRoundtripperState::http_roundtripper_error_scrach_exceeded)
				errorno = 2;
			else if (rt->state == HttpRoundtripperState::http_roundtripper_code_is_invalid)
				errorno = 3;
			else if (rt->state == HttpRoundtripperState::http_roundtripper_header_is_invalid)
				errorno = 4;
			else if (rt->state == HttpRoundtripperState::http_roundtripper_application_data_is_invalid)
				errorno = 5;
			if (errorno > 0)
			{
				processError(rt->opaque, errorno);
			}
			return 0;
		}

	} // while (size)
	*read = initial_size - size;
	return 1;
}

} // namespace Protocol