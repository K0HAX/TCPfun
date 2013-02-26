typedef struct _socket_data socket_data;

socket_data *SocketDataNew(int sock, char ip[256], unsigned int p);
int getSocketDataSocket(socket_data *sData);
int getSocketDataIP(socket_data *sData, char buf[]);
int getSocketDataPort(socket_data *sData);
