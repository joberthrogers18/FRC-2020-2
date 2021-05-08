CC = gcc
BLDDIR = .
MAINFILE_CLIENT = clientUDP.c
MAINFILE_SERVER = serverUDP.c
EXE_SERVER = server
EXE_CLIENT = client

all: 
	$(CC) ${MAINFILE_SERVER} -o ${EXE_SERVER} && $(CC) ${MAINFILE_CLIENT} -o ${EXE_CLIENT}

clean:
	rm -f $(EXE_CLIENT) && rm -f $(EXE_SERVER)

run-server:
	${BLDDIR}/${EXE_SERVER}

run-client:
	${BLDDIR}/${EXE_CLIENT}