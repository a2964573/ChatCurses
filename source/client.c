#include "main.h"


int clientConnect(CLIENT* client, int isNonBlock)
{
	if(client == NULL) {
		return -1;
	}

	strcpy(client->server_ip, "127.0.0.1");
	client->server_port = 30000;

	client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    client->flags = fcntl(client->sockfd, F_GETFL, 0);

	client->isNonBlock = isNonBlock;

	if(isNonBlock == TRUE) {
		client->flags |= O_NONBLOCK;
		UTILLOG(LOGLV_DBG, __FUNCTION__, "Set NonBlock");
	}
	else {
		client->flags &= ~O_NONBLOCK;
		UTILLOG(LOGLV_DBG, __FUNCTION__, "Set Block");
	}
	fcntl(client->sockfd, F_SETFL, client->flags);
	UTILLOG(LOGLV_DBG, __FUNCTION__, "client->flags[0x%04x][0x%04x:0x%04x][%d]"
		, fcntl(client->sockfd, F_GETFL, 0), client->flags, O_NONBLOCK, client->flags & O_NONBLOCK);

    client->address.sin_family = AF_INET;
    client->address.sin_port   = htons(client->server_port);

	int rtn = inet_pton(AF_INET, client->server_ip, &client->address.sin_addr);
    if(rtn <= 0) {
        return rtn;
    }

	UTILLOG(LOGLV_DBG, __FUNCTION__, "%s:%d Try Connected..", client->server_ip, client->server_port);

	int result = connect(client->sockfd, (struct sockaddr*)&client->address, sizeof(client->address));
    if(result < 0) {
		if(errno == EINPROGRESS) {
			int so_error;
			socklen_t len = sizeof(so_error);
			if(getsockopt(client->sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len) == 0 && so_error == 0) {
				UTILLOG(LOGLV_DBG, __FUNCTION__, "Connect Success.[0x%04x]", client->flags);
			}
			else {
				UTILLOG(LOGLV_ERR, __FUNCTION__, "Connect Failed.[%d]", result);
			}
		}
		else {
			UTILLOG(LOGLV_ERR, __FUNCTION__, "Connect Failed.[%d]", result);
			return -1;
		}
    }
	else {
		UTILLOG(LOGLV_DBG, __FUNCTION__, "Connect Success.[0x%04x]", client->flags);
	}


    return 0;
}

int clientSocketSend(CLIENT* client)
{
	if(client == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client is NULL");
		return -1;
	}
/*
	if(client->isNonBlock == TRUE) {
		UTILLOG(LOGLV_DBG, __FUNCTION__, "[SEND]Set NonBlock");
		client->flags = fcntl(client->sockfd, F_GETFL, 0);
		client->flags |= O_NONBLOCK;
		fcntl(client->sockfd, F_SETFL, client->flags);
	}
*/
//	UTILLOG(LOGLV_DBG, __FUNCTION__, "Try Sending..[Flag:0x%04x][0x%04x][NB:0x%04x]"
//		, fcntl(client->sockfd, F_GETFL, 0), client->flags, O_NONBLOCK);
//	UTILLOG(LOGLV_DBG, __FUNCTION__, "send[%d:%.*s]", client->send_len, client->send_len, client->send_buffer);

    int result = send(client->sockfd, client->send_buffer, client->send_len, 0);
	if(result < 0) {
		if(fcntl(client->sockfd, F_GETFL, 0) & O_NONBLOCK) {
			if (errno == EINPROGRESS) {
				return 0;
			}
			else {
				return -1;
			}
		}

		return -1;		
	}
	UTILLOG(LOGLV_DBG, __FUNCTION__, "send Success.");


	return result;
}

int clientSocketReceive(CLIENT* client)
{
	if(client == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client is NULL");
		return -1;
	}
/*
//	UTILLOG(LOGLV_DBG, __FUNCTION__, "[RECV]client isNonBlock[%d]", client->isNonBlock);
	if(client->isNonBlock == TRUE) {
//		UTILLOG(LOGLV_DBG, __FUNCTION__, "[RECV]Set NonBlock");
		client->flags = fcntl(client->sockfd, F_GETFL, 0);
		client->flags |= O_NONBLOCK;
		fcntl(client->sockfd, F_SETFL, client->flags);
	}
*/
//	UTILLOG(LOGLV_DBG, __FUNCTION__, "Try Receive..[Flag:0x%04x][0x%04x][NB:0x%04x]"
//		, fcntl(client->sockfd, F_GETFL, 0), client->flags, O_NONBLOCK);

    int result = recv(client->sockfd, client->recv_buffer, SZ_RECV_BUFFER_MAX, 0);
	if(result < 0) {
		if(fcntl(client->sockfd, F_GETFL, 0) & O_NONBLOCK) {
//			UTILLOG(LOGLV_DBG, __FUNCTION__, "result [%d][%d]", fcntl(client->sockfd, F_GETFL, 0) & O_NONBLOCK, result);
//			UTILLOG(LOGLV_DBG, __FUNCTION__, "errno  [%d]", errno);

			if(errno == EAGAIN || errno == EWOULDBLOCK) {
				return 0;
			}
			else {
				return -1;
			}
		}

		return -1;		
	}
	client->recv_len = result;

	UTILLOG(LOGLV_DBG, __FUNCTION__, "recv[%d:%.*s]", client->recv_len, client->recv_len, client->recv_buffer);
	UTILLOG(LOGLV_DBG, __FUNCTION__, "recv Success.");


    return client->recv_len;
}

int clientClose(CLIENT* client)
{
	if(client == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client is NULL");
		return -1;
	}

	UTILLOG(LOGLV_DBG, __FUNCTION__, "close.. [%d]", client->sockfd);
	if(client->sockfd >= 0) {
		if(close(client->sockfd) < 0) {
			UTILLOG(LOGLV_ERR, __FUNCTION__, "close failed");
		}

		client->sockfd = -1;
	}

	return 0;
}

int clientSocketSendClear(CLIENT* client)
{
	if(client == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client is NULL");
		return -1;
	}

	memset(client->send_buffer, 0x00, SZ_SEND_BUFFER_MAX);
	client->send_len = 0;


	return 0;
}

int clientSocketRecvClear(CLIENT* client)
{
	if(client == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client is NULL");
		return -1;
	}

	memset(client->recv_buffer, 0x00, SZ_RECV_BUFFER_MAX);
	client->recv_len = 0;


	return 0;
}

int clientMakeHeader(CLIENT* client, char* svc_cd)
{
	if(client == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client is NULL");
		return -1;
	}

	UTILLOG(LOGLV_DBG, __FUNCTION__, "[%s]Make Header..", svc_cd);

	HEADER header;
	memset(&header, 0x20, SZ_HEADER);

	char date[32] = {0,};
	char time[64] = {0,};

	if(utilGetDate(date) < 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "utilGetDate Failed.");
		return -1;
	}

	if(utilGetTime(time) < 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "utilGetTime Failed.");
		return -1;
	}

	memcpy(header.length, "00000", HEADER_LENGTH_LEN    );
	memcpy(header.ndate , date   , sizeof(header.ndate ));
	memcpy(header.ntime , time   , sizeof(header.ntime ));
	memcpy(header.svc_cd, svc_cd , sizeof(header.svc_cd));

	client->send_len = sprintf(client->send_buffer, "%.*s", SZ_HEADER, (char*)&header);

	UTILLOG(LOGLV_DBG, __FUNCTION__, "[%s]Make Header Success.[%d:%.*s]"
		, svc_cd, client->send_len, client->send_len, client->send_buffer);


	return client->send_len;
}

int clientMakePacket(CLIENT* client, char* inbound, int size)
{
	if(client == NULL || inbound == NULL || size <= 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "client/inbound/size is NULL/0");
		return -1;
	}

	UTILLOG(LOGLV_DBG, __FUNCTION__, "[%.*s]Make Packet.."
		, HEADER_SVCCD_LEN, &client->send_buffer[HEADER_SVCCD_POS]);

	memcpy(&client->send_buffer[SZ_HEADER], inbound, size);
	client->send_len = SZ_HEADER + size;

	char length[8] = {0,};
	sprintf(length, "%05d", client->send_len);
	memcpy(&client->send_buffer[0], length, HEADER_LENGTH_LEN);

	UTILLOG(LOGLV_DBG, __FUNCTION__, "[%.*s]Make Packet Success.[%d:%.*s]"
		, HEADER_SVCCD_LEN, &client->send_buffer[HEADER_SVCCD_POS]
		, client->send_len, client->send_len, client->send_buffer);


	return client->send_len;
}

