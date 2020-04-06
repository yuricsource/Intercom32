#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include "FastDelegate.h"

namespace Protocol
{

class HttpParser
{
public:
	enum class HttpHeaderStatus
	{
		http_header_status_done,
		http_header_status_continue,
		http_header_status_version_character,
		http_header_status_code_character,
		http_header_status_status_character,
		http_header_status_key_character,
		http_header_status_value_character,
		http_header_status_store_keyvalue,
		http_header_status_code_done,
		http_header_version_done
	};

	enum class HttpRoundtripperState
	{
		http_roundtripper_header,
		http_roundtripper_chunk_header,
		http_roundtripper_chunk_data,
		http_roundtripper_raw_data,
		http_roundtripper_unknown_data,
		http_roundtripper_close,
		http_roundtripper_error,
		http_roundtripper_error_scrach_exceeded,
		http_roundtripper_code_is_invalid,
		http_roundtripper_header_is_invalid,
		http_roundtripper_application_data_is_invalid,
		http_roundtripper_start_line_is_invalid
	};

private:
	static constexpr unsigned char http_header_state[] =
	{
		/*     *    \t    \n   \r    ' '     ,     :   PAD */
		0x80, 0x01, 0xC1, 0xC1, 0xA1, 0x80, 0x80, 0xC1, /* state 0: HTTP version */
		0x81, 0x02, 0xC1, 0xC1, 0x92, 0x01, 0x01, 0xC1, /* state 1: Response code */
		0x82, 0x82, 0x04, 0x03, 0x82, 0x82, 0x82, 0xC1, /* state 2: Response reason */
		0xC1, 0xC1, 0x04, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, /* state 3: HTTP version newline */
		0x84, 0xC1, 0xC0, 0x05, 0xC1, 0xC1, 0x06, 0xC1, /* state 4: Start of header field */
		0xC1, 0xC1, 0xC0, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, /* state 5: Last CR before end of header */
		0x87, 0x06, 0xC1, 0xC1, 0x06, 0x87, 0x87, 0xC1, /* state 6: leading whitespace before header value */
		0x87, 0x87, 0xC4, 0x10, 0x87, 0x88, 0x87, 0xC1, /* state 7: header field value */
		0x87, 0x88, 0x06, 0x09, 0x88, 0x88, 0x87, 0xC1, /* state 8: Split value field value */
		0xC1, 0xC1, 0x06, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, /* state 9: CR after split value field */
		0xC1, 0xC1, 0xC4, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, /* state 10:CR after header value */
	};

	static constexpr unsigned char http_chunk_state[] =
		{
			/*     *    LF    CR    HEX */
			0xC1, 0xC1, 0xC1, 0x01, /* s0: initial hex char */
			0xC1, 0xC1, 0x02, 0x81, /* s1: additional hex chars, followed by CR */
			0xC1, 0x83, 0xC1, 0xC1, /* s2: trailing LF */
			0xC1, 0xC1, 0x04, 0xC1, /* s3: CR after chunk block */
			0xC1, 0xC0, 0xC1, 0xC1, /* s4: LF after chunk block */
	};

public:
	static constexpr uint16_t ScratchSize = 256;

	struct HttpParserRoundtripper
	{
		void *opaque;
		char *scratch;
		int code;
		int contentlength;

		HttpRoundtripperState parsestate;
		HttpRoundtripperState state;

		int nscratch;
		int nkey;
		int nvalue;

		int chunked;

		bool endOfHeadersDetected;
	};

	typedef fastdelegate::FastDelegate3<HttpParserRoundtripper *, const char *, uint16_t, bool> AppendBodyDelegate;
	typedef fastdelegate::FastDelegate5<void *, const char *, uint16_t, const char *, uint16_t, bool> ProcessHeaderDelegate;
	typedef fastdelegate::FastDelegate2<void *, uint32_t, bool> ProcessCodeDelegate;
	typedef fastdelegate::FastDelegate2<void *, uint32_t> ProcessErrorDelegate;

	void HttpInit(struct HttpParserRoundtripper *rt, void *opaque)
	{
		rt->scratch = &_scratch[0];
		rt->nscratch = ScratchSize;
		rt->opaque = opaque;
		rt->code = 0;
		rt->contentlength = -1;

		rt->parsestate = HttpRoundtripperState::http_roundtripper_header;
		rt->state = HttpRoundtripperState::http_roundtripper_header;

		rt->nkey = 0;
		rt->nvalue = 0;
		rt->chunked = 0;
		rt->endOfHeadersDetected = false;
	}

	int HttpProcessData(struct HttpParserRoundtripper *rt, const char *data, int size, int *read);

	inline int IsAnyHttpError(struct HttpParserRoundtripper *rt)
	{
		return rt->state == HttpRoundtripperState::http_roundtripper_error ||
			   rt->state == HttpRoundtripperState::http_roundtripper_error_scrach_exceeded ||
			   rt->state == HttpRoundtripperState::http_roundtripper_code_is_invalid ||
			   rt->state == HttpRoundtripperState::http_roundtripper_application_data_is_invalid ||
			   rt->state == HttpRoundtripperState::http_roundtripper_start_line_is_invalid;
	}

	inline bool IsHttpStartLineError(struct HttpParserRoundtripper *rt)
	{
		return rt->state == HttpRoundtripperState::http_roundtripper_start_line_is_invalid;
	}

	inline bool HttpHeaderParsed(struct HttpParserRoundtripper *rt)
	{
		return rt->endOfHeadersDetected;
	}

	inline bool HttpIsChunked(struct HttpParserRoundtripper *rt)
	{
		return rt->chunked;
	}

	void SetAppendBody(AppendBodyDelegate delegate)
	{
		_onAppendBody = delegate;
	}

	void SetProcessHeader(ProcessHeaderDelegate delegate)
	{
		_onProcessHeader = delegate;
	}

	void SetProcessCode(ProcessCodeDelegate delegate)
	{
		_onProcessCode = delegate;
	}

	void SetProcessError(ProcessErrorDelegate delegate)
	{
		_onProcessError = delegate;
	}

private:
	char _scratch[ScratchSize];

	HttpHeaderStatus httpParseHeaderChar(HttpRoundtripperState &state, char ch);

	int httpParseChunked(HttpRoundtripperState &state, int &size, char ch);

	AppendBodyDelegate _onAppendBody;
	ProcessHeaderDelegate _onProcessHeader;
	ProcessCodeDelegate _onProcessCode;
	ProcessErrorDelegate _onProcessError;

	bool appendBody(HttpParserRoundtripper *rt, const char *data, int ndata)
	{
		if (_onAppendBody)
			return _onAppendBody(rt, data, ndata);
		else
			return true;
	}

	bool processHeader(void *opaque, const char *key, int nkey, const char *value, int nvalue)
	{
		if (_onProcessHeader)
			return _onProcessHeader(opaque, key, nkey, value, nvalue);
		else
			return true;
	}

	bool processCode(void *opqaue, int code)
	{
		if (_onProcessCode)
			return _onProcessCode(opqaue, (uint32_t)code);
		else
			return true;
	}

	void processError(void *opqaue, int errorno)
	{
		if (_onProcessError)
			_onProcessError(opqaue, (uint32_t)errorno);
	}

	bool growScratch(struct HttpParserRoundtripper *rt, int size);

	inline int min(int a, int b)
	{
		return a > b ? b : a;
	}
};

} // namespace Protocol