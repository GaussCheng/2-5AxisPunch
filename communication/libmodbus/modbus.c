/*
 * Copyright © 2001-2008 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
   The library is designed to send and receive data from a device that
   communicate via the Modbus protocol.

   The function names used are inspired by the Modicon Modbus Protocol
   Reference Guide which can be obtained from Schneider at
   www.schneiderautomation.com.

   Documentation:
   http://www.easysw.com/~mike/serial/serial.html
   http://copyleft.free.fr/wordpress/index.php/libmodbus/
*/

#include "modbus.h"

#ifndef NATIVE_WIN32
#include <termios.h>
#endif /* NATIVE_WIN32 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
//#include <QDebug>

/* TCP */
#ifndef NATIVE_WIN32
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#endif /* NATIVE_WIN32 */
#include <sys/types.h>
#include "config.h"

#define UNKNOWN_ERROR_MSG "Not defined in modbus specification"

/* This structure reduces the number of params in functions and so
 * optimizes the speed of execution (~ 37%). */
typedef struct {
	int slave;
	int function;
	int t_id;
} sft_t;

static const uint8_t NB_TAB_ERROR_MSG = 12;
static const char *TAB_ERROR_MSG[] = {
	/* 0x00 */ UNKNOWN_ERROR_MSG,
	/* 0x01 */ "Illegal function code",
	/* 0x02 */ "Illegal data address",
	/* 0x03 */ "Illegal data value",
	/* 0x04 */ "Slave device or server failure",
	/* 0x05 */ "Acknowledge",
	/* 0x06 */ "Slave device or server busy",
	/* 0x07 */ "Negative acknowledge",
	/* 0x08 */ "Memory parity error",
	/* 0x09 */ UNKNOWN_ERROR_MSG,
	/* 0x0A */ "Gateway path unavailable",
	/* 0x0B */ "Target device failed to respond"
};

/* Table of CRC values for high-order byte */
static uint8_t table_crc_hi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static uint8_t table_crc_lo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


//extern void busMonitorAddItem( uint8_t isOut,
//				uint8_t slave,
//				uint8_t func,
//				uint16_t addr,
//				uint16_t nb,
//				uint16_t crc );
//extern void busMonitorRawData( uint8_t * data,
//				uint8_t dataLen );


/* Treats errors and flush or close connection if necessary */
static void error_treat(modbus_param_t *mb_param, int code, const char *string)
{
	printf("\nERROR %s (%d)\n", string, code);

	if (mb_param->error_handling == FLUSH_OR_RECONNECT_ON_ERROR) {
		switch (code) {
		case ILLEGAL_DATA_VALUE:
		case ILLEGAL_DATA_ADDRESS:
		case ILLEGAL_FUNCTION:
			break;
		default:
			if (mb_param->type_com == RTU) {
#ifdef NATIVE_WIN32
							FlushFileBuffers(mb_param->w_ser.fd);
							mb_param->w_ser.n_bytes = 0;
#else  /* NATIVE_WIN32 */
                            tcflush(mb_param->fd, TCIOFLUSH);
#endif /* NATIVE_WIN32 */
				} else {
				modbus_close(mb_param);
				modbus_connect(mb_param);
			}
		}
	}
}

/* Computes the length of the expected response */
static unsigned int compute_response_length(modbus_param_t *mb_param,
					    uint8_t *query)
{
	int length;
	int offset;

	offset = mb_param->header_length;

	switch (query[offset + 1]) {
#ifdef HC_OLD
    case FC_HC_INIT_PARA:
    case FC_HC_QUERY_STATUS:
    case FC_HC_AUTO_ADJ:
        {
            length = ((query[offset + 5] << 8) | query[offset + 4]) << 1;
            length += 6;
        }
        break;
    case FC_HC_TEACH_STEP:
    case FC_HC_GET_AXIS_PARA:
    case FC_HC_SET_AXIS_PARA:
        {
            return  16;
        }
    case FC_HC_SELECT_CONFIG:
    {
        return 7;
    }
    case FC_HC_COMMAND:
        {
            length = 5;
        }
        break;
    case FC_HC_MANUAL_RUN:
        length = 8;
        break;
#else
	case FC_READ_COIL_STATUS:
	case FC_READ_INPUT_STATUS: {
		/* Header + nb values (code from force_multiple_coils) */
		int nb = (query[offset + 4] << 8) | query[offset + 5];
		length = 3 + (nb / 8) + ((nb % 8) ? 1 : 0);
	}
		break;
	case FC_READ_HOLDING_REGISTERS:
	case FC_READ_INPUT_REGISTERS:
		/* Header + 2 * nb values */
		length = 3 + 2 * (query[offset + 4] << 8 |
				       query[offset + 5]);
		break;
//    case FC_HC_ORIGIN_CHECK:
//    case FC_HC_CMD_EXECUTE:
	case FC_READ_EXCEPTION_STATUS:
		length = 4;
		break;
    case FC_HC_DOWNLOAD_ACT:
        length = 12;
        break;
#endif
	default:
		length = 6;
	}

	return length + offset + mb_param->checksum_length;
}

/* Builds a RTU query header */
static int build_query_basis_rtu(int slave, int function,
				 int start_addr, int nb,
				 uint8_t *query)
{
	query[0] = slave;
	query[1] = function;
	query[2] = start_addr >> 8;
	query[3] = start_addr & 0x00ff;
	query[4] = nb >> 8;
	query[5] = nb & 0x00ff;


	return PRESET_QUERY_LENGTH_RTU;
}

/* Builds a TCP query header */
static int build_query_basis_tcp(int slave, int function,
				 int start_addr, int nb,
				 uint8_t *query)
{

	/* Extract from MODBUS Messaging on TCP/IP Implementation
	   Guide V1.0b (page 23/46):
	   The transaction identifier is used to associate the future
	   response with the request. So, at a time, on a TCP
	   connection, this identifier must be unique.
	*/
	static uint16_t t_id = 0;

	/* Transaction ID */
	if (t_id < UINT16_MAX)
		t_id++;
	else
		t_id = 0;
	query[0] = t_id >> 8;
	query[1] = t_id & 0x00ff;

	/* Protocol Modbus */
	query[2] = 0;
	query[3] = 0;

	/* Length to fix later with set_query_length_tcp (4 and 5) */

	query[6] = slave;
	query[7] = function;
	query[8] = start_addr >> 8;
	query[9] = start_addr & 0x00ff;
	query[10] = nb >> 8;
	query[11] = nb & 0x00ff;

	return PRESET_QUERY_LENGTH_TCP;
}

static int build_query_basis(modbus_param_t *mb_param, int slave,
			     int function, int start_addr,
			     int nb, uint8_t *query)
{
	if (mb_param->type_com == RTU)
		return build_query_basis_rtu(slave, function, start_addr,
					     nb, query);
	else
		return build_query_basis_tcp(slave, function, start_addr,
					     nb, query);
}

/* Builds a RTU response header */
static int build_response_basis_rtu(sft_t *sft, uint8_t *response)
{
	response[0] = sft->slave;
	response[1] = sft->function;

	return PRESET_RESPONSE_LENGTH_RTU;
}

/* Builds a TCP response header */
static int build_response_basis_tcp(sft_t *sft, uint8_t *response)
{
	/* Extract from MODBUS Messaging on TCP/IP Implementation
	   Guide V1.0b (page 23/46):
	   The transaction identifier is used to associate the future
	   response with the request. */
	response[0] = sft->t_id >> 8;
	response[1] = sft->t_id & 0x00ff;

	/* Protocol Modbus */
	response[2] = 0;
	response[3] = 0;

	/* Length to fix later with set_message_length_tcp (4 and 5) */

	response[6] = sft->slave;
	response[7] = sft->function;

	return PRESET_RESPONSE_LENGTH_TCP;
}

static int build_response_basis(modbus_param_t *mb_param, sft_t *sft,
				uint8_t *response)
{
	if (mb_param->type_com == RTU)
		return build_response_basis_rtu(sft, response);
	else
		return build_response_basis_tcp(sft, response);
}

/* Sets the length of TCP message in the message (query and response) */
void set_message_length_tcp(uint8_t *msg, int msg_length)
{
	/* Substract the header length to the message length */
	int mbap_length = msg_length - 6;

	msg[4] = mbap_length >> 8;
	msg[5] = mbap_length & 0x00FF;
}

/* Fast CRC */
static uint16_t crc16(uint8_t *buffer, uint16_t buffer_length)
{
	uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
	uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
	unsigned int i; /* will index into CRC lookup */

	/* pass through message buffer */
	while (buffer_length--) {
		i = crc_hi ^ *buffer++; /* calculate the CRC  */
		crc_hi = crc_lo ^ table_crc_hi[i];
		crc_lo = table_crc_lo[i];
	}

    return (crc_hi << 8 | crc_lo);
}

/* If CRC is correct returns 0 else returns INVALID_CRC */
static int check_crc16(modbus_param_t *mb_param,
		       uint8_t *msg,
		       const int msg_length)
{
	int ret;
	uint16_t crc_calc;
	uint16_t crc_received;

	crc_calc = crc16(msg, msg_length - 2);
	crc_received = (msg[msg_length - 2] << 8) | msg[msg_length - 1];

	/* Check CRC of msg */
	if (crc_calc == crc_received) {
		ret = 0;
	} else {
		char s_error[64];
		sprintf(s_error,
			"invalid crc received %0X - crc_calc %0X",
			crc_received, crc_calc);
		ret = INVALID_CRC;
		error_treat(mb_param, ret, s_error);
	}

	return ret;
}

/* Sends a query/response over a serial or a TCP communication */
static int modbus_send(modbus_param_t *mb_param, uint8_t *query,
		       int query_length)
{
	int ret;
	uint16_t s_crc = 0;
	int i;

	if (mb_param->type_com == RTU) {
		s_crc = crc16(query, query_length);
		query[query_length++] = s_crc >> 8;
        query[query_length++] = s_crc & 0x00FF;
	} else {
		set_message_length_tcp(query, query_length);
	}
//	busMonitorAddItem( 1, query[0], query[1],
//				( query[2] << 8 ) + query[3],
//				( query[4] << 8 ) + query[5],
//				s_crc );

	if (mb_param->debug) {
		for (i = 0; i < query_length; i++)
			printf("[%.2X]", query[i]);
		printf("\n");
	}

	if (mb_param->type_com == RTU)
#ifdef NATIVE_WIN32
			{
				DWORD n_bytes = 0;
				ret = (WriteFile(mb_param->w_ser.fd, query, query_length, &n_bytes, NULL)) ? n_bytes : -1;
			}
#else /* NATIVE_WIN32 */
				ret = write(mb_param->fd, query, query_length);
#endif /* NATIVE_WIN32 */
	else
#ifdef NATIVE_WIN32
				ret = -1; /* TODO: ethernet support */
#else /* NATIVE_WIN32 */
		ret = send(mb_param->fd, query, query_length, 0);
#endif /* NATIVE_WIN32 */

	/* Return the number of bytes written (0 to n)
	   or PORT_SOCKET_FAILURE on error */
	if ((ret == -1) || (ret != query_length)) {
		ret = PORT_SOCKET_FAILURE;
        error_treat(mb_param, ret, "Write port/socket failure");
	}

	return ret;
}

/* Computes the length of the header following the function code */
static uint8_t compute_query_length_header(int function)
{
	int length;

	if (function <= FC_FORCE_SINGLE_COIL ||
	    function == FC_PRESET_SINGLE_REGISTER)
		/* Read and single write */
		length = 4;
	else if (function == FC_FORCE_MULTIPLE_COILS ||
		 function == FC_PRESET_MULTIPLE_REGISTERS)
		/* Multiple write */
		length = 5;
	else
		length = 0;

	return length;
}

/* Computes the length of the data to write in the query */
static int compute_query_length_data(modbus_param_t *mb_param, uint8_t *msg)
{
	int function = msg[mb_param->header_length + 1];
	int length;

	if (function == FC_FORCE_MULTIPLE_COILS ||
	    function == FC_PRESET_MULTIPLE_REGISTERS)
		length = msg[mb_param->header_length + 6];
	else
		length = 0;

	length += mb_param->checksum_length;

	return length;
}

#ifdef NATIVE_WIN32
/* This simple implementation is sort of a substitute of the select() call, working
 * this way: the win32_ser_select() call tries to read some data from the serial port,
 * setting the timeout as the select() call would. Data read is stored into the
 * receive buffer, that is then consumed by the win32_ser_read() call.
 * So win32_ser_select() does both the event waiting and the reading,
 * while win32_ser_read() only consumes the receive buffer.
 */

static void win32_ser_init(struct win32_ser *ws) {
	/* Clear everything */
	memset(ws,0x00,sizeof(struct win32_ser));
	/* Set file handle to invalid */
	ws->fd = INVALID_HANDLE_VALUE;
}

static int win32_ser_select(struct win32_ser *ws, int max_len, struct timeval *tv) {
	COMMTIMEOUTS comm_to; unsigned int msec = 0;
	/* Check if some data still in the buffer to be consumed */
	if (ws->n_bytes> 0) {
		return 1;
	}
	/* Setup timeouts like select() would do */
	msec = tv->tv_sec * 1000 + tv->tv_usec / 1000;
	if (msec < 1) msec = 1;
	comm_to.ReadIntervalTimeout = msec;
	comm_to.ReadTotalTimeoutMultiplier = 0;
	comm_to.ReadTotalTimeoutConstant = msec;
	comm_to.WriteTotalTimeoutMultiplier = 0;
	comm_to.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(ws->fd,&comm_to);
	/* Read some bytes */
	if ((max_len > PY_BUF_SIZE) || (max_len < 0)) {
		max_len = PY_BUF_SIZE;
	}
	if (ReadFile(ws->fd, &ws->buf, max_len, &ws->n_bytes, NULL)) {
		/* Check if some bytes available */
		if (ws->n_bytes > 0) {
			/* Some bytes read */
			return 1;
		} else {
			/* Just timed out */
			return 0;
		}
	} else {
		/* Some kind of error */
		return -1;
	}
}

static int win32_ser_read(struct win32_ser *ws, char *p_msg, unsigned int max_len) {
	unsigned int n = ws->n_bytes;
	if (max_len < n) {
		n = max_len;
	}
	if (n > 0) {
		memcpy(p_msg,ws->buf,n);
	}
	ws->n_bytes -= n;
	return(n);
}

#define WAIT_DATA() 																\
{																					\
	select_ret = win32_ser_select(&mb_param->w_ser, msg_length_computed, &tv);		\
	if (select_ret == 0) {															\
		return COMM_TIME_OUT;														\
	}																				\
	if (select_ret < 0) {															\
		error_treat(mb_param, SELECT_FAILURE, "Select failure");       				\
		return SELECT_FAILURE;														\
	}																				\
}
#else /* NATIVE_WIN32 */
#define WAIT_DATA()                                                                \
{                                                                                  \
    while ((select_ret = select(mb_param->fd+1, &rfds, NULL, NULL, &tv)) == -1) {  \
	    if (errno == EINTR) {                                                  \
		    printf("A non blocked signal was caught\n");                   \
		    /* Necessary after an error */                                 \
		    FD_ZERO(&rfds);                                                \
            FD_SET(mb_param->fd, &rfds);                                   \
	    } else {                                                               \
		    error_treat(mb_param, SELECT_FAILURE, "Select failure");       \
		    return SELECT_FAILURE;                                         \
	    }                                                                      \
    }                                                                              \
										   \
    if (select_ret == 0) {                                                         \
	    /* Call to error_treat is done later to manage exceptions */           \
	    return COMM_TIME_OUT;                                                  \
    }                                                                              \
}
#endif /* NATIVE_WIN32 */

/* Waits a reply from a modbus slave or a query from a modbus master.
   This function blocks for timeout seconds if there is no reply.

   In
   - msg_length_computed must be set to MSG_LENGTH_UNDEFINED if undefined

   Out
   - msg is an array of uint8_t to receive the message
   - p_msg_length, the variable is assigned to the number of
     characters received. This value won't be greater than
     msg_length_computed.

   Returns 0 in success or a negative value if an error occured.
*/
static int receive_msg(modbus_param_t *mb_param,
		       int msg_length_computed,
		       uint8_t *msg, int *p_msg_length,
		       int timeout_usec)
{
	int select_ret;
	int read_ret;
#ifndef NATIVE_WIN32
	fd_set rfds;
#endif /* NATIVE_WIN32 */
	struct timeval tv;
	int length_to_read;
	uint8_t *p_msg;
	enum { FUNCTION, BYTE, COMPLETE };
	int state;

	if (mb_param->debug) {
		if (msg_length_computed == MSG_LENGTH_UNDEFINED)
			printf("Waiting for a message...\n");
		else
			printf("Waiting for a message (%d bytes)...\n",
			       msg_length_computed);
	}

	/* Add a file descriptor to the set */
#ifndef NATIVE_WIN32
	FD_ZERO(&rfds);
	FD_SET(mb_param->fd, &rfds);
#endif /* NATIVE_WIN32 */

	if (msg_length_computed == MSG_LENGTH_UNDEFINED) {
		/* Wait for a message */
		tv.tv_sec = timeout_usec / ( 1000*1000 );
		tv.tv_usec = timeout_usec % ( 1000*1000 );

		/* The message length is undefined (query receiving) so
		 * we need to analyse the message step by step.
		 * At the first step, we want to reach the function
		 * code because all packets have that information. */
		msg_length_computed = mb_param->header_length + 2;
		state = FUNCTION;
	} else {
		tv.tv_sec = 0;
		tv.tv_usec = TIME_OUT_BEGIN_OF_TRAME;
		state = COMPLETE;
	}

	length_to_read = msg_length_computed;

	select_ret = 0;
	WAIT_DATA();

	/* Initialize the readin the message */
	(*p_msg_length) = 0;
	p_msg = msg;

	while (select_ret) {
		if (mb_param->type_com == RTU)
#ifdef NATIVE_WIN32
						read_ret = win32_ser_read(&mb_param->w_ser, p_msg, length_to_read);
#else /* NATIVE_WIN32 */
			read_ret = read(mb_param->fd, p_msg, length_to_read);
#endif /* NATIVE_WIN32 */
		else
#ifdef NATIVE_WIN32
						read_ret = -1; /* TODO: ethernet support */
#else /* NATIVE_WIN32 */
			read_ret = recv(mb_param->fd, p_msg, length_to_read, 0);
#endif /* NATIVE_WIN32 */

		if (read_ret == 0) {
			printf("Connection closed\n");
			return CONNECTION_CLOSED;
		} else if (read_ret < 0) {
			/* The only negative possible value is -1 */
			error_treat(mb_param, PORT_SOCKET_FAILURE,
				    "Read port/socket failure");
			return PORT_SOCKET_FAILURE;
		}

		/* Sums bytes received */
		(*p_msg_length) += read_ret;

		/* Display the hex code of each character received */
		if (mb_param->debug) {
			int i;
			for (i=0; i < read_ret; i++)
				printf("<%.2X>", p_msg[i]);
		}

//        if(p_msg[1] == 0x82 && p_msg[2] == 0x17)
//        {
//            return 0;
//        }
		if ((*p_msg_length) < msg_length_computed) {
			/* Message incomplete */
			length_to_read = msg_length_computed - (*p_msg_length);
		} else {
			switch (state) {
			case FUNCTION:
				/* Function code position */
				length_to_read = compute_query_length_header(msg[mb_param->header_length + 1]);
				msg_length_computed += length_to_read;
				/* It's useless to check
				   p_msg_length_computed value in this
				   case (only defined values are used). */
				state = BYTE;
				break;
			case BYTE:
				length_to_read = compute_query_length_data(mb_param, msg);
				msg_length_computed += length_to_read;
				if (msg_length_computed > MAX_MESSAGE_LENGTH) {
				     error_treat(mb_param, TOO_MANY_DATA, "Too many data");
				     return TOO_MANY_DATA;
				}
				state = COMPLETE;
//				busMonitorRawData( msg, msg_length_computed );
				break;
			case COMPLETE:
				length_to_read = 0;
				break;
			}
		}

		/* Moves the pointer to receive other data */
		p_msg = &(p_msg[read_ret]);

		if (length_to_read > 0) {
			/* If no character at the buffer wait
			   TIME_OUT_END_OF_TRAME before to generate an error. */
			tv.tv_sec = 0;
			tv.tv_usec = TIME_OUT_END_OF_TRAME;

			WAIT_DATA();
		} else {
			/* All chars are received */
			select_ret = FALSE;
		}
	}

	if (mb_param->debug)
		printf("\n");

	if (mb_param->type_com == RTU) {
		check_crc16(mb_param, msg, (*p_msg_length));
	}

	/* OK */
	return 0;
}



/* Receives the response and checks values (and checksum in RTU).

   Returns:
   - the number of values (bits or word) if success or the response
     length if no value is returned
   - less than 0 for exception errors

   Note: all functions used to send or receive data with modbus return
   these values. */
static int modbus_receive(modbus_param_t *mb_param,
			  uint8_t *query,
			  uint8_t *response)
{
	int ret;
	int response_length = 0;
	int response_length_computed;
	int offset = mb_param->header_length;

    response_length_computed = compute_response_length(mb_param, query);
	ret = receive_msg(mb_param, response_length_computed,
			  response, &response_length, TIME_OUT_DEFAULT);
	if (ret == 0) {
		/* GOOD RESPONSE */
		int query_nb_value;
		int response_nb_value;
		int data_len = -1;
		int nb = 0;
		/* The number of values is returned if it's corresponding
		 * to the query */
//        printf("rl:%d\n", response_length);
//        for(int i = 0; i != response_length; ++i)
//        {
//            printf("response:%d ", response[i]);
//        }
//        printf("\n");
		switch (response[offset + 1]) {
#ifdef HC_OLD
        case FC_HC_INIT_PARA:
            {
            nb = query_nb_value = ((query[offset + 5] << 8) | query[offset + 4]) << 1;
            response_nb_value = ((response[offset + 5] << 8) | response[offset + 4]) << 1;
            }
            break;
        case FC_HC_AUTO_ADJ:
        {
            query_nb_value = response_nb_value = 14;
        }
        break;
        case FC_HC_QUERY_STATUS:
            {
//                response[2] = 0x17;
                if(response[2] == 0x17)
                {
                    query_nb_value = response_nb_value = 5;
                }
                else
                {
                    nb = query_nb_value = ((query[offset + 5] << 8) | query[offset + 4]) << 1;
                    response_nb_value = ((response[offset + 5] << 8) | response[offset + 4]) << 1;
                }
            }
            break;
        case FC_HC_COMMAND:
            {
                response_nb_value = query_nb_value = 5;
            }
            break;
        case FC_HC_TEACH_STEP:
        case FC_HC_GET_AXIS_PARA:
        case FC_HC_SET_AXIS_PARA:
            {
                query_nb_value = response_nb_value = 16;
            }
            break;
        case FC_HC_MODIFY_SYS_PARA:
        case FC_HC_TEACH_PARA:
            {
                response_nb_value = query_nb_value = 6;
            }
            break;
        case FC_HC_SELECT_CONFIG:
        {
            query_nb_value = response_nb_value = 7;
        }
            break;
        case FC_HC_MANUAL_RUN:
            query_nb_value = response_nb_value = 10;
            break;
#else
		case FC_READ_COIL_STATUS:
		case FC_READ_INPUT_STATUS:
			/* Read functions, 8 values in a byte (nb
			 * of values in the query and byte count in
			 * the response. */
			nb = query_nb_value = (query[offset+4] << 8) + query[offset+5];
			query_nb_value = (query_nb_value / 8) + ((query_nb_value % 8) ? 1 : 0);
			response_nb_value = response[offset + 2];
			break;
		case FC_READ_HOLDING_REGISTERS:
		case FC_READ_INPUT_REGISTERS:
			/* Read functions 1 value = 2 bytes */
			query_nb_value = (query[offset+4] << 8) + query[offset+5];
			response_nb_value = (response[offset + 2] / 2);
			nb = response_nb_value;
			data_len = response[offset + 2];
			break;
		case FC_FORCE_MULTIPLE_COILS:
		case FC_PRESET_MULTIPLE_REGISTERS:
			/* N Write functions */
            query_nb_value = (query[offset+4] << 8) + query[offset+5];
            response_nb_value = (response[offset + 4] << 8) | response[offset + 5];
			nb = response_nb_value;
			data_len = 3;
			break;
		case FC_REPORT_SLAVE_ID:
			/* Report slave ID (bytes received) */
			query_nb_value = response_nb_value = response_length;
			break;
        case FC_HC_UPLOAD_ACT:
        case FC_HC_RUNTIME_MODIFY:
            query_nb_value = response_nb_value = 2;
            break;
        case FC_HC_DOWNLOAD_ACT:
            query_nb_value = response_nb_value = 5;
            break;

#endif
		default:
			/* 1 Write functions & others */
			query_nb_value = response_nb_value = 1;
		}
		if( data_len < 0 )
		{
			data_len = response_nb_value;
		}
//		busMonitorAddItem( 0, response[offset+0], response[offset+1],
//					0, nb,
//					( response[offset+3+data_len] << 8 ) |
//								response[offset+4+data_len] );
		if (query_nb_value == response_nb_value) {
			ret = response_nb_value;
		} else {
			char *s_error = malloc(64 * sizeof(char));
			sprintf(s_error, "Quantity (%d) not corresponding to the query (%d)",
				response_nb_value, query_nb_value);
			ret = ILLEGAL_DATA_VALUE;
			error_treat(mb_param, ILLEGAL_DATA_VALUE, s_error);
			free(s_error);
		}
	} else if (ret == COMM_TIME_OUT) {

		if (response_length == (offset + 3 + mb_param->checksum_length)) {
			/* EXCEPTION CODE RECEIVED */

			/* Optimization allowed because exception response is
			   the smallest trame in modbus protocol (3) so always
			   raise a timeout error */

//			busMonitorAddItem( 0, response[offset+0], response[offset+1],
//					0, 0,
//					( response[response_length-2] << 8 ) |
//								response[response_length-1] );

			/* CRC must be checked here (not done in receive_msg) */
			if (mb_param->type_com == RTU) {
				ret = check_crc16(mb_param, response, response_length);
				if (ret != 0)
					return ret;
			}

			/* Check for exception response.
			   0x80 + function is stored in the exception
			   response. */
			if (0x80 + query[offset + 1] == response[offset + 1]) {

				int exception_code = response[offset + 2];
				/* FIXME check test */
				if (exception_code < NB_TAB_ERROR_MSG) {
					error_treat(mb_param, -exception_code,
						    TAB_ERROR_MSG[response[offset + 2]]);
					/* RETURN THE EXCEPTION CODE */
					/* Modbus error code is negative */
					return -exception_code;
				} else {
					/* The chances are low to hit this
					   case but it can avoid a vicious
					   segfault */
					char *s_error = malloc(64 * sizeof(char));
					sprintf(s_error,
						"Invalid exception code %d",
						response[offset + 2]);
					error_treat(mb_param, INVALID_EXCEPTION_CODE,
						    s_error);
					free(s_error);
					return INVALID_EXCEPTION_CODE;
				}
			}
			/* If doesn't return previously, return as
			   TIME OUT here */
		}

		/* COMMUNICATION TIME OUT */
//		error_treat(mb_param, ret, "Communication time out");
		return ret;
	}

	return ret;
}

static int response_io_status(int address, int nb,
			      uint8_t *tab_io_status,
			      uint8_t *response, int offset)
{
	int shift = 0;
	int byte = 0;
	int i;

	for (i = address; i < address+nb; i++) {
		byte |= tab_io_status[i] << shift;
		if (shift == 7) {
			/* Byte is full */
			response[offset++] = byte;
			byte = shift = 0;
		} else {
			shift++;
		}
	}

	if (shift != 0)
		response[offset++] = byte;

	return offset;
}

/* Build the exception response */
static int response_exception(modbus_param_t *mb_param, sft_t *sft,
			      int exception_code, uint8_t *response)
{
	int response_length;

	sft->function = sft->function + 0x80;
	response_length = build_response_basis(mb_param, sft, response);

	/* Positive exception code */
	response[response_length++] = -exception_code;

	return response_length;
}

/* Manages the received query.
   Analyses the query and constructs a response.
   If an error occurs, this function construct the response
   accordingly.
*/
void modbus_manage_query(modbus_param_t *mb_param, const uint8_t *query,
			 int query_length, modbus_mapping_t *mb_mapping)
{
	int offset = mb_param->header_length;
	int slave = query[offset];
	int function = query[offset+1];
	uint16_t address = (query[offset+2] << 8) + query[offset+3];
	uint8_t response[MAX_MESSAGE_LENGTH];
	int resp_length = 0;
	sft_t sft;

	sft.slave = slave;
	sft.function = function;
	if (mb_param->type_com == TCP) {
		sft.t_id = (query[0] << 8) + query[1];
	} else {
		sft.t_id = 0;
		query_length -= CHECKSUM_LENGTH_RTU;
	}

	switch (function) {
	case FC_READ_COIL_STATUS: {
		int nb = (query[offset+4] << 8) + query[offset+5];

		if ((address + nb) > mb_mapping->nb_coil_status) {
			printf("Illegal data address %0X in read_coil_status\n",
			       address + nb);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			resp_length = build_response_basis(mb_param, &sft, response);
			response[resp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
			resp_length = response_io_status(address, nb,
							 mb_mapping->tab_coil_status,
							 response, resp_length);
		}
	}
		break;
	case FC_READ_INPUT_STATUS: {
		/* Similar to coil status (but too much arguments to use a
		 * function) */
		int nb = (query[offset+4] << 8) + query[offset+5];

		if ((address + nb) > mb_mapping->nb_input_status) {
			printf("Illegal data address %0X in read_input_status\n",
			       address + nb);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			resp_length = build_response_basis(mb_param, &sft, response);
			response[resp_length++] = (nb / 8) + ((nb % 8) ? 1 : 0);
			resp_length = response_io_status(address, nb,
							 mb_mapping->tab_input_status,
							 response, resp_length);
		}
	}
		break;
	case FC_READ_HOLDING_REGISTERS: {
		int nb = (query[offset+4] << 8) + query[offset+5];

		if ((address + nb) > mb_mapping->nb_holding_registers) {
			printf("Illegal data address %0X in read_holding_registers\n",
			       address + nb);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			int i;

			resp_length = build_response_basis(mb_param, &sft, response);
			response[resp_length++] = nb << 1;
			for (i = address; i < address + nb; i++) {
				response[resp_length++] = mb_mapping->tab_holding_registers[i] >> 8;
				response[resp_length++] = mb_mapping->tab_holding_registers[i] & 0xFF;
			}
		}
	}
		break;
	case FC_READ_INPUT_REGISTERS: {
		/* Similar to holding registers (but too much arguments to use a
		 * function) */
		int nb = (query[offset+4] << 8) + query[offset+5];

		if ((address + nb) > mb_mapping->nb_input_registers) {
			printf("Illegal data address %0X in read_input_registers\n",
			       address + nb);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			int i;

			resp_length = build_response_basis(mb_param, &sft, response);
			response[resp_length++] = nb << 1;
			for (i = address; i < address + nb; i++) {
				response[resp_length++] = mb_mapping->tab_input_registers[i] >> 8;
				response[resp_length++] = mb_mapping->tab_input_registers[i] & 0xFF;
			}
		}
	}
		break;
	case FC_FORCE_SINGLE_COIL:
		if (address >= mb_mapping->nb_coil_status) {
			printf("Illegal data address %0X in force_singe_coil\n", address);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			int data = (query[offset+4] << 8) + query[offset+5];

			if (data == 0xFF00 || data == 0x0) {
				mb_mapping->tab_coil_status[address] = (data) ? ON : OFF;

				/* In RTU mode, the CRC is computed and added
				   to the query by modbus_send, the computed
				   CRC will be same and optimisation is
				   possible here (FIXME). */
				memcpy(response, query, query_length);
				resp_length = query_length;
			} else {
				printf("Illegal data value %0X in force_single_coil request at address %0X\n",
				       data, address);
				resp_length = response_exception(mb_param, &sft,
								 ILLEGAL_DATA_VALUE, response);
			}
		}
		break;
	case FC_PRESET_SINGLE_REGISTER:
		if (address >= mb_mapping->nb_holding_registers) {
			printf("Illegal data address %0X in preset_holding_register\n", address);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			int data = (query[offset+4] << 8) + query[offset+5];

			mb_mapping->tab_holding_registers[address] = data;
			memcpy(response, query, query_length);
			resp_length = query_length;
		}
		break;
	case FC_FORCE_MULTIPLE_COILS: {
		int nb = (query[offset+4] << 8) + query[offset+5];

		if ((address + nb) > mb_mapping->nb_coil_status) {
			printf("Illegal data address %0X in force_multiple_coils\n",
			       address + nb);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			/* 6 = byte count, 7 = first byte of data */
			set_bits_from_bytes(mb_mapping->tab_coil_status, address, nb, &query[offset + 7]);

			resp_length = build_response_basis(mb_param, &sft, response);
			/* 4 to copy the coil address (2) and the quantity of coils */
			memcpy(response + resp_length, query + resp_length, 4);
			resp_length += 4;
		}
	}
		break;
	case FC_PRESET_MULTIPLE_REGISTERS: {
		int nb = (query[offset+4] << 8) + query[offset+5];

		if ((address + nb) > mb_mapping->nb_holding_registers) {
			printf("Illegal data address %0X in preset_multiple_registers\n",
			       address + nb);
			resp_length = response_exception(mb_param, &sft,
							 ILLEGAL_DATA_ADDRESS, response);
		} else {
			int i, j;
			for (i = address, j = 0; i < address + nb; i++, j += 2) {
				/* 6 = byte count, 7 and 8 = first value */
				mb_mapping->tab_holding_registers[i] =
					(query[offset + 7 + j] << 8) + query[offset + 8 + j];
			}

			resp_length = build_response_basis(mb_param, &sft, response);
			/* 4 to copy the address (2) and the no. of registers */
			memcpy(response + resp_length, query + resp_length, 4);
			resp_length += 4;
		}
	}
		break;
	case FC_READ_EXCEPTION_STATUS:
	case FC_REPORT_SLAVE_ID:
		printf("Not implemented\n");
		break;
	}

	modbus_send(mb_param, response, resp_length);
}

/* Listens any message on a socket or file descriptor.
   Returns:
   - 0 if OK, or a negative error number if the request fails
   - query, message received
   - query_length, length in bytes of the message */
int modbus_listen(modbus_param_t *mb_param, uint8_t *query, int *query_length, int timeout)
{
	int ret;

	/* The length of the query to receive isn't known. */
	ret = receive_msg(mb_param, MSG_LENGTH_UNDEFINED, query, query_length, timeout);

	return ret;
}

/* Reads IO status */
static int read_io_status(modbus_param_t *mb_param, int slave, int function,
			  int start_addr, int nb, uint8_t *data_dest)
{
	int ret;
	int query_length;

	uint8_t query[MIN_QUERY_LENGTH];
	uint8_t response[MAX_MESSAGE_LENGTH];

	query_length = build_query_basis(mb_param, slave, function,
					 start_addr, nb, query);

	ret = modbus_send(mb_param, query, query_length);
	if (ret > 0) {
		int i, temp, bit;
		int pos = 0;
		int offset;
		int offset_length;

		ret = modbus_receive(mb_param, query, response);
		if (ret < 0)
			return ret;

		offset = mb_param->header_length;

		offset_length = offset + ret;
		for (i = offset; i < offset_length; i++) {
			/* Shift reg hi_byte to temp */
			temp = response[3 + i];

			for (bit = 0x01; (bit & 0xff) && (pos < nb);) {
				data_dest[pos++] = (temp & bit) ? TRUE : FALSE;
				bit = bit << 1;
			}

		}
	}

	return ret;
}

/* Reads the boolean status of coils and sets the array elements
   in the destination to TRUE or FALSE. */
int read_coil_status(modbus_param_t *mb_param, int slave, int start_addr,
		     int nb, uint8_t *data_dest)
{
	int status;

	if (nb > MAX_STATUS) {
		printf("ERROR Too many coils status requested (%d > %d)\n",
		       nb, MAX_STATUS);
		return TOO_MANY_DATA;
	}

	status = read_io_status(mb_param, slave, FC_READ_COIL_STATUS,
				start_addr, nb, data_dest);

	if (status > 0)
		status = nb;

	return status;
}


/* Same as read_coil_status but reads the slaves input table */
int read_input_status(modbus_param_t *mb_param, int slave, int start_addr,
		      int nb, uint8_t *data_dest)
{
	int status;

	if (nb > MAX_STATUS) {
		printf("ERROR Too many input status requested (%d > %d)\n",
		       nb, MAX_STATUS);
		return TOO_MANY_DATA;
	}

	status = read_io_status(mb_param, slave, FC_READ_INPUT_STATUS,
				start_addr, nb, data_dest);

	if (status > 0)
		status = nb;

	return status;
}

/* Reads the data from a modbus slave and put that data into an array */
static int  read_registers(modbus_param_t *mb_param, int slave, int function,
			  int start_addr, int nb, uint16_t *data_dest)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
	int ret;
	int query_length;
	uint8_t query[MIN_QUERY_LENGTH];
	uint8_t response[MAX_MESSAGE_LENGTH];

	if (nb > MAX_REGISTERS) {
		printf("EROOR Too many holding registers requested (%d > %d)\n",
		       nb, MAX_REGISTERS);
		return TOO_MANY_DATA;
	}

	query_length = build_query_basis(mb_param, slave, function,
					 start_addr, nb, query);

	ret = modbus_send(mb_param, query, query_length);
	if (ret > 0) {
		int offset;
		int i;

		ret = modbus_receive(mb_param, query, response);

		offset = mb_param->header_length;

        /* If is functin used in SZHC */
        if(response[offset + 1] > 0x40)
        {
//            if(function == FC_HC_DOWNLOAD_ACT)
//            {
//                printf("in modbut %d\n", ret);
//            }
            for (i = 0; i < ret; i++) {
                /* shift reg hi_byte to temp OR with lo_byte */
                data_dest[i] = (response[offset + 2 + (i << 1)] << 8) |
                    response[offset + 3 + (i << 1)];
            }
        }
        else
        {
            for (i = 0; i < ret; i++) {
                /* shift reg hi_byte to temp OR with lo_byte */
                data_dest[i] = (response[offset + 3 + (i << 1)] << 8) |
                               response[offset + 4 + (i << 1)];
            }
        }
//
    }

	return ret;
}

static int hc_upload_act_query(modbus_param_t *mb_param,
                                     int slave,
                                     int cmdType,
                                     int step,
                                     int commandCode,
                                     int param1,
                                     int param2,
                                     int param3,
                                     uint16_t *data_dest)
{
    int ret;
    int query_length;
    uint8_t query[16];
    uint8_t response[MAX_MESSAGE_LENGTH];

    query[0]  = slave;
    query[1]  = FC_HC_UPLOAD_ACT;
    query[2]  = cmdType >> 8;
    query[3]  = cmdType & 0x00ff;
    query[4]  = step >> 8;
    query[5]  = step & 0x00ff;
    query[6]  = commandCode >> 8;
    query[7]  = commandCode & 0x00ff;
    query[8]  = param1 >> 8;
    query[9]  = param1 & 0x00ff;
    query[10] = param2 >> 8;
    query[11] = param2 & 0x00ff;
    query[12] = param3 >> 8;
    query[13] = param3 & 0x00ff;

    query_length = 14;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0)
    {
        int offset;
        int i;

        ret = modbus_receive(mb_param, query, response);

        offset = mb_param->header_length;

        /* If ret is negative, the loop is jumped ! */
        for (i = 0; i < ret; i++)
        {
            /* shift reg hi_byte to temp OR with lo_byte */
            data_dest[i] = (response[offset + 2 + (i << 1)] << 8) |
                           response[offset + 3 + (i << 1)];
        }
    }

    return ret;
}

static int hc_runtime_modify_query(modbus_param_t *mb_param,
                                   int slave,
                                   int step,
                                   int param1,
                                   int param2,
                                   int param3,
                                   uint16_t *data_dest)
{
    int ret;
    int query_length;
    uint8_t query[16];
    uint8_t response[MAX_MESSAGE_LENGTH];

    query[0]  = slave;
    query[1]  = FC_HC_RUNTIME_MODIFY;
    query[2]  = step >> 8;
    query[3]  = step & 0x00ff;
    query[4]  = param1 >> 8;
    query[5]  = param1 & 0x00ff;
    query[6] = param2 >> 8;
    query[7] = param2 & 0x00ff;
    query[8] = param3 >> 8;
    query[9] = param3 & 0x00ff;

    query_length = 10;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0)
    {
        int offset;
        int i;

        ret = modbus_receive(mb_param, query, response);

        offset = mb_param->header_length;

        /* If ret is negative, the loop is jumped ! */
        for (i = 0; i < ret; i++)
        {
            /* shift reg hi_byte to temp OR with lo_byte */
            data_dest[i] = (response[offset + 2 + (i << 1)] << 8) |
                           response[offset + 3 + (i << 1)];
        }
    }

    return ret;
}

int hc_manual_run(modbus_param_t *mb_param,
                                   int slave,
                                   int num,
                                   int gm,
                                   int sub,
                                   int pos,
                                   int ifval)
{
    int ret;
    int query_length;
    uint8_t query[16];
    uint8_t response[MAX_MESSAGE_LENGTH];

    query[0]  = slave;
    query[1]  = FC_HC_MANUAL_RUN;
    query[2]  = num;
    query[3]  = gm;
    query[4]  = sub;
    query[5]  = pos >> 8;
    query[6]  = pos & 0x00ff;
    query[7] = ifval;

    query_length = 8;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0)
    {
        int i;

        ret = modbus_receive(mb_param, query, response);
//        qDebug()<<"ret"
        printf("ret: %d\n", ret);
        if(ret < 0) return -1;
        for(int i = 0; i != ret; ++i)
        {
            printf("i:%d ", response[i]);
        }
        printf("\n");

//        if(ret != query_length) return -1;

        /* If ret is negative, the loop is jumped ! */
        for (i = 0; i < ret; i++)
        {
            if(response[i] != query[i])
            {
                return -1;
            }
        }
        return ret;
    }

    return -1;
}

/* Reads the holding registers in a slave and put the data into an
   array */
int read_holding_registers(modbus_param_t *mb_param, int slave,
			   int start_addr, int nb, uint16_t *data_dest)
{
	int status;

	if (nb > MAX_REGISTERS) {
		printf("ERROR Too many holding registers requested (%d > %d)\n",
		       nb, MAX_REGISTERS);
		return TOO_MANY_DATA;
	}

	status = read_registers(mb_param, slave, FC_READ_HOLDING_REGISTERS,
				start_addr, nb, data_dest);
	return status;
}

/* Reads the input registers in a slave and put the data into
   an array */
int read_input_registers(modbus_param_t *mb_param, int slave,
			 int start_addr, int nb, uint16_t *data_dest)
{
	int status;

	if (nb > MAX_REGISTERS) {
		printf("ERROR Too many input registers requested (%d > %d)\n",
		       nb, MAX_REGISTERS);
		return TOO_MANY_DATA;
	}

	status = read_registers(mb_param, slave, FC_READ_INPUT_REGISTERS,
				start_addr, nb, data_dest);

	return status;
}

/* Sends a value to a register in a slave.
   Used by force_single_coil and preset_single_register */
static int set_single(modbus_param_t *mb_param, int slave, int function,
		      int addr, int value)
{
	int ret;
	int query_length;
	uint8_t query[MIN_QUERY_LENGTH];

	query_length = build_query_basis(mb_param, slave, function,
					 addr, value, query);

	ret = modbus_send(mb_param, query, query_length);
	if (ret > 0) {
		/* Used by force_single_coil and
		 * preset_single_register */
		uint8_t response[MIN_QUERY_LENGTH];
		ret = modbus_receive(mb_param, query, response);
	}

	return ret;
}

/* Turns ON or OFF a single coil in the slave device */
int force_single_coil(modbus_param_t *mb_param, int slave,
		      int coil_addr, int state)
{
	int status;

	if (state)
		state = 0xFF00;

	status = set_single(mb_param, slave, FC_FORCE_SINGLE_COIL,
			    coil_addr, state);

	return status;
}

/* Sets a value in one holding register in the slave device */
int preset_single_register(modbus_param_t *mb_param, int slave,
			   int reg_addr, int value)
{
	int status;

	status = set_single(mb_param, slave, FC_PRESET_SINGLE_REGISTER,
			    reg_addr, value);

	return status;
}

/* Sets/resets the coils in the slave from an array in argument */
int force_multiple_coils(modbus_param_t *mb_param, int slave,
			 int start_addr, int nb,
			 const uint8_t *data_src)
{
	int ret;
	int i;
	int byte_count;
	int query_length;
	int coil_check = 0;
	int pos = 0;

	uint8_t query[MAX_MESSAGE_LENGTH];

	if (nb > MAX_STATUS) {
		printf("ERROR Writing to too many coils (%d > %d)\n",
		       nb, MAX_STATUS);
		return TOO_MANY_DATA;
	}

	query_length = build_query_basis(mb_param, slave,
					 FC_FORCE_MULTIPLE_COILS,
					 start_addr, nb, query);
	byte_count = (nb / 8) + ((nb % 8) ? 1 : 0);
	query[query_length++] = byte_count;

	for (i = 0; i < byte_count; i++) {
		int bit;

		bit = 0x01;
		query[query_length] = 0;

		while ((bit & 0xFF) && (coil_check++ < nb)) {
			if (data_src[pos++])
				query[query_length] |= bit;
			else
				query[query_length] &=~ bit;

			bit = bit << 1;
		}
		query_length++;
	}

	ret = modbus_send(mb_param, query, query_length);
	if (ret > 0) {
		uint8_t response[MAX_MESSAGE_LENGTH];
		ret = modbus_receive(mb_param, query, response);
	}


	return ret;
}

/* Copies the values in the slave from the array given in argument */
int preset_multiple_registers(modbus_param_t *mb_param, int slave,
			      int start_addr, int nb,
			      const uint16_t *data_src)
{
	int ret;
	int i;
	int query_length;
	int byte_count;

	uint8_t query[MAX_MESSAGE_LENGTH];

	if (nb > MAX_REGISTERS) {
		printf("ERROR Trying to write to too many registers (%d > %d)\n",
		       nb, MAX_REGISTERS);
		return TOO_MANY_DATA;
	}

	query_length = build_query_basis(mb_param, slave,
					 FC_PRESET_MULTIPLE_REGISTERS,
					 start_addr, nb, query);
	byte_count = nb * 2;
	query[query_length++] = byte_count;

	for (i = 0; i < nb; i++) {
		query[query_length++] = data_src[i] >> 8;
		query[query_length++] = data_src[i] & 0x00FF;
	}

	ret = modbus_send(mb_param, query, query_length);
	if (ret > 0) {
		uint8_t response[MAX_MESSAGE_LENGTH];
		ret = modbus_receive(mb_param, query, response);
	}

	return ret;
}

/* Returns the slave id! */
int report_slave_id(modbus_param_t *mb_param, int slave,
		    uint8_t *data_dest)
{
	int ret;
	int query_length;
	uint8_t query[MIN_QUERY_LENGTH];

	query_length = build_query_basis(mb_param, slave, FC_REPORT_SLAVE_ID,
					 0, 0, query);

	/* HACKISH, start_addr and count are not used */
	query_length -= 4;

	ret = modbus_send(mb_param, query, query_length);
	if (ret > 0) {
		int i;
		int offset;
		int offset_length;
		uint8_t response[MAX_MESSAGE_LENGTH];

		/* Byte count, slave id, run indicator status,
		   additional data */
		ret = modbus_receive(mb_param, query, response);
		if (ret < 0)
			return ret;

		offset = mb_param->header_length;
		offset_length = offset + ret;

		for (i = offset; i < offset_length; i++)
			data_dest[i] = response[i];
	}

	return ret;
}

/*custom command used in SZHC*/
int hc_origin_check(modbus_param_t *mb_param, int slave, uint16_t *dest)
{
    int status = read_registers(mb_param, slave, FC_HC_ORIGIN_CHECK, 0, 0, dest);
    return status;
}

int hc_cmd_execute(modbus_param_t *mb_param, int slave, int cmdCode, uint16_t *dest)
{
    int status = read_registers(mb_param, slave, FC_HC_CMD_EXECUTE, cmdCode, 0, dest);
    return status;
}

int hc_test_act(modbus_param_t *mb_param, int slave, int cmdCode, uint16_t *dest)
{
    int status = read_registers(mb_param, slave, FC_HC_TEST_ACT, cmdCode, 0, dest);
    return status;
}

int hc_act_execute(modbus_param_t *mb_param,
                   int slave,
                   int step,
                   uint16_t *dest)
{
    int status = read_registers(mb_param, slave, FC_HC_ACT_EXECUTE, step, 0, dest);

    return status;
}

int hc_upload_act(modbus_param_t *mb_param,
                  int slave,
                  int cmdType,
                  int step,
                  int cmdCode,
                  int param1,
                  int param2,
                  int param3,
                  uint16_t* dest)
{
    int status = hc_upload_act_query(mb_param,
                                     slave,
                                     cmdType,
                                     step,
                                     cmdCode,
                                     param1,
                                     param2,
                                     param3,
                                     dest);
    return status;
}

int hc_download_act(modbus_param_t *mb_param, int slave, int step, uint16_t *dest)
{
    int status = read_registers(mb_param, slave, FC_HC_DOWNLOAD_ACT, step, 0, dest);
    return status;
}

int hc_runtime_modify(modbus_param_t *mb_param,
                      int slave,
                      int step,
                      int param1,
                      int param2,
                      int param3,
                      uint16_t *dest)
{
    int status = hc_runtime_modify_query(mb_param,
                                         slave,
                                         step,
                                         param1,
                                         param2,
                                         param3,
                                         dest);
    return status;
}

int hc_init_parameters(modbus_param_t *mb_param, int slave, int start_addr, int nb, const uint8_t *data)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int i;
    int query_length;

    uint8_t query[16];

    if (nb > 4) {
        printf("ERROR Trying to write to too many registers (%d > %d)\n",
               nb, 4);
        return TOO_MANY_DATA;
    }

    query_length = 6;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_INIT_PARA;
    query[2] = start_addr & 0x00FF;
    query[3] = start_addr >> 8;
    query[4] = nb & 0x00FF;
    query[5] = nb >> 8;

    for (i = 0; i < (nb << 1); i++) {
        query[query_length++] = data[i];
    }

//    for(i = 0; i != 16; ++i)
//    {
//        printf("query %d = %d \n",i ,query[i]);
//    }
//    return;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        ret = modbus_receive(mb_param, query, response);
    }

    return ret;
}

int hc_query_status(modbus_param_t *mb_param, int slave, int start_addr, int nb, uint16_t *dest)
{
    uint8_t response[MAX_MESSAGE_LENGTH];
    uint8_t query[8];
    fd_set readFD;
    FD_ZERO(&readFD);
    FD_SET(mb_param->fd, &readFD);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int ret = select(mb_param->fd + 1, &readFD, NULL, NULL, &tv);
    if(ret == 0)
    {
//        printf("Query Timeout!!\n");
        ret = -1;
        goto Send;
    }
    ret = read(mb_param->fd, response, MAX_MESSAGE_LENGTH);
    if(ret <= 0)
    {
        ret = -1;
        goto Send;
    }
    uint16_t crcCal;
    int length;
    if(response[2] == 0x16)
    {
        crcCal = crc16(response, 3);
        length = 3;
    }
    else
    {
        if(ret != 16)
        {
            ret = -1;
            goto Send;
        }
        crcCal = crc16(response, 14);
        length = 14;
    }
    uint16_t crcRec = (response[length] << 8) | response[length + 1];
    if(crcCal != crcRec)
    {
        ret = -1;
        goto Send;
    }
    if(length == 14)
    {
        int index = 0;
        for(int i = 0; i != 8; ++i)
        {
            dest[index++] = (response[6 + i + 1] << 8) | response[6 + i++];
        }
        ret = nb;
    }
    else
    {
        ret = response[2];
    }

#ifdef HC_5AXIS
    start_addr = (++start_addr) % 10;
#elif defined HC_8AXIS
    start_addr = (++start_addr) % 9;
#else
    start_addr = (++start_addr) % 7;
#endif

#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    Send:
    query[0] = slave;
    query[1] = FC_HC_QUERY_STATUS;
    query[2] = start_addr & 0x00FF;
    query[3] = start_addr >> 8;
    query[4] = nb & 0x00FF;
    query[5] = nb >> 8;
    modbus_send(mb_param, query, 6);
//    for(int i = 0; i != 8; ++i)
//    {
//        printf("query send[%d]:%d\n", i, query[i]);
//    }
//    printf("Send End *******************\n");
    return ret;

//    while ((ret = select(mb_param->fd+1, &readFD, NULL, NULL, &tv)) == -1)
//    {
//        if (errno == EINTR)
//        {
//            printf("A non blocked signal was caught\n");
//            /* Necessary after an error */
//            FD_ZERO(&readFD);
//            FD_SET(mb_param->fd, &readFD);
//        }
//        else
//        {
//            error_treat(mb_param, SELECT_FAILURE, "Select failure");
//            return SELECT_FAILURE;
//        }
//    }
//#ifndef NATIVE_WIN32
//    tcflush(mb_param->fd, TCIOFLUSH);
//#endif
//    int ret;
//    int query_length;

//    uint8_t query[8];

//    if (nb > 4) {
//        printf("ERROR Trying to write to too many registers (%d > %d)\n",
//               nb, 4);
//        return TOO_MANY_DATA;
//    }

//    query_length = 6;
////	byte_count = nb * 2;
////	query[query_length++] = byte_count;
//    query[0] = slave;
//    query[1] = FC_HC_QUERY_STATUS;
//    query[2] = start_addr & 0x00FF;
//    query[3] = start_addr >> 8;
//    query[4] = nb & 0x00FF;
//    query[5] = nb >> 8;
////    mb_param->debug = 1;


//    ret = modbus_send(mb_param, query, query_length);
////    if (mb_param->debug)
////    {

////        printf("Waiting for a message (%d bytes)...\n",
////               16);
////    }
//    if (ret > 0) {
//        uint8_t response[MAX_MESSAGE_LENGTH];
//        fd_set readFD;
//        FD_ZERO(&readFD);
//        FD_SET(mb_param->fd, &readFD);

//        struct timeval tv;
//        tv.tv_sec = 0;
//        tv.tv_usec = 10000;
//        while ((ret = select(mb_param->fd+1, &readFD, NULL, NULL, &tv)) == -1)
//        {
//            if (errno == EINTR)
//            {
//                printf("A non blocked signal was caught\n");
//                /* Necessary after an error */
//                FD_ZERO(&readFD);
//                FD_SET(mb_param->fd, &readFD);
//            }
//            else
//            {
//                error_treat(mb_param, SELECT_FAILURE, "Select failure");
//                return SELECT_FAILURE;
//            }
//        }
//        if(ret == 0)
//        {
//            return -1;
//        }
////        do
////        {
////            ret = select(mb_param->fd + 1,
////                         &readFD,
////                         NULL,
////                         NULL,
////                         &tv);
////        }while(ret < 0 && (errno == EINTR));
////        if(ret <= 0)
////        {
////            return ret;
////        }
//        ret = read(mb_param->fd, response, 16);
//        if(ret <= 0)
//        {
//            return -1;
//        }
////        if (mb_param->debug) {
////            int i;
////            for (i=0; i < ret; i++)
////                printf("<%.2X>", response[i]);
////        }
//        uint16_t crcCal;
//        int length;
//        if(response[2] == 0x17)
//        {
//            crcCal = crc16(response, 3);
//            length = 3;
//        }
//        else
//        {
//            if(ret != 16)
//            {
//                return -1;
//            }
//            crcCal = crc16(response, 14);
//            length = 14;
//        }
//        uint16_t crcRec = (response[length] << 8) | response[length + 1];
//        if(crcCal != crcRec)
//        {
//            return -1;
//        }
//        if(length == 14)
//        {
//            int index = 0;
//            for(int i = 0; i != 8; ++i)
//            {
//                dest[index++] = (response[query_length + i + 1] << 8) | response[query_length + i++];
//            }
//            ret = nb;
//        }
//        else
//        {
//            ret = 0x17;
//        }
//    }
//    return ret;


//        ret = modbus_receive(mb_param, query, response);

//        printf("ret = %d \n", ret);
//        exit(-1);
//        if(ret == (nb << 1))
//        {
//            int i;
//            int index = 0;
//            for(i = 0; i != ret; ++i)
//            {
//                dest[index++] = (response[query_length + i + 1] << 8) | response[query_length + i++];
//            }
//            ret = nb;
//        }
//    }
//    return ret;
}

int hc_teach_step(modbus_param_t *mb_param, int slave, int step, uint8_t *dest)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[7];

//    if (nb > 4) {
//        printf("ERROR Trying to write to too many registers (%d > %d)\n",
//               nb, 4);
//        return TOO_MANY_DATA;
//    }

    query_length = 5;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_TEACH_STEP;
    query[2] = CMDTEACH;
    query[3] = step & 0x00FF;
    query[4] = 0;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        ret = modbus_receive(mb_param, query, response);
        if(ret == 16)
        {
            for(int i = 0; i != 8; ++i)
            {
                dest[i] = response[6 + i];
            }
            dest[8] = response[2] & 0X00FF;
            return 9;
        }
    }
    return -1;
}

int hc_command(modbus_param_t *mb_param, int slave, int cmd, int keyvalue, int act, int sum)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[7];

//    if (nb > 4) {
//        printf("ERROR Trying to write to too many registers (%d > %d)\n",
//               nb, 4);
//        return TOO_MANY_DATA;
//    }

    query_length = 5;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_COMMAND;
    query[2] = cmd;
    switch(cmd)
    {
    case CMDPULSEA:
    case CMDPULSEB:
    {
        query[3] = keyvalue;
        query[4] = keyvalue >> 8;
    }
        break;
    case CMDTURNTEACH:
    case CMDTURNAUTO:
    case CMDTURNTCHSUB0:
    case CMDTURNTCHSUB1:
    case CMDTURNTCHSUB2:
    case CMDTURNTCHSUB3:
    case CMDTURNTCHSUB4:
    case CMDTURNTCHSUB5:
    case CMDTURNTCHSUB6:
    case CMDTURNTCHSUB7:
    case CMDTURNTCHSUB8:
        {
            query[3] = act & 0x00FF;
            query[4] = sum & 0x00FF;
        }
        break;
    default:
        {
            query[3] = keyvalue;
            query[4] = 0;
        }
    }
//    mb_param->debug = 1;

    ret = modbus_send(mb_param, query, query_length);
//    mb_param->debug = 0;
//    exit(-1);
    if (ret < 0)
    {
        return -1;
    }
    uint8_t response[MAX_MESSAGE_LENGTH];
    ret = modbus_receive(mb_param, query, response);
    if(ret != query_length)
    {
        return -1;
    }
    int i;
//    int index = 0;
    for(i = 0; i != ret; ++i)
    {
        if(query[i] != response[i])
        {
            return -1;
        }
    }
    return 0;
}

int hc_modify_sys_parameter(modbus_param_t *mb_param, int slave, int start_addr, int value)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[8];

    query_length = 6;

    query[0] = slave;
    query[1] = FC_HC_MODIFY_SYS_PARA;
    query[2] = start_addr & 0x00FF;
    query[3] = start_addr >> 8;
    query[4] = value & 0x00FF;
    query[5] = value >> 8;

    ret = modbus_send(mb_param, query, query_length);
    if (ret < 0)
    {
        return -1;
    }
    uint8_t response[MAX_MESSAGE_LENGTH];
    ret = modbus_receive(mb_param, query, response);
    if(ret != query_length)
    {
        return -1;
    }
    int i;
//    int index = 0;
    for(i = 0; i != ret; ++i)
    {
        if(query[i] != response[i])
        {
            return -1;
        }
    }
    return 0;
}

int hc_teach_parameter(modbus_param_t *mb_param, int slave, int start_addr, int value)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[8];

    query_length = 6;

    query[0] = slave;
    query[1] = FC_HC_TEACH_PARA;
    query[2] = start_addr & 0x00FF;
    query[3] = start_addr >> 8;
    query[4] = value & 0x00FF;
    query[5] = value >> 8;

    ret = modbus_send(mb_param, query, query_length);
    if (ret < 0)
    {
        return -1;
    }
    uint8_t response[MAX_MESSAGE_LENGTH];
    ret = modbus_receive(mb_param, query, response);
    if(ret != query_length)
    {
        return -1;
    }
    int i;
//    int index = 0;
    for(i = 0; i != ret; ++i)
    {
        if(query[i] != response[i])
        {
            return -1;
        }
    }
    return 0;
}

int hc_auto_adj(modbus_param_t *mb_param, int slave, int seq, int delay, int speed, int dpos, int gmValue, int sum)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int i;
    int query_length;

    uint8_t query[16];

    query_length = 14;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_AUTO_ADJ;
    query[2] = seq & 0x00FF;
    query[3] = seq >> 8;
    query[4] = 4;
    query[5] = 0;
    query[6] = delay & 0x00FF;
    query[7] = delay >> 8;
    query[8] = speed;
    query[9] = dpos;
    query[10] = gmValue;
    query[11] = sum;
    query[12] = 0;
    query[13] = 0;

//    printf("delay:%d speed:%d dpos:%d gmvalue:%d sum%d \n", query[6],query[7], query[8], query[9], query[10]);
    ret = modbus_send(mb_param, query, query_length);
    if (ret < 0)
    {
        return -1;
    }
    uint8_t response[MAX_MESSAGE_LENGTH];
    ret = modbus_receive(mb_param, query, response);
    if(ret != 14)
    {
        return -1;
    }

    for(i = 0; i != 14; ++i)
    {
        if(query[i] != response[i])
        {
            return -1;
        }
    }

    return 0;
}

int hc_does_need_teach_step(modbus_param_t *mb_param, int slave, int start_addr, int nb)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[8];


    query_length = 6;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_QUERY_STATUS;
    query[2] = start_addr & 0x00FF;
    query[3] = start_addr >> 8;
    query[4] = nb & 0x00FF;
    query[5] = nb >> 8;
//    mb_param->debug = 1;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
//        ret = modbus_receive(mb_param, query, response);
//        printf("ret = %d \n", ret);
//        exit(-1);
        int response_length = 0;

        ret = receive_msg(mb_param, 16,
                          response, &response_length, TIME_OUT_DEFAULT);
        if(ret == 0)
        {
            if(response[2] == 0x17)
            {
                return 1;
            }
            ret = -1;
        }
    }

    return ret;
}

int hc_does_need_reconfig(modbus_param_t *mb_param, int slave, int start_addr, int nb)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[8];


    query_length = 6;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_QUERY_STATUS;
    query[2] = start_addr & 0x00FF;
    query[3] = start_addr >> 8;
    query[4] = nb & 0x00FF;
    query[5] = nb >> 8;
//    mb_param->debug = 1;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
//        ret = modbus_receive(mb_param, query, response);
//        printf("ret = %d \n", ret);
//        exit(-1);
        int response_length = 0;

        ret = receive_msg(mb_param, 16,
                          response, &response_length, TIME_OUT_DEFAULT);
        if(ret == 0)
        {
            if(response[2] == 0x16)
            {
                return 1;
            }
            ret = -1;
        }
    }

    return ret;
}

int hc_get_axis_parameter(modbus_param_t *mb_param, int slave, int axis, uint16_t *dest)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[7];

//    if (nb > 4) {
//        printf("ERROR Trying to write to too many registers (%d > %d)\n",
//               nb, 4);
//        return TOO_MANY_DATA;
//    }

    query_length = 5;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_GET_AXIS_PARA;
    query[2] = 0;
    query[3] = axis >> 8;
    query[4] = axis & 0x00FF;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        ret = modbus_receive(mb_param, query, response);
        if(ret == 16)
        {
//            for(int i = 0; i != 8; ++i)
//            {
//                dest[i] = (response[6 + i + 1] << 8) | response[6 + i];
//                ++i;
//            }
            dest[0] = (response[7]  << 8) | response[6];
            dest[1] = (response[9]  << 8) | response[8];
            dest[2] = (response[11] << 8) | response[10];
            dest[3] = (response[13] << 8) | response[12];
            return 4;
        }
    }
    return -1;
}

int hc_select_axis_config(modbus_param_t *mb_param, int slave, int selected)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[7];

//    if (nb > 4) {
//        printf("ERROR Trying to write to too many registers (%d > %d)\n",
//               nb, 4);
//        return TOO_MANY_DATA;
//    }

    query_length = 5;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_SELECT_CONFIG;
    query[2] = 1;
    query[3] = selected >> 8;
    query[4] = selected & 0x00FF;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        ret = modbus_receive(mb_param, query, response);
        if(ret == 7)
        {
            for(int i = 0; i != 7; ++i)
            {
                if(query[i] != response[i])
                {
                    return -1;
                }
            }
            return 1;
        }
    }
    return -1;
}

int hc_set_axis_parameter(modbus_param_t *mb_param, int slave, int frame, int axis, uint16_t *configs)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[16];

    query_length = 14;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = FC_HC_SET_AXIS_PARA;
    query[2] = frame;
    query[3] = 0;
    query[4] = axis;
    query[5] = 0;
    query[7] = configs[0] >> 8;
    query[6] = configs[0] & 0x00FF;
    query[9] = configs[1] >> 8;
    query[8] = configs[1] & 0x00FF;
    query[11] = configs[2] >> 8;
    query[10] = configs[2] & 0x00FF;
    query[13] = configs[3] >> 8;
    query[12] = configs[3] & 0x00FF;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        ret = modbus_receive(mb_param, query, response);
        if(ret == 16)
        {
            for(int i = 0; i != 16; ++i)
            {
                printf("response[%d] = %d query[%d] = %d \n", i, response[i], i, query[i]);
                if(response[i] != query[i])
                {
                    return -1;
                }
            }
            return 1;
        }
    }
    return -1;
}

int hc_update_host_req(modbus_param_t *mb_param)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[5];

    query_length = 3;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = 0x55;
    query[1] = 0xAA;
    query[2] = 0x51;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        fd_set readFD;
        FD_ZERO(&readFD);
        FD_SET(mb_param->fd, &readFD);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        int ret = select(mb_param->fd + 1, &readFD, NULL, NULL, &tv);
        if(ret == 0)
        {
            return -1;
        }
        ret = read(mb_param->fd, response, MAX_MESSAGE_LENGTH);
        if(ret <= 0)
        {
            return -1;
        }
        uint16_t crcCal = crc16(response, 3);
        uint16_t crcRec = (response[3] << 8) | response[4];
        if(crcCal != crcRec)
        {
            return -1;
        }
        for(int i = 0; i != 5; ++i)
        {
            printf("send[%d] = %d  rec[%d] = %d \n", i, query[i], i, response[i]);
            if(query[i] != response[i])
            {
                return -1;
            }
        }
        return 1;
    }
    return -1;
}

int hc_update_host_transfer(modbus_param_t *mb_param, int addr, char *data)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[39];

    query_length = 37;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = 0x55;
    query[1] = 0xAA;
    query[2] = 0x52;
    query[3] = addr & 0X00FF;
    query[4] = addr >> 8;
    memcpy(&query[5], data, 32);

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        fd_set readFD;
        FD_ZERO(&readFD);
        FD_SET(mb_param->fd, &readFD);

        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 10000;
        int ret = select(mb_param->fd + 1, &readFD, NULL, NULL, &tv);
        if(ret == 0)
        {
            printf("ReadTimeOut\n");
            return -1;
        }
        ret = read(mb_param->fd, response, MAX_MESSAGE_LENGTH);
        if(ret <= 0)
        {
            printf("ReadError\n");
            return -1;
        }
        uint16_t crcCal = crc16(response, 7);
        uint16_t crcRec = (response[7] << 8) | response[8];
        printf("crc = %d crcRec = %d\n", crcCal, crcRec);
        int sum = 0;
        for(int i = 0; i != 32; ++i)
        {
            sum += query[5 + i + 1] << 8 | query[5 + i];
            ++i;
        }
        sum &= 0x0000FFFF;
        int recSum = (response[6] << 8) | response[5];
        printf("sum = %d recSum = %d\n", sum, recSum);
        if(crcCal != crcRec)
        {
            return -1;
        }
        for(int i = 0; i != 5; ++i)
        {
//            printf("send[%d] = %d  rec[%d] = %d \n", i, query[i], i, response[i]);
            if(query[i] != response[i])
            {
                return -1;
            }
        }
        return sum == recSum;
    }
    return -1;
}

int hc_update_host_finish(modbus_param_t *mb_param)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[5];

    query_length = 3;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = 0x55;
    query[1] = 0xAA;
    query[2] = 0x53;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        fd_set readFD;
        FD_ZERO(&readFD);
        FD_SET(mb_param->fd, &readFD);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        int ret = select(mb_param->fd + 1, &readFD, NULL, NULL, &tv);
        if(ret == 0)
        {
            return -1;
        }
        ret = read(mb_param->fd, response, MAX_MESSAGE_LENGTH);
        if(ret <= 0)
        {
            return -1;
        }
        uint16_t crcCal = crc16(response, 3);
        uint16_t crcRec = (response[3] << 8) | response[4];
        if(crcCal != crcRec)
        {
            return -1;
        }
        for(int i = 0; i != 5; ++i)
        {
            printf("send[%d] = %d  rec[%d] = %d \n", i, query[i], i, response[i]);
            if(query[i] != response[i])
            {
                return -1;
            }
        }
        return 1;
    }
    return -1;
}

int hc_update_host_restart(modbus_param_t *mb_param)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[5];

    query_length = 3;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = 0x55;
    query[1] = 0xAA;
    query[2] = 0x55;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        fd_set readFD;
        FD_ZERO(&readFD);
        FD_SET(mb_param->fd, &readFD);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        int ret = select(mb_param->fd + 1, &readFD, NULL, NULL, &tv);
        if(ret == 0)
        {
            printf("111111111111111\n");
            return -1;
        }
        ret = read(mb_param->fd, response, MAX_MESSAGE_LENGTH);
        if(ret <= 0)
        {
            printf("222222222222222\n");
            return -1;
        }
        uint16_t crcCal = crc16(response, 3);
        uint16_t crcRec = (response[3] << 8) | response[4];
        if(crcCal != crcRec)
        {
            printf("333333333333\n");
            return -1;
        }
        for(int i = 0; i != 5; ++i)
        {
            printf("send[%d] = %d  rec[%d] = %d \n", i, query[i], i, response[i]);
            if(query[i] != response[i])
            {
                printf("4444444444444\n");
                return -1;
            }
        }
        return 1;
    }
    printf("555555555555\n");
    return -1;
}

int hc_update_host_query(modbus_param_t *mb_param)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[5];

    query_length = 3;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = 0x55;
    query[1] = 0xAA;
    query[2] = 0x54;

    ret = modbus_send(mb_param, query, query_length);
    if (ret > 0) {
        uint8_t response[MAX_MESSAGE_LENGTH];
        fd_set readFD;
        FD_ZERO(&readFD);
        FD_SET(mb_param->fd, &readFD);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
        int ret = select(mb_param->fd + 1, &readFD, NULL, NULL, &tv);
        if(ret == 0)
        {
            return -1;
        }
        ret = read(mb_param->fd, response, MAX_MESSAGE_LENGTH);
        if(ret <= 0)
        {
            return -1;
        }
        for(int i = 0; i != 6; ++i)
        {
            printf("query[%d] = %d\n", i, response[i]);
        }
        uint16_t crcCal = crc16(response, 4);
        uint16_t crcRec = (response[4] << 8) | response[5];
        if(crcCal != crcRec)
        {
            return -1;
        }
        return response[3];
    }
    return -1;
}

int hc_update_host_start(modbus_param_t *mb_param, int slave)
{
#ifndef NATIVE_WIN32
    tcflush(mb_param->fd, TCIOFLUSH);
#endif
    int ret;
    int query_length;

    uint8_t query[8];

    query_length = 6;
//	byte_count = nb * 2;
//	query[query_length++] = byte_count;
    query[0] = slave;
    query[1] = 0x55;
    query[2] = 0x10;
    query[3] = 0x10;
    query[4] = 0x11;
    query[5] = 0x20;

    ret = modbus_send(mb_param, query, query_length);
    return ret;
}


/* Initializes the modbus_param_t structure for RTU
   - device: "/dev/ttyS0"
   - baud:   9600, 19200, 57600, 115200, etc
   - parity: "even", "odd" or "none"
   - data_bits: 5, 6, 7, 8
   - stop_bits: 1, 2
*/
void modbus_init_rtu(modbus_param_t *mb_param, const char *device,
		     int baud, const char *parity, int data_bit,
		     int stop_bit)
{
	memset(mb_param, 0, sizeof(modbus_param_t));
	strcpy(mb_param->device, device);
	mb_param->baud = baud;
	strcpy(mb_param->parity, parity);
	mb_param->debug = FALSE;
	mb_param->data_bit = data_bit;
	mb_param->stop_bit = stop_bit;
	mb_param->type_com = RTU;
	mb_param->header_length = HEADER_LENGTH_RTU;
	mb_param->checksum_length = CHECKSUM_LENGTH_RTU;
}

/* Initializes the modbus_param_t structure for TCP.
   - ip : "192.168.0.5"
   - port : 1099

   Set the port to MODBUS_TCP_DEFAULT_PORT to use the default one
   (502). It's convenient to use a port number greater than or equal
   to 1024 because it's not necessary to be root to use this port
   number.
*/
void modbus_init_tcp(modbus_param_t *mb_param, const char *ip, int port)
{
	memset(mb_param, 0, sizeof(modbus_param_t));
	strncpy(mb_param->ip, ip, sizeof(char)*16);
	mb_param->port = port;
	mb_param->type_com = TCP;
	mb_param->header_length = HEADER_LENGTH_TCP;
	mb_param->checksum_length = CHECKSUM_LENGTH_TCP;
	mb_param->error_handling = FLUSH_OR_RECONNECT_ON_ERROR;
}

/* By default, the error handling mode used is FLUSH_OR_RECONNECT_ON_ERROR.

   With FLUSH_OR_RECONNECT_ON_ERROR, the library will flush to I/O
   port in RTU mode or attempt an immediate reconnection which may
   hang for several seconds if the network to the remote target unit
   is down in TCP mode.

   With NOP_ON_ERROR, it is expected that the application will
   check for error returns and deal with them as necessary.
*/
void modbus_set_error_handling(modbus_param_t *mb_param,
			       error_handling_t error_handling)
{
	if (error_handling == FLUSH_OR_RECONNECT_ON_ERROR ||
	    error_handling == NOP_ON_ERROR) {
		mb_param->error_handling = error_handling;
	} else {
		printf("Invalid setting for error handling (not changed)\n");
	}
}


/* Sets up a serial port for RTU communications */
#ifdef NATIVE_WIN32
static int modbus_connect_rtu(modbus_param_t *mb_param)
{
		DCB dcb;

		if (mb_param->debug) {
				printf("Opening %s at %d bauds (%s)\n",
						mb_param->device, mb_param->baud, mb_param->parity);
		}

		/* Some references here:
		 * http://msdn.microsoft.com/en-us/library/aa450602.aspx
		 */

		/* Open serial port */
		win32_ser_init(&mb_param->w_ser);
		/* mb_param->device should contain a string like "COMxx:" xx being a decimal number */
		mb_param->w_ser.fd = CreateFileA(mb_param->device,
										GENERIC_READ | GENERIC_WRITE,
										0,
										NULL,
										OPEN_EXISTING,
										0,
										NULL);

		/* Error checking */
		if (mb_param->w_ser.fd == INVALID_HANDLE_VALUE) {
				perror("CreateFile");
				printf("ERROR Can't open the device %s (LastError %d)\n",
						mb_param->device, GetLastError());
				return -1;
		}

		/* Save params */
		mb_param->old_dcb.DCBlength = sizeof(DCB);
		if (!GetCommState(mb_param->w_ser.fd, &mb_param->old_dcb)) {
				perror("GetCommState");
				printf ("ERROR Error getting configuration (LastError %d)\n",
						GetLastError());
				return -1;
		}

		/* Build new configuration (starting from current settings) */
		dcb = mb_param->old_dcb;

		/* Speed setting */
		switch (mb_param->baud) {
		case 110:
				dcb.BaudRate = CBR_110;
				break;
		case 300:
				dcb.BaudRate = CBR_300;
				break;
		case 600:
				dcb.BaudRate = CBR_600;
				break;
		case 1200:
				dcb.BaudRate = CBR_1200;
				break;
		case 2400:
				dcb.BaudRate = CBR_2400;
				break;
		case 4800:
				dcb.BaudRate = CBR_4800;
				break;
		case 9600:
				dcb.BaudRate = CBR_9600;
				break;
		case 19200:
				dcb.BaudRate = CBR_19200;
				break;
		case 38400:
				dcb.BaudRate = CBR_38400;
				break;
		case 57600:
				dcb.BaudRate = CBR_57600;
				break;
		case 115200:
				dcb.BaudRate = CBR_115200;
				break;
		default:
				dcb.BaudRate = CBR_9600;
				printf("WARNING Unknown baud rate %d for %s (B9600 used)\n",
						mb_param->baud, mb_param->device);
		}

		/* Data bits */
		switch (mb_param->data_bit) {
		case 5:
				dcb.ByteSize = 5;
				break;
		case 6:
				dcb.ByteSize = 6;
				break;
		case 7:
				dcb.ByteSize = 7;
				break;
		case 8:
				default:
				dcb.ByteSize = 8;
				break;
		}

		/* Stop bits */
		if (mb_param->stop_bit == 1)
				dcb.StopBits = ONESTOPBIT;
		else /* 2 */
				dcb.StopBits = TWOSTOPBITS;

		/* Parity */
		if (strncmp(mb_param->parity, "none", 4) == 0) {
				dcb.Parity = NOPARITY;
				dcb.fParity = FALSE;
		} else if (strncmp(mb_param->parity, "even", 4) == 0) {
				dcb.Parity = EVENPARITY;
				dcb.fParity = TRUE;
		} else {
				/* odd */
				dcb.Parity = ODDPARITY;
				dcb.fParity = TRUE;
		}

		/* Hardware handshaking left as default settings retrieved */

		/* No software handshaking */
		dcb.fTXContinueOnXoff = TRUE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;

		/* Binary mode (it's the only supported on Windows anyway) */
		dcb.fBinary = TRUE;

		/* Don't want errors to be blocking */
		dcb.fAbortOnError = FALSE;

		/* TODO: any other flags !? */

		/* Setup port */
		if (!SetCommState(mb_param->w_ser.fd, &dcb)) {
				perror("SetCommState\n");
				return -1;
		}

		return 0;
}
#else /* NATIVE_WIN32 */
static int modbus_connect_rtu(modbus_param_t *mb_param)
{
	struct termios tios;
	speed_t speed;

	if (mb_param->debug) {
		printf("Opening %s at %d bauds (%s)\n",
		       mb_param->device, mb_param->baud, mb_param->parity);
	}

	/* The O_NOCTTY flag tells UNIX that this program doesn't want
	   to be the "controlling terminal" for that port. If you
	   don't specify this then any input (such as keyboard abort
	   signals and so forth) will affect your process

	   Timeouts are ignored in canonical input mode or when the
	   NDELAY option is set on the file via open or fcntl */
	mb_param->fd = open(mb_param->device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (mb_param->fd < 0) {
		perror("open");
		printf("ERROR Can't open the device %s (errno %d)\n",
		       mb_param->device, errno);
		return -1;
	}

	/* Save */
	tcgetattr(mb_param->fd, &(mb_param->old_tios));

	memset(&tios, 0, sizeof(struct termios));

	/* C_ISPEED     Input baud (new interface)
	   C_OSPEED     Output baud (new interface)
	*/
	switch (mb_param->baud) {
	case 110:
		speed = B110;
		break;
	case 300:
		speed = B300;
		break;
	case 600:
		speed = B600;
		break;
	case 1200:
		speed = B1200;
		break;
	case 2400:
		speed = B2400;
		break;
	case 4800:
		speed = B4800;
		break;
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	case 38400:
		speed = B38400;
		break;
	case 57600:
		speed = B57600;
		break;
	case 115200:
		speed = B115200;
		break;
	default:
		speed = B9600;
		printf("WARNING Unknown baud rate %d for %s (B9600 used)\n",
		       mb_param->baud, mb_param->device);
	}

	/* Set the baud rate */
	if ((cfsetispeed(&tios, speed) < 0) ||
	    (cfsetospeed(&tios, speed) < 0)) {
		perror("cfsetispeed/cfsetospeed\n");
		return -1;
	}

	/* C_CFLAG      Control options
	   CLOCAL       Local line - do not change "owner" of port
	   CREAD        Enable receiver
	*/
	tios.c_cflag |= (CREAD | CLOCAL);
	/* CSIZE, HUPCL, CRTSCTS (hardware flow control) */

	/* Set data bits (5, 6, 7, 8 bits)
	   CSIZE        Bit mask for data bits
	*/
	tios.c_cflag &= ~CSIZE;
	switch (mb_param->data_bit) {
	case 5:
		tios.c_cflag |= CS5;
		break;
	case 6:
		tios.c_cflag |= CS6;
		break;
	case 7:
		tios.c_cflag |= CS7;
		break;
	case 8:
	default:
		tios.c_cflag |= CS8;
		break;
	}

	/* Stop bit (1 or 2) */
	if (mb_param->stop_bit == 1)
		tios.c_cflag &=~ CSTOPB;
	else /* 2 */
		tios.c_cflag |= CSTOPB;

	/* PARENB       Enable parity bit
	   PARODD       Use odd parity instead of even */
	if (strncmp(mb_param->parity, "none", 4) == 0) {
		tios.c_cflag &=~ PARENB;
	} else if (strncmp(mb_param->parity, "even", 4) == 0) {
		tios.c_cflag |= PARENB;
		tios.c_cflag &=~ PARODD;
	} else {
		/* odd */
		tios.c_cflag |= PARENB;
		tios.c_cflag |= PARODD;
	}

	/* Read the man page of termios if you need more information. */

	/* This field isn't used on POSIX systems
	   tios.c_line = 0;
	*/

	/* C_LFLAG      Line options

	   ISIG Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
	   ICANON       Enable canonical input (else raw)
	   XCASE        Map uppercase \lowercase (obsolete)
	   ECHO Enable echoing of input characters
	   ECHOE        Echo erase character as BS-SP-BS
	   ECHOK        Echo NL after kill character
	   ECHONL       Echo NL
	   NOFLSH       Disable flushing of input buffers after
	   interrupt or quit characters
	   IEXTEN       Enable extended functions
	   ECHOCTL      Echo control characters as ^char and delete as ~?
	   ECHOPRT      Echo erased character as character erased
	   ECHOKE       BS-SP-BS entire line on line kill
	   FLUSHO       Output being flushed
	   PENDIN       Retype pending input at next read or input char
	   TOSTOP       Send SIGTTOU for background output

	   Canonical input is line-oriented. Input characters are put
	   into a buffer which can be edited interactively by the user
	   until a CR (carriage return) or LF (line feed) character is
	   received.

	   Raw input is unprocessed. Input characters are passed
	   through exactly as they are received, when they are
	   received. Generally you'll deselect the ICANON, ECHO,
	   ECHOE, and ISIG options when using raw input
	*/

	/* Raw input */
	tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* C_IFLAG      Input options

	   Constant     Description
	   INPCK        Enable parity check
	   IGNPAR       Ignore parity errors
	   PARMRK       Mark parity errors
	   ISTRIP       Strip parity bits
	   IXON Enable software flow control (outgoing)
	   IXOFF        Enable software flow control (incoming)
	   IXANY        Allow any character to start flow again
	   IGNBRK       Ignore break condition
	   BRKINT       Send a SIGINT when a break condition is detected
	   INLCR        Map NL to CR
	   IGNCR        Ignore CR
	   ICRNL        Map CR to NL
	   IUCLC        Map uppercase to lowercase
	   IMAXBEL      Echo BEL on input line too long
	*/
	if (strncmp(mb_param->parity, "none", 4) == 0) {
		tios.c_iflag &= ~INPCK;
	} else {
		tios.c_iflag |= INPCK;
	}

	/* Software flow control is disabled */
	tios.c_iflag &= ~(IXON | IXOFF | IXANY);

	/* C_OFLAG      Output options
	   OPOST        Postprocess output (not set = raw output)
	   ONLCR        Map NL to CR-NL

	   ONCLR ant others needs OPOST to be enabled
	*/

	/* Raw ouput */
	tios.c_oflag &=~ OPOST;

	/* C_CC         Control characters
	   VMIN         Minimum number of characters to read
	   VTIME        Time to wait for data (tenths of seconds)

	   UNIX serial interface drivers provide the ability to
	   specify character and packet timeouts. Two elements of the
	   c_cc array are used for timeouts: VMIN and VTIME. Timeouts
	   are ignored in canonical input mode or when the NDELAY
	   option is set on the file via open or fcntl.

	   VMIN specifies the minimum number of characters to read. If
	   it is set to 0, then the VTIME value specifies the time to
	   wait for every character read. Note that this does not mean
	   that a read call for N bytes will wait for N characters to
	   come in. Rather, the timeout will apply to the first
	   character and the read call will return the number of
	   characters immediately available (up to the number you
	   request).

	   If VMIN is non-zero, VTIME specifies the time to wait for
	   the first character read. If a character is read within the
	   time given, any read will block (wait) until all VMIN
	   characters are read. That is, once the first character is
	   read, the serial interface driver expects to receive an
	   entire packet of characters (VMIN bytes total). If no
	   character is read within the time allowed, then the call to
	   read returns 0. This method allows you to tell the serial
	   driver you need exactly N bytes and any read call will
	   return 0 or N bytes. However, the timeout only applies to
	   the first character read, so if for some reason the driver
	   misses one character inside the N byte packet then the read
	   call could block forever waiting for additional input
	   characters.

	   VTIME specifies the amount of time to wait for incoming
	   characters in tenths of seconds. If VTIME is set to 0 (the
	   default), reads will block (wait) indefinitely unless the
	   NDELAY option is set on the port with open or fcntl.
	*/
	/* Unused because we use open with the NDELAY option */
	tios.c_cc[VMIN] = 0;
	tios.c_cc[VTIME] = 0;

	if (tcsetattr(mb_param->fd, TCSANOW, &tios) < 0) {
		perror("tcsetattr\n");
		return -1;
	}

	return 0;
}
#endif /* NATIVE_WIN32 */

/* Establishes a modbus TCP connection with a modbus slave */
#ifdef NATIVE_WIN32
static int modbus_connect_tcp(modbus_param_t *mb_param)
{
		return -1; /* TODO: ethernet support */
}
#else /* NATIVE_WIN32 */
static int modbus_connect_tcp(modbus_param_t *mb_param)
{
	int ret;
	int option;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(mb_param->port);
	addr.sin_addr.s_addr = inet_addr(mb_param->ip);

	mb_param->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (mb_param->fd < 0) {
		return mb_param->fd;
	}

	/* Set the TCP no delay flag */
	/* SOL_TCP = IPPROTO_TCP */
	option = 1;
	ret = setsockopt(mb_param->fd, IPPROTO_TCP, TCP_NODELAY,
			 (const void *)&option, sizeof(int));
	if (ret < 0) {
		perror("setsockopt");
		close(mb_param->fd);
		return ret;
	}

	/* Set the IP low delay option */
	option = IPTOS_LOWDELAY;
	ret = setsockopt(mb_param->fd, IPPROTO_TCP, IP_TOS,
			 (const void *)&option, sizeof(int));
	if (ret < 0) {
		perror("setsockopt");
		close(mb_param->fd);
		return ret;
	}

	if (mb_param->debug) {
		printf("Connecting to %s\n", mb_param->ip);
	}

	ret = connect(mb_param->fd, (struct sockaddr *)&addr,
		      sizeof(struct sockaddr_in));
	if (ret < 0) {
		perror("connect");
		close(mb_param->fd);
		return ret;
	}

	return 0;
}
#endif /* NATIVE_WIN32 */

/* Establishes a modbus connexion.
   Returns -1 if an error occured. */
int modbus_connect(modbus_param_t *mb_param)
{
	int ret;

	if (mb_param->type_com == RTU)
		ret = modbus_connect_rtu(mb_param);
	else
		ret = modbus_connect_tcp(mb_param);

	return ret;
}

/* Closes the file descriptor in RTU mode */
#ifdef NATIVE_WIN32
static void modbus_close_rtu(modbus_param_t *mb_param)
{
		/* Revert settings */
		if (!SetCommState(mb_param->w_ser.fd, &mb_param->old_dcb))
				perror("SetCommState");

		if (!CloseHandle(mb_param->w_ser.fd))
				perror("CloseHandle");

}
#else /* NATIVE_WIN32 */
static void modbus_close_rtu(modbus_param_t *mb_param)
{
	if (tcsetattr(mb_param->fd, TCSANOW, &(mb_param->old_tios)) < 0)
		perror("tcsetattr");

	close(mb_param->fd);
}
#endif /* NATIVE_WIN32 */

/* Closes the network connection and socket in TCP mode */
#ifdef NATIVE_WIN32
static void modbus_close_tcp(modbus_param_t *mb_param)
{
		/* TODO: ethernet support */
}
#else /* NATIVE_WIN32 */
static void modbus_close_tcp(modbus_param_t *mb_param)
{
	shutdown(mb_param->fd, SHUT_RDWR);
	close(mb_param->fd);
}
#endif /* NATIVE_WIN32 */

/* Closes a modbus connection */
void modbus_close(modbus_param_t *mb_param)
{
	if (mb_param->type_com == RTU)
		modbus_close_rtu(mb_param);
	else
		modbus_close_tcp(mb_param);
}

/* Activates the debug messages */
void modbus_set_debug(modbus_param_t *mb_param, int boolean)
{
	mb_param->debug = boolean;
}

/* Allocates 4 arrays to store coils, input status, input registers and
   holding registers. The pointers are stored in modbus_mapping structure.

   Returns: TRUE if ok, FALSE on failure
*/
int modbus_mapping_new(modbus_mapping_t *mb_mapping,
		       int nb_coil_status, int nb_input_status,
		       int nb_holding_registers, int nb_input_registers)
{
	/* 0X */
	mb_mapping->nb_coil_status = nb_coil_status;
	mb_mapping->tab_coil_status =
		(uint8_t *) malloc(nb_coil_status * sizeof(uint8_t));
	memset(mb_mapping->tab_coil_status, 0,
	       nb_coil_status * sizeof(uint8_t));
	if (mb_mapping->tab_coil_status == NULL)
		return FALSE;

	/* 1X */
	mb_mapping->nb_input_status = nb_input_status;
	mb_mapping->tab_input_status =
		(uint8_t *) malloc(nb_input_status * sizeof(uint8_t));
	memset(mb_mapping->tab_input_status, 0,
	       nb_input_status * sizeof(uint8_t));
	if (mb_mapping->tab_input_status == NULL) {
		free(mb_mapping->tab_coil_status);
		return FALSE;
	}

	/* 4X */
	mb_mapping->nb_holding_registers = nb_holding_registers;
	mb_mapping->tab_holding_registers =
		(uint16_t *) malloc(nb_holding_registers * sizeof(uint16_t));
	memset(mb_mapping->tab_holding_registers, 0,
	       nb_holding_registers * sizeof(uint16_t));
	if (mb_mapping->tab_holding_registers == NULL) {
		free(mb_mapping->tab_coil_status);
		free(mb_mapping->tab_input_status);
		return FALSE;
	}

	/* 3X */
	mb_mapping->nb_input_registers = nb_input_registers;
	mb_mapping->tab_input_registers =
		(uint16_t *) malloc(nb_input_registers * sizeof(uint16_t));
	memset(mb_mapping->tab_input_registers, 0,
	       nb_input_registers * sizeof(uint16_t));
	if (mb_mapping->tab_input_registers == NULL) {
		free(mb_mapping->tab_coil_status);
		free(mb_mapping->tab_input_status);
		free(mb_mapping->tab_holding_registers);
		return FALSE;
	}

	return TRUE;
}

/* Frees the 4 arrays */
void modbus_mapping_free(modbus_mapping_t *mb_mapping)
{
	free(mb_mapping->tab_coil_status);
	free(mb_mapping->tab_input_status);
	free(mb_mapping->tab_holding_registers);
	free(mb_mapping->tab_input_registers);
}

/* Listens for any query from a modbus master in TCP */
#ifdef NATIVE_WIN32
int modbus_init_listen_tcp(modbus_param_t *mb_param)
{
		perror("not implemented win32");
	    exit(1);
}
#else /* NATIVE_WIN32 */
int modbus_init_listen_tcp(modbus_param_t *mb_param)
{
	int ret;
	int new_socket;
	struct sockaddr_in addr;
	socklen_t addrlen;

	addr.sin_family = AF_INET;
	/* If the modbus port is < to 1024, we need the setuid root. */
	addr.sin_port = htons(mb_param->port);
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(addr.sin_zero), '\0', 8);

	new_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (new_socket < 0) {
		perror("socket");
		exit(1);
	} else {
		printf("Socket OK\n");
	}

	ret = bind(new_socket, (struct sockaddr *)&addr,
		   sizeof(struct sockaddr_in));
	if (ret < 0) {
		perror("bind");
		close(new_socket);
		exit(1);
	} else {
		printf("Bind OK\n");
	}

	ret = listen(new_socket, 1);
	if (ret != 0) {
		perror("listen");
		close(new_socket);
		exit(1);
	} else {
		printf("Listen OK\n");
	}

	addrlen = sizeof(struct sockaddr_in);
	mb_param->fd = accept(new_socket, (struct sockaddr *)&addr, &addrlen);
	if (mb_param->fd < 0) {
		perror("accept");
		close(new_socket);
		new_socket = 0;
		exit(1);
	} else {
		printf("The client %s is connected\n",
		       inet_ntoa(addr.sin_addr));
	}

	return new_socket;
}
#endif /* NATIVE_WIN32 */

/** Utils **/

/* Sets many input/coil status from a single byte value (all 8 bits of
   the byte value are setted) */
void set_bits_from_byte(uint8_t *dest, int address, const uint8_t value)
{
	int i;

	for (i=0; i<8; i++) {
		dest[address+i] = (value & (1 << i)) ? ON : OFF;
	}
}

/* Sets many input/coil status from a table of bytes (only the bits
   between address and address + nb_bits are setted) */
void set_bits_from_bytes(uint8_t *dest, int address, int nb_bits,
			 const uint8_t tab_byte[])
{
	int i;
	int shift = 0;

	for (i = address; i < address + nb_bits; i++) {
		dest[i] = tab_byte[(i - address) / 8] & (1 << shift) ? ON : OFF;
		/* gcc doesn't like: shift = (++shift) % 8; */
		shift++;
		shift %= 8;
	}
}

/* Gets the byte value from many input/coil status.
   To obtain a full byte, set nb_bits to 8. */
uint8_t get_byte_from_bits(const uint8_t *src, int address, int nb_bits)
{
	int i;
	uint8_t value = 0;

	if (nb_bits > 8) {
		printf("Error: nb_bits is too big\n");
		nb_bits = 8;
	}

	for (i=0; i < nb_bits; i++) {
		value |= (src[address+i] << i);
	}

	return value;
}


void modbus_poll(modbus_param_t*mb_param)
{
	uint8_t msg[MAX_MESSAGE_LENGTH];
	int msg_len = 0;
	const int ret = modbus_listen( mb_param, msg, &msg_len, 500 );	/* wait for 0.5 ms */
	if( ( ret == COMM_TIME_OUT && msg_len > 0 ) || ret >= 0 )
	{
		const int o = mb_param->header_length;
		const int slave = msg[o+0];
		const int func = msg[o+1];
		const int datalen = msg_len - mb_param->header_length - mb_param->checksum_length - 2;
		int addr = 0;
		int nb = -1;
		int isQuery = 1;
		switch( func )
		{
			case FC_READ_COIL_STATUS:
			case FC_READ_INPUT_STATUS:
				if( msg[o+2] == datalen-1 )
				{
					isQuery = 0;
					nb = (datalen-1) * 8;
				}
				break;
			case FC_READ_HOLDING_REGISTERS:
			case FC_READ_INPUT_REGISTERS:
				if( msg[o+2] == datalen-1 )
				{
					isQuery = 0;
					nb = (datalen-1) / 2;
				}
				break;
			case FC_FORCE_SINGLE_COIL:
			case FC_PRESET_SINGLE_REGISTER:
				/* can't decide from message whether it is a query or response */
				isQuery = 0;
				nb = 1;
				addr = ( msg[o+2] << 8 ) | msg[o+3];
				break;
			case FC_REPORT_SLAVE_ID:
				nb = 0;
			case FC_PRESET_MULTIPLE_REGISTERS:
			case FC_FORCE_MULTIPLE_COILS:
			default:
				/* can't decide from message whether it is a query or response */
				isQuery = 0;
				break;
		}
		if( nb == -1 )	/* is query or a write-response? */
		{
			addr = ( msg[o+2] << 8 ) | msg[o+3];
			nb = ( msg[o+4] << 8 ) | msg[o+5];
		}
//		busMonitorAddItem( isQuery,				/* is query */
//					slave,				/* slave */
//					func,				/* func */
//					addr,				/* addr */
//					nb,				/* nb */
//					( msg[msg_len-2] << 8 ) | msg[msg_len-1]	/* CRC */
//				);
	}
}
