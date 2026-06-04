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

test_quad_eq: clear quad_eq_test
	./quad_eq_test

test_stack: clear stack_test
	./stack_test