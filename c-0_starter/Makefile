##
 # MIT License
 #
 # Copyright (c) 2024 Gwenael Saint-Genest <gwen@cowlab.fr>
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 # SOFTWARE.
##
CFLAGS=-g -Wall -pedantic -Wextra
LDFLAGS=-lcurl

all: starter_1 starter_2 starter_3 starter_4 starter_5 starter_6

starter_1: _game_.o 01_variables.o
	@echo "  [LD] 01_variables"
	@cc $(CFLAGS) $(LDFLAGS) -o 01_variables _game_.o 01_variables.o
	@echo ""

starter_2: _game_.o 02_concatenation.o
	@echo "  [LD] 02_concatenation"
	@cc $(CFLAGS) $(LDFLAGS) -o 02_concatenation _game_.o 02_concatenation.o
	@echo ""

starter_3: _game_.o 03_condition.o
	@echo "  [LD] 03_condition"
	@cc $(CFLAGS) $(LDFLAGS) -o 03_condition _game_.o 03_condition.o
	@echo ""

starter_4: _game_.o 04_boucle_for.o
	@echo "  [LD] 04_boucle_for"
	@cc $(CFLAGS) $(LDFLAGS) -o 04_boucle_for _game_.o 04_boucle_for.o
	@echo ""

starter_5: _game_.o 05_foreach.o
	@echo "  [LD] 05_foreach"
	@cc $(CFLAGS) $(LDFLAGS) -o 05_foreach _game_.o 05_foreach.o
	@echo ""

starter_6: _game_.o 06_fonction.o
	@echo "  [LD] 06_fonction"
	@cc $(CFLAGS) $(LDFLAGS) -o 06_fonction _game_.o 06_fonction.o
	@echo ""

test: _game_.o test.o
	cc $(CFLAGS) $(LDFLAGS) -o test _game_.o test.o

clean:
	rm -f 01_variables     01_variables.o
	rm -f 02_concatenation 02_concatenation.o
	rm -f 03_condition     03_condition.o
	rm -f 04_boucle_for    04_boucle_for.o
	rm -f 05_foreach       05_foreach.o
	rm -f 06_fonction      06_fonction.o
	rm -f _game_.o
	rm -f *~

%.o: %.c
	@echo "  [CC] $@"
	@cc $(CFLAGS) -c $< -o $@
