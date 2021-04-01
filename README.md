REXEC: Remotely execute on host from CMM2
-----------------------------------------
Author: Epsilon

Changelog
---------
0.2: 
- Fix for transfer of very long files
- Show transfer activity in rexec.py terminal window.

0.1: Initial version

Description
-----------
REXEC allows you to remotely execute commands on an attached host machine from the CMM2 command line.

For example:

	> *rexec shell dir /w

	REXEC07101972 shell_cmd_to_pc  dir /w

	REXEC07101972 shell_ret_from_pc
	0
	REXEC07101972_DONE
	REXEC07101972 shell_out_from_pc
	 Volume in drive C is OS
	 Volume Serial Number is 8497-A711

	 Directory of c:\cmm2\rexec\host

	[.]                     [..]                    arm-gcc-link.ld
	armcfgenV143join.bas    armcfgenV143merge.bas   ARMCFunctions.h
	Make.bat                rexec.py
	               6 File(s)         54,007 bytes
	               2 Dir(s)  79,417,647,104 bytes free

	REXEC07101972_DONE
	REXEC07101972 shell_err_from_pc

	REXEC07101972_DONE

Commmunication goes between a rexec.bas client on the CMM2 and a rexec.py server on the host, connected via serial.
Following commands are supported on the CMM2 side:

rexec upload <local file path> [remote file path]
rexec download <remote file path> [local file path]
rexec shell <remote shell command>
rexec quit: Request REXEC server to quit.

Upload and download use the xmodem protocol but correct for filesize, so you don't end up with those pesky trailing bytes at the end of a transfered file.

The server on the host is started as follows:

python rexec.py -p <serial port> -b <baudrate>
Example:
python rexec.py -p COM8 -b 115200

rexec.inc and csubgen:
---------------------
rexec.bas just forwards the command line string to rexec.inc, so you can easily build rexec based scripts by #including rexec.inc.

An example of such a script is csubgen.bas, also included in this package. csubgen remote compiles a given C file and optional headers and transfers the resulting CSUB back to CMM2.

Example:

	*csubgen merge parseGetNextCSUB.c

	REXEC07101972 xmodem_send_to_pc parseGetNextCSUB.c 5286
	Done.

	REXEC07101972 shell_cmd_to_pc  make parseGetNextCSUB merge

	REXEC07101972 shell_ret_from_pc
	0
	REXEC07101972_DONE
	REXEC07101972 shell_out_from_pc

	REXEC07101972_DONE
	REXEC07101972 shell_err_from_pc

	REXEC07101972_DONE

	Done.
	Remote compile OK.

	REXEC07101972 xmodem_receive_from_pc parseGetNextCSUB.bas
	1724
	Done.

So you can now kick off CSUB compilations from the comfort of your CMM2 :-)

Required CMM2 firmware version
------------------------------
V5.06.00

Required Python version
-----------------------
3.x

csubgen assumes a Windows host with the GNU ARM emmbedded toolchain end DOS MMBasic are installed and in the path.

ToDos
-----
Accept wildcards in rexec upload/download filespec.

Limitations
-----------
rexec is only tested against a Windows host but should also work with MacOSX or Linux.
Output is too verbose.

GitHub
------
https://github.com/epsilon537/rexec_cmm2
