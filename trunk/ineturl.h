//=====================================================================
// 
// ineturl.h - urllib
//
// NOTE:
// for more information, please see the readme file
//
//=====================================================================
#ifndef __INETURL_H__
#define __INETURL_H__

#include "imemdata.h"
#include "inetbase.h"
#include "inetcode.h"


//=====================================================================
// IHTTPSOCK 
//=====================================================================

//---------------------------------------------------------------------
// IHTTPSOCK DEFINITION
//---------------------------------------------------------------------
#define IHTTPSOCK_STATE_CLOSED		0
#define IHTTPSOCK_STATE_CONNECTING	1
#define IHTTPSOCK_STATE_CONNECTED	2


struct IHTTPSOCK
{
	int state;
	int sock;
	char *buffer;
	int bufsize;
	int endless;
	int error;
	IINT64 blocksize;
	IINT64 received;
	IINT64 conntime;
	struct sockaddr remote;
	struct IMSTREAM sendmsg;
	struct IMSTREAM recvmsg;
};

typedef struct IHTTPSOCK IHTTPSOCK;

#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------
// IHTTPSOCK INTERFACE
//---------------------------------------------------------------------

// create a http sock
IHTTPSOCK *ihttpsock_new(struct IMEMNODE *nodes);

// delete a http sock
void ihttpsock_delete(IHTTPSOCK *httpsock);

// update http sock state
void ihttpsock_update(IHTTPSOCK *httpsock);


// connect to remote address
int ihttpsock_connect(IHTTPSOCK *httpsock, const struct sockaddr *remote);

// close connection
void ihttpsock_close(IHTTPSOCK *httpsock);

// assign to a connected socket
int ihttpsock_assign(IHTTPSOCK *httpsock, int sock);


// returns zero if blocked
// returns below zero if connection shutdown or error
// returns received data size if data received
long ihttpsock_recv(IHTTPSOCK *httpsock, void *data, long size);

// send data
long ihttpsock_send(IHTTPSOCK *httpsock, const void *data, long size);

// poll socket
int ihttpsock_poll(IHTTPSOCK *httpsock, int event, int millsec);

// get data size in send buffer (nbytes of data which hasn't been sent)
long ihttpsock_dsize(const IHTTPSOCK *httpsock);

// change buffer size
void ihttpsock_bufsize(IHTTPSOCK *httpsock, long bufsize);

// get socket
int ihttpsock_sock(const IHTTPSOCK *httpsock);


// ihttpsock_block_* functions will returns these values or data size
#define IHTTPSOCK_BLOCK_AGAIN	-1
#define IHTTPSOCK_BLOCK_DONE	-2
#define IHTTPSOCK_BLOCK_CLOSED	-3

// set block size
int ihttpsock_block_set(IHTTPSOCK *httpsock, IINT64 blocksize);

// returns equal or above zero for data size
// returns IHTTPSOCK_BLOCK_AGAIN for block
// returns IHTTPSOCK_BLOCK_DONE for job finished
// returns IHTTPSOCK_BLOCK_CLOSED for connection shutdown or error
long ihttpsock_block_recv(IHTTPSOCK *httpsock, void *data, long size);


// returns equal or above zero for data value, 
// returns IHTTPSOCK_BLOCK_AGAIN for block
// returns IHTTPSOCK_BLOCK_CLOSED for connection shutdown or error
int ihttpsock_block_getch(IHTTPSOCK *httpsock);

// returns IHTTPSOCK_BLOCK_AGAIN for block
// returns IHTTPSOCK_BLOCK_DONE for job finished
// returns IHTTPSOCK_BLOCK_CLOSED for connection shutdown or error
int ihttpsock_block_gets(IHTTPSOCK *httpsock, ivalue_t *text);


#ifdef __cplusplus
}
#endif


//=====================================================================
// IHTTPLIB
//=====================================================================

#define IHTTP_STATE_STOP			0
#define IHTTP_STATE_CONNECTING		1
#define IHTTP_STATE_CONNECTED		2

#define IHTTP_SENDING_STATE_WAIT	0
#define IHTTP_SENDING_STATE_HEADER	1
#define IHTTP_SENDING_STATE_DATA	2
#define IHTTP_RECVING_STATE_WAIT	0
#define IHTTP_RECVING_STATE_HEADER	1
#define IHTTP_RECVING_STATE_DATA	2

#define IHTTP_CHUNK_STATE_HEAD		0
#define IHTTP_CHUNK_STATE_DATA		1
#define IHTTP_CHUNK_STATE_TAIL		2
#define IHTTP_CHUNK_STATE_DONE		3

#define IHTTP_RESULT_DONE			0
#define IHTTP_RESULT_NOT_STARTED	1
#define IHTTP_RESULT_NOT_COMPLETED	2
#define IHTTP_RESULT_NOT_FIND		3
#define IHTTP_RESULT_HTTP_ERROR		4
#define IHTTP_RESULT_HTTP_UNSUPPORT	5
#define IHTTP_RESULT_HTTP_OUTRANGE  6
#define IHTTP_RESULT_ABORTED		7
#define IHTTP_RESULT_SOCK_ERROR		8
#define IHTTP_RESULT_INVALID_ADDR	9
#define IHTTP_RESULT_CONNECT_FAIL	10
#define IHTTP_RESULT_DISCONNECTED	11



//---------------------------------------------------------------------
// URL Descriptor
//---------------------------------------------------------------------
struct IHTTPLIB
{
	int state;
	int result;
	int snext;
	int rnext;
	int cnext;
	int shutdown;
	int chunked;
	int httpver;
	int nosize;
	int keepalive;
	int partial;
	int isredirect;
	IINT64 clength;
	IINT64 chunksize;
	IINT64 datasize;
	IINT64 range_start;
	IINT64 range_endup;
	IINT64 range_size;
	IHTTPSOCK *sock;
	ivalue_t host;
	ivalue_t line;
	ivalue_t ctype;
	ivalue_t sheader;
	ivalue_t rheader;
	ivalue_t redirect;
	ivalue_t buffer;
};

typedef struct IHTTPLIB IHTTPLIB;



#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------
// URL Interface
//---------------------------------------------------------------------
IHTTPLIB *ihttplib_new(void);

void ihttplib_delete(IHTTPLIB *http);

int ihttplib_open(IHTTPLIB *http, const char *HOST);

int ihttplib_close(IHTTPLIB *http);

int ihttplib_update(IHTTPLIB *http, int wait);

void ihttplib_header_reset(IHTTPLIB *http);

void ihttplib_header_write(IHTTPLIB *http, const char *head);

void ihttplib_header_send(IHTTPLIB *http);

long ihttplib_send(IHTTPLIB *http, const void *data, long size);


#define IHTTP_RECV_AGAIN	-1
#define IHTTP_RECV_DONE		-2
#define IHTTP_RECV_CLOSED	-3
#define IHTTP_RECV_NOTFIND	-4
#define IHTTP_RECV_ERROR	-5

// returns IHTTP_RECV_AGAIN for block
// returns IHTTP_RECV_DONE for okay
// returns IHTTP_RECV_CLOSED for closed
// returns IHTTP_RECV_NOTFIND for not find
// returns IHTTP_RECV_ERROR for http error
long ihttplib_recv(IHTTPLIB *http, void *data, long size);

// returns data size in send buffer
long ihttplib_dsize(IHTTPLIB *http);


#define IHTTP_METHOD_GET	0
#define IHTTP_METHOD_POST	1

int ihttplib_request(IHTTPLIB *http, int method, const char *url, 
	const void *body, long bodysize, const char *header);


// returns IHTTP_RECV_AGAIN for block
// returns IHTTP_RECV_DONE for okay
// returns IHTTP_RECV_CLOSED for closed
// returns IHTTP_RECV_NOTFIND for not find
// returns IHTTP_RECV_ERROR for http error
int ihttplib_getresponse(IHTTPLIB *http, ivalue_t *content, int waitms);


#ifdef __cplusplus
}
#endif



//=====================================================================
// IURLLIB
//=====================================================================
struct IURLD
{
	IHTTPLIB *http;
	int done;
	ivalue_t url;
	ivalue_t host;
	ivalue_t proxy;
};

typedef struct IURLD IURLD;


#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------
// URL Interface
//---------------------------------------------------------------------

// open a url
// POST mode: size >= 0 && data != NULL 
// GET mode: size < 0 || data == NULL
// proxy format: 218.107.xx.xx:80 (http proxy only)
IURLD *ineturl_open(const char *URL, const void *data, long size, 
	const char *header, const char *proxy);

void ineturl_close(IURLD *url);

// returns IHTTP_RECV_AGAIN for block
// returns IHTTP_RECV_DONE for okay
// returns IHTTP_RECV_CLOSED for closed
// returns IHTTP_RECV_NOTFIND for not find
// returns IHTTP_RECV_ERROR for http error
// returns > 0 for received data size
long ineturl_read(IURLD *url, void *data, long size, int waitms);

// writing extra post data
// returns data size in send-buffer;
long ineturl_write(IURLD *url, const void *data, long size);

// flush: try to send data from buffer to network
void ineturl_flush(IURLD *url);


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------------
// TOOL AND DEMO
//---------------------------------------------------------------------

// download to a file
int _demo_download(const char *URL, const char *filename);

// wget into a string
// returns IHTTP_RECV_DONE for okay
// returns IHTTP_RECV_CLOSED for closed
// returns IHTTP_RECV_NOTFIND for not find
// returns IHTTP_RECV_ERROR for http error
int _demo_wget(const char *URL, ivalue_t *string);


#ifdef __cplusplus
}
#endif



#endif


