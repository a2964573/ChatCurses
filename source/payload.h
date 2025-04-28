#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

#define HEADER_LENGTH_POS 0
#define HEADER_LENGTH_LEN 5
#define HEADER_SVCCD_POS  25
#define HEADER_SVCCD_LEN  9
typedef struct {
	char length [HEADER_LENGTH_LEN];
	char ndate  [8  ];
	char ntime  [12 ];
	char svc_cd [HEADER_SVCCD_LEN ];
	char filler [30 ];
} HEADER;
#define SZ_HEADER 64

#define SERVICE_CODE_LOGIN0001 "LOGIN0001"
typedef struct {
	char id     [32 ];
	char pw     [64 ];
	char filler [32 ];
} LOGIN0001_IN;
#define SZ_LOGIN0001_IN sizeof(LOGIN0001_IN)

typedef struct {
	char result [2  ];
	char id     [32 ];
	char name   [64 ];
	char filler [30 ];
} LOGIN0001_OUT;
#define SZ_LOGIN0001_OUT sizeof(LOGIN0001_OUT)

#define SERVICE_CODE_MCHAT0001 "MCHAT0001"
#define SZ_MCHAT0001_MESSAGE_MAX 1024
typedef struct {
	char id     [32 ];
	char name   [64 ];
	char message[SZ_MCHAT0001_MESSAGE_MAX];
	char filler [32 ];
} MCHAT0001_IN;
#define SZ_MCHAT0001_IN sizeof(MCHAT0001_IN)

typedef struct {
	char result [2  ];
	char message[SZ_MCHAT0001_MESSAGE_MAX];
	char filler [30 ];
} MCHAT0001_OUT;
#define SZ_MCHAT0001_OUT sizeof(MCHAT0001_OUT)

#define SERVICE_CODE_MCHAT0002 "MCHAT0002"
#define SZ_MCHAT0002_MESSAGE_MAX 1024
typedef struct {
	char id     [32 ];
	char name   [64 ];
	char message[SZ_MCHAT0002_MESSAGE_MAX];
	char filler [32 ];
} MCHAT0002_IN;
#define SZ_MCHAT0002_IN sizeof(MCHAT0002_IN)

typedef struct {
	char result [2  ];
	char message[SZ_MCHAT0002_MESSAGE_MAX];
	char filler [30 ];
} MCHAT0002_OUT;
#define SZ_MCHAT0002_OUT sizeof(MCHAT0002_OUT)

#endif // _PAYLOAD_H_

