$! INSTALL.COM -- Installs the files in a given directory tree
$!
$! Author: Richard Levitte <richard@levitte.org>
$! Time of creation: 22-MAY-1998 10:13
$!
$! P1	root of the directory tree
$!
$	IF P1 .EQS. ""
$	THEN
$	    WRITE SYS$OUTPUT "First argument missing."
$	    WRITE SYS$OUTPUT "Should be the directory where you want things installed."
$	    EXIT
$	ENDIF
$
$	ROOT = F$PARSE(P1,"[]A.;0",,,"SYNTAX_ONLY,NO_CONCEAL") - "A.;0"
$	ROOT_DEV = F$PARSE(ROOT,,,"DEVICE","SYNTAX_ONLY")
$	ROOT_DIR = F$PARSE(ROOT,,,"DIRECTORY","SYNTAX_ONLY") -
		   - "[000000." - "][" - "[" - "]"
$	ROOT = ROOT_DEV + "[" + ROOT_DIR
$
$	DEFINE/NOLOG WRK_SSLROOT 'ROOT'.] /TRANS=CONC
$	DEFINE/NOLOG WRK_SSLVEXE WRK_SSLROOT:[VAX_EXE]
$	DEFINE/NOLOG WRK_SSLAEXE WRK_SSLROOT:[ALPHA_EXE]
$	DEFINE/NOLOG WRK_SSLLIB WRK_SSLROOT:[LIB]
$
$	IF F$PARSE("WRK_SSLROOT:[000000]") .EQS. "" THEN -
	   CREATE/DIR/LOG WRK_SSLROOT:[000000]
$	IF F$PARSE("WRK_SSLVEXE:") .EQS. "" THEN -
	   CREATE/DIR/LOG WRK_SSLVEXE:
$	IF F$PARSE("WRK_SSLAEXE:") .EQS. "" THEN -
	   CREATE/DIR/LOG WRK_SSLAEXE:
$	IF F$PARSE("WRK_SSLLIB:") .EQS. "" THEN -
	   CREATE/DIR/LOG WRK_SSLLIB:
$
$	EXE := openssl
$
$	VEXE_DIR := [-.VAX.EXE.APPS]
$	AEXE_DIR := [-.AXP.EXE.APPS]
$
$	I = 0
$ LOOP_EXE: 
$	E = F$EDIT(F$ELEMENT(I, ",", EXE),"TRIM")
$	I = I + 1
$	IF E .EQS. "," THEN GOTO LOOP_EXE_END
$	SET NOON
$	IF F$SEARCH(VEXE_DIR+E+".EXE") .NES. ""
$	THEN
$	  COPY 'VEXE_DIR''E'.EXE WRK_SSLVEXE:'E'.EXE/log
$	  SET FILE/PROT=W:RE WRK_SSLVEXE:'E'.EXE
$	ENDIF
$	IF F$SEARCH(AEXE_DIR+E+".EXE") .NES. ""
$	THEN
$	  COPY 'AEXE_DIR''E'.EXE WRK_SSLAEXE:'E'.EXE/log
$	  SET FILE/PROT=W:RE WRK_SSLAEXE:'E'.EXE
$	ENDIF
$	SET ON
$	GOTO LOOP_EXE
$ LOOP_EXE_END:
$
$	SET NOON
$	COPY CA.COM WRK_SSLAEXE:CA.COM/LOG
$	SET FILE/PROT=W:RE WRK_SSLAEXE:CA.COM
$	COPY CA.COM WRK_SSLVEXE:CA.COM/LOG
$	SET FILE/PROT=W:RE WRK_SSLVEXE:CA.COM
$	COPY OPENSSL-VMS.CNF WRK_SSLROOT:[000000]OPENSSL.CNF/LOG
$	SET FILE/PROT=W:R WRK_SSLROOT:[000000]OPENSSL.CNF
$	SET ON
$
$	EXIT
