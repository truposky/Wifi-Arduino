#define MAXDATASIZE 256
#define HEADER_LEN (sizeof(unsigned short)*3)
struct appdata{

        unsigned short id; //identificador
        unsigned short op; //codigo de operacion
        unsigned short len; //longitud de datos

        char data [MAXDATASIZE-HEADER_LEN];//datos


};
#define ID 0x00;
#define OP_SALUDO         0x00
#define OP_MOVE_FORWARD   0x01
#define OP_MOVE_BACKWARD  0x02
#define OP_MOVE_RIGHT     0x03
#define OP_MOVE_LEFT      0x04
