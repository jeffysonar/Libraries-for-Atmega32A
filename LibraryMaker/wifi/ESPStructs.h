
// commands 
# define AT_TEST "AT"

# define MODE_QUERY "AT+CWMODE_DEF?"
# define MODE_STATION "AT+CWMODE_DEF=1"
# define MODE_AP "AT+CWMODE_DEF=2"
# define STATION_MODE 1
# define AP_MODE 2

# define AP_QUERY "AT+CWJAP_DEF?"
# define AP_ASSIGN "AT+CWJAP_DEF="

# define LIST_AP "AT+CWLAP"

# define CHECK_STATUS "AT+CIPSTATUS"
# define STATUS_IDLE '0'
# define STATUS_GOT_IP '2' 
# define STATUS_CONNECTED '3'
# define STATUS_DISCONNECTED '4'

# define START_TCP_CONN_CMD "AT+CIPSTART=\"TCP\","

# define SEND_DATA_CMD "AT+CIPSENDEX="

# define CLOSE_CONN_CMD "AT+CIPCLOSE"

# define PING_CMD "AT+PING="

// constant responses used for matching
# define OK_RESPONSE "OK\r\n"
# define NO_AP "No AP\r\n"
# define TIMEOUT "timeout\r\n"
# define CLOSED "CLOSED\r\n"

// structures used to easily access and process response

typedef struct Test {
	char nl1[2];
	char result[5];
	char nl2[2];
} test; 

typedef struct CWMODE_DEF_QUERY_RES {
	char label[12];
	char mode;
	char nl[2];
	test t_r;
} cwmode_q_t;

typedef struct CWMODE_DEF_ASSIGN_RES {
	test t_r;
} cwmode_a_t;

typedef struct CWJAP_DEF_QUERY_RES {
	char noAP[5];
	char nl[2];
	test t_r;
} cwjap_q_t;

typedef struct CWJAP_DEF_ASSIGN_RES {
	char wconip[29];
	test t_r;
} cwjap_a_t;

typedef struct CIPSTATUS_RES {
	char label[7];
	char status;
	char nl[2];
	test t_r;
} cipstatus_t;

typedef struct CIPSTART_SUCESS_RES {
	char conn[7];
	char nl[2];
	test t_r;
} cipstart_s_t;

typedef struct CIPSTART_FAIL_RES {
	test t_r;
	char close[6];
	char nl[2];
} cipstart_f_t;

typedef struct CIPCLOSE {
	char close[6];
	test t_r;
} cipclose_t;

typedef struct PING {
	char plus;
	char timeout[7];
	test t_r;
} ping_t;//_timeout_t;

typedef struct POST_SENDEX_RES {
		char busyTillNumberBytes[18];
		char rest[24];
} sendex_t;

// combine all in a union
typedef union WIFI_BUFFER {
	char buffer[512];
	char APlist[16][32];
	char dislist[32][16];
	test t_r;
	cwmode_q_t cwmode_q;
	cwmode_a_t cwmode_a;
	cwjap_q_t cwjap_q;
	cwjap_a_t cwjap_a;
	cipstatus_t cipstatus;
	cipstart_s_t cipstart_s;
	cipstart_f_t cipstart_f;
	cipclose_t cipclose;
	ping_t ping;
	sendex_t sendex;	
} WifiBuffer;