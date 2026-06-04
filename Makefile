clear:
	rm -rf *.o *.a *_test

check_fmt:
	clang-format -style=LLVM -i `find -regex ".+\.[ch]"` --dry-run --Werror

fmt:
	clang-format -style=LLVM -i `find -regex ".+\.[ch]"`

%.o: %.c %.h
	gcc -g -c $< -o $@ -lm

%.a: %.o
	ar rc $@ $^

%_test.o: %_test.c %.h
	gcc -g -c $< -o $@

%_test: %_test.o %.a
	gcc -g -o $@ $^ -lm

test:
	@srcs=`ls *.c | grep -v "_test.c"`; \
	for test_src in *_test.c; do \
		bin=$${test_src%.c}; \
		gcc -Wall -Wextra -Werror $$test_src $$srcs -o $$bin; \
	done
	@for test in *_test; do \
		echo "$$test"; \
		./$$test; \
	done