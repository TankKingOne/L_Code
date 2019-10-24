ser=server 
cli=client 
cc=gcc 

.PHONY:all
all:$(ser) $(cli)

$(cli):client.c
	$(cc) -o $@ $^
$(ser):server.c
	$(cc) -o $@ $^
.PHONY:clean
clean:
	rm -f $(cli) $(ser)
