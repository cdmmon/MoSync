/*
 * benchdb.c
 *
 *  Created on: 3 nov 2011
 *      Author: Alex
 */

#include <MAUtil/Moblet.h>
#include <mautil/connection.h>
#include <mastdlib.h>
#include <conprint.h>
#define CONNECTION_BUFFER_SIZE 1024

class ConnectionMoblet : public MAUtil::Moblet, private MAUtil::HttpConnectionListener
{
public:
    ConnectionMoblet(char * completeUrl);

    void httpFinished(MAUtil::HttpConnection *conn, int result);
    void connRecvFinished(MAUtil::Connection *conn, int result);
    void connReadFinished(MAUtil::Connection *conn, int result);
    void initiateConnection(const char* url);
private:
    char mBuffer[CONNECTION_BUFFER_SIZE];
    MAUtil::HttpConnection mHttp;
    bool mIsConnected;
};
ConnectionMoblet::ConnectionMoblet(char * completeUrl) : mHttp(this) //constructor, taking the complete url of the publish_script as an arg
, mIsConnected(false)
{
	printf("Publishing result via url:");
	printf(completeUrl);
	initiateConnection(completeUrl); //connect to the url, publishing the results via HTTP GET
}
// connect to the given url if not other connection is active
void ConnectionMoblet::initiateConnection(const char* url) {
    if(mIsConnected) {
        printf("already connected\n..");
        return;
    }
    printf("\nconnecting to %s", url);

    int res = mHttp.create(url, HTTP_GET); //we publish using HTTP GET
    if(res < 0) {
        printf("unable to connect - %i\n", res);
    } else {
        mHttp.finish();
        mIsConnected = true;
    }
}
void ConnectionMoblet::httpFinished(MAUtil::HttpConnection* http, int result) {
    printf("HTTP %i\n", result);

    MAUtil::String contentLengthStr;
    int responseBytes = mHttp.getResponseHeader("content-length", &contentLengthStr);
    int contentLength = 0;
    if(responseBytes == CONNERR_NOHEADER)
    printf("no content-length response header\n");
    else {
        printf("content-length : %s\n", contentLengthStr.c_str());
        contentLength = atoi(contentLengthStr.c_str());
    }
    if(contentLength >= CONNECTION_BUFFER_SIZE || contentLength == 0) {
        printf("Receive in chunks..\n");
        mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
    } else {
        mBuffer[contentLength] = 0;
        mHttp.read(mBuffer, contentLength);
    }

}
void ConnectionMoblet::connReadFinished(MAUtil::Connection* conn, int result) {
    if(result >= 0)
    printf("connReadFinished %i\n", result);
    else
    printf("connection error %i\n", result);
    mHttp.close();

    mIsConnected = false;
}
void ConnectionMoblet::connRecvFinished(MAUtil::Connection * conn, int result){
    if(result >= 0) {
        printf("connRecvFinished %i\n", result);
        mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
        return;
    }
    else if(result == CONNERR_CLOSED) {
        printf("Receive finished!\n");
    } else {
        printf("connection error %i\n", result);
    }
    mHttp.close();
    mIsConnected = false;
}

extern "C" int publish_linpack_result(char * url, char * revision, char * runtime, char * git_hash, char * phone, char * native_sdk_ver, float mflops) {

	InitConsole();
	gConsoleLogging = 1;

	//need to use sprintf to build the url and pass it to the constructor, then we will be all set!
	char complete_url[200]; //build and store our url here
	/* building the url using sprintf */
	sprintf(complete_url, "%s%s%s%s%s%s%s%s%s%s%s%s%f", url, "?benchmark=linpack&revision=", revision, "&runtime=", runtime, "&git_hash=", git_hash, "&phone=", phone, "&native_sdk_ver=", native_sdk_ver, "&mflops=", mflops);

	ConnectionMoblet * cm = new ConnectionMoblet(complete_url); //pass the url to the ConnectionMoblet
	MAUtil::Moblet::run(cm); //run it
	delete cm;
	return 0;

}

