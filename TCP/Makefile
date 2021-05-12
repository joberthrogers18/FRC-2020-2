CC = gcc
BLDDIR = .
MAINFILE_CLIENT = clientTCP.c
MAINFILE_SERVER = serverTCP.c
EXE_SERVER = server
EXE_CLIENT = client

all: 
	$(CC) ${MAINFILE_SERVER} -o ${EXE_SERVER} -lpthread && $(CC) ${MAINFILE_CLIENT} -o ${EXE_CLIENT} -lpthread

clean:
	rm -f $(EXE_CLIENT) && rm -f $(EXE_SERVER)

run-server:
	${BLDDIR}/${EXE_SERVER}

run-client:
	${BLDDIR}/${EXE_CLIENT}