#ifndef SHORT_MSG_C_CALL_CAPP_MIDDLE_WARE_CPP
#define SHORT_MSG_C_CALL_CAPP_MIDDLE_WARE_CPP


#include "common.h"
#include "c_call_cpp_middle_ware.h"


#ifdef __cplusplus
extern "C" {
#endif

	const char * get_cmd_from_sockfd_c(int fd){
		string res =  get_cmd_from_sockfd(fd);
		return res.c_str();
	}

#ifdef __cplusplus
}
#endif
#endif