
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_TCPSocket_h
#define bolt_TCPSocket_h

/*
 * @file TCPSocket.h
 */


#ifdef WIN32
#include <winsock2.h>
#endif /* WIN32 */

#include <bolt/Options.h>


namespace bolt {


/*
 * @class TCPSocket
 * TCP client socket abstraction.
 */

class TCPSocket
{
public:

	TCPSocket();
	virtual ~TCPSocket();

	Result setOptions(const Options &o);

#ifdef WIN32
typedef SOCKET handle_t;
#else
typedef int handle_t;
#endif /* WIN32 */

	Result connect(const char *host, int port);
	Result disconnect();

	int doRead(char *buffer, int count, int timeout_ms);
	int write(const char *buffer, int count);

	int getError() const;
	std::string getErrorString() const;


private:

	Result open();
	Result release();
	int warning(const char *s);

	int setError(int localError = ERR_NONE, bool setString = true);
	int setError(int error, const string &text);
	void setErrorString(int code);
	Result setError(Result &result);
	int setError(const char *text, int code = 0);

	handle_t handle;
	bool connected;
	Mutex readMutex;
	int error;
	std::string errorString;
	std::string tag;

	i32 connect_timeout_ms;

	bool m_nagle;
	int  m_tcpBufferSize;
};

} // namespace bolt


#endif /* bolt_TCPSocket_h */

