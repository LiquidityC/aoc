all:
	@make -sC .build/
.PHONY: all

run: all
	@.build/main
.PHONY: run

dbg: all
	@gdb .build/main
.PHONY: dbg

clean:
	@make -sC .build/ clean
.PHONY: clean
