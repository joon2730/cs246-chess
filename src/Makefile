CXX = g++
CXXFLAGS =-std=c++14 -Wall -Werror=vla -MMD -I/opt/X11/include
LDFLAGS = -L/opt/X11/lib -lX11
EXEC = chess
OBJECTS = main.o chessgame.o board.o square.o move.o piece.o pawn.o knight.o bishop.o rook.o queen.o king.o math.o player.o human.o computerlevel1.o computerlevel2.o computerlevel3.o computerlevel4.o computerlevel5.o openingbook.o chessai.o subject.o textdisplay.o graphicdisplay.o window.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${LDFLAGS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS}

