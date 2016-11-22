#ifndef PROTOCOL_HH_
# define PROTOCOL_HH_

# define MAGIC_VALUE 42

#include <iostream>

typedef char STRING[64];
typedef char TEXT[1024];
typedef char PWD[256];
# define SOUND_SIZE 256
typedef char SOUND[SOUND_SIZE];

/**
 * Command code from client to server
 */

namespace cmd
{
  typedef enum  e_cmd
  {
    REGISTER = 1,
    LOGIN,
    LOGOUT,
    GET_CONTACT_LIST,
    ADD_CONTACT,
    DEL_CONTACT,
    REP_CONTACT,
    SEND_CALL,
    RESPONSE_CALL,
    END_CALL,
    SEND_TEXT,
    GET_ALL_CONTACT,
    GET_CONTACT_ONLINE,
    GET_CONV_HISTORY
  }             t_cmd;
}



/**
 * Command code of the server's responses
 */

namespace rsp
{
  typedef enum e_response
  {
      ADD_CONTACT = 105,
			REP_CONTACT = 107,
      SEND_CALL,
      RESPONSE_CALL,
      END_CALL,
      RECEIVE_TEXT,
      NEW_CONN = 113,
      DECONN
  }           t_rsp;
}

/**
 * Error code of the server's reponses
 */

namespace err
{
  typedef enum e_error
  {
    USER_EXISTS = 401,
    NO_USER,
    BAD_PASSWORD,
    UNKOWN,
    ALREADY_IN_CONTACTS,
    USER_OFFLINE,
    USER_IN_CALL,
    USER_NOT_CONNECTED,
    CANT_ADD_YOURSELF
  }           t_err;
}

/**
 * Code of client to client commands
 */

namespace c2c
{
  typedef enum e_c2c
  {
    SOUND = 601,
    VIDEO = 602
  }           t_c2c;
}

typedef struct s_header
{
	unsigned int	magic;
	unsigned int	cmd;
}							 t_hdr;

  enum Status
  {
    OK = 0,
    NO
  };

	typedef struct s_rsp
	{
		unsigned int  status;
		// STRING				*users;
	}								t_rsp;

	typedef struct s_usrpwd
	{
		STRING			user;
		PWD			    pwd;
	}							 t_usrpwd;

	typedef struct s_usr
	{
		STRING			user;
	}							t_usr;

	typedef struct s_usrrep
	{
		STRING				user;
		unsigned int	rep;
	}								t_usrrep;

	typedef struct s_usrsck
	{
		STRING				user;
		STRING				ip;
		unsigned int	port;
	}							t_usrsck;

  typedef struct s_usrtxt
  {
    STRING       user;
		TEXT				 txt;
  }              t_usrtxt;

  typedef struct s_snd
  {
    long unsigned int idx;
    unsigned int  size;
    //unsigned char        *data;
  }               t_snd;

  typedef struct s_hdrsnd
  {
    unsigned int	magic;
  	unsigned int	cmd;
    STRING        user;
    unsigned int  size;
    SOUND         sound;
  }               t_hs;

  typedef struct  s_conv
  {
    STRING      user1;
    STRING      user2;
    TEXT        txt;
  }                t_conv;

#endif /* !PROTOCOL_HH_ */
